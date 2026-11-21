#ifdef DREAMCAST
#include <kos.h>
extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT);
KOS_INIT_ROMDISK(romdisk);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL.h>
#include <ctype.h>
#include <string.h>

#include "neocd.h"
#include "icon.h"

#include "console.h"

#ifndef BEGINTRACE
#define BEGINTRACE 0
#endif
#ifndef TOTRACE
#define TOTRACE 0 
#endif
// #define KTOTRACE 0
#define CPU_BLOQUE 20000
int trazando=0;
Uint32 tiempoactual=0;
int neogeo_emulating=0;
int	neogeo_sound_enable=1;
int	neogeo_frameskip=-1;

static int neogeo_frameskip_count=0;

#ifndef DREAMCAST
static char		neocd_wm_title[255]=VERSION1;
#endif

int neo4all_skip_next_frame=0;
Uint32 neo4all_z80_cycles=NEO4ALL_Z80_UNDER_CYCLES;
Uint32 neo4all_68k_cycles=NEO4ALL_68K_NORMAL_CYCLES;

//-- Global Variables --------------------------------------------------------
char			*neogeo_rom_memory = NULL;
char			*neogeo_prg_memory = NULL;
char			*neogeo_fix_memory = NULL;
char			*neogeo_spr_memory = NULL;
char			*neogeo_pcm_memory = NULL;

char			global_error[80];
unsigned char		neogeo_memorycard[8192];
int			neogeo_prio_mode = 0;
int			neogeo_ipl_done = 0;
char			neogeo_region=REGION;
unsigned char		config_game_name[80];

Uint32		neocd_time; /* next time marker */


//-- 68K Core related stuff --------------------------------------------------
int				mame_debug = 0;
int				previouspc = 0;
int				ophw = 0;
int				cur_mrhard = 0;


//-- Function Prototypes -----------------------------------------------------
void	neogeo_init(void);
void	neogeo_reset(void);
void	neogeo_hreset(void);
void	neogeo_shutdown(void);
void	MC68000_Cause_Interrupt(int);
void	neogeo_exception(void);
void	neogeo_run(void);
void	draw_main(void);
void	neogeo_quit(void);
void	not_implemented(void);
void	neogeo_machine_settings(void);
void	neogeo_debug_mode(void);
void	neogeo_cdda_check(void);
void	neogeo_cdda_control(void);
void	neogeo_do_cdda( int command, int trck_number_bcd);
void	neogeo_read_gamename(void);

//----------------------------------------------------------------------------
int	main(int argc, char* argv[])
{
	FILE			*fp;
	SDL_RWops	*iconfp;
	SDL_Surface *icon;
	int				result;
	//int				a, b, c,w,h,i;
	
	neogeo_emulating=0;
	char * fixtmp;

	puts("MAIN!!!");
	// Initialise SDL
	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_CDROM|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK)==-1)) { 
            console_printf("Could not initialize SDL: %s.\n", SDL_GetError());
            exit(-1);
        }
#ifndef DREAMCAST
	// Register exit procedure
	atexit(neogeo_shutdown);
#endif
	
	// Video init
	result = video_init();
	if (!result)
	{
		video_mode = 0;
		console_puts(global_error);
		return 0;
	}
#ifndef DREAMCAST
	//set window title
	SDL_WM_SetCaption(neocd_wm_title,neocd_wm_title);	

	//Hide Mouse Pointer
	SDL_ShowCursor(SDL_DISABLE);
	//set up window icon
	iconfp=SDL_RWFromMem((void*)rawicon, sizeof(rawicon));
	icon=SDL_LoadBMP_RW(iconfp,0);
	SDL_WM_SetIcon(icon,NULL);
#endif

	// Allocate needed memory buffers
	
	console_puts("NEOGEO: Allocating memory...");

	neogeo_prg_memory = calloc(1, 0x200000);
	if (neogeo_prg_memory==NULL) {
		console_puts("failed !");
		return 0;
	}

	neogeo_spr_memory = calloc(1, 0x400000);
	if (neogeo_spr_memory==NULL) {
		console_puts("failed !");
		return 0;
	}
	
	neogeo_rom_memory = calloc(1, 0x80000);
	if (neogeo_rom_memory==NULL) {
		console_puts("failed !");
		return 0;
	}

	neogeo_fix_memory = calloc(1, 0x20000);
	if (neogeo_fix_memory==NULL) {
		console_puts("failed !");
		return 0;
	}

	neogeo_pcm_memory = calloc(1, 0x100000);
	if (neogeo_pcm_memory==NULL) {
		console_puts("failed !");
		return 0;
	}
	
	// Initialize Memory Mapping
	initialize_memmap();

	// Read memory card
	fp = fopen(DATA_PREFIX "memcard.bin", "rb");
	if (fp!=NULL) {
		fread(neogeo_memorycard, 1, 8192, fp);
		fclose(fp);
	}

	// Load BIOS
	fp = fopen(ROM_PREFIX "neocd.bin", "rb");
	if (fp==NULL) {
		console_println();
		console_printf("Fatal Error: Could not load NEOCD.BIN\n");
		console_wait();
		return 0;
	}
	fread(neogeo_rom_memory, 1, 0x80000, fp);
	fclose(fp);

	fixtmp=calloc(1, 65536);
	memcpy(fixtmp,&neogeo_rom_memory[458752],65536);
	
	fix_conv(fixtmp,&neogeo_rom_memory[458752],65536,rom_fix_usage);
	/*swab(neogeo_rom_memory, neogeo_rom_memory, 131072);*/
	free(fixtmp);
	fixtmp=NULL;

	// Load startup RAM
	fp = fopen(DATA_PREFIX "startup.bin", "rb");
	if (fp==NULL) {
		console_println();
		console_printf("Fatal Error: Could not load STARTUP.BIN\n");
		console_wait();
		return 0;
	}
	fread(neogeo_prg_memory + 0x10F300, 1, 3328, fp);
	fclose(fp);
	swab(neogeo_prg_memory + 0x10F300, neogeo_prg_memory + 0x10F300, 3328);


	// Check BIOS validity
	if (*((short*)(neogeo_rom_memory+0xA822)) != 0x4BF9)
	{
		console_println();
		console_printf("Fatal Error: Invalid BIOS file.");
		console_wait();
		return 0;
	}


	/*** Patch BIOS load files w/ now loading message ***/
	*((short*)(neogeo_rom_memory+0x552)) = 0xFABF;
	*((short*)(neogeo_rom_memory+0x554)) = 0x4E75;
	/*** Patch BIOS load files w/out now loading ***/
	*((short*)(neogeo_rom_memory+0x564)) = 0xFAC0;
	*((short*)(neogeo_rom_memory+0x566)) = 0x4E75;
	/*** Patch BIOS CDROM Check ***/
	*((short*)(neogeo_rom_memory+0xB040)) = 0x4E71;
	*((short*)(neogeo_rom_memory+0xB042)) = 0x4E71;
	/*** Patch BIOS upload command ***/
	*((short*)(neogeo_rom_memory+0x546)) = 0xFAC1;
	*((short*)(neogeo_rom_memory+0x548)) = 0x4E75;

	/*** Patch BIOS CDDA check ***/
	*((short*)(neogeo_rom_memory+0x56A)) = 0xFAC3;
	*((short*)(neogeo_rom_memory+0x56C)) = 0x4E75;

	/*** Full reset, please ***/
	*((short*)(neogeo_rom_memory+0xA87A)) = 0x4239;
	*((short*)(neogeo_rom_memory+0xA87C)) = 0x0010;
	*((short*)(neogeo_rom_memory+0xA87E)) = 0xFDAE;

	/*** Trap exceptions ***/
	*((short*)(neogeo_rom_memory+0xA5B6)) = 0x4AFC;

	// Initialise input
	input_init();

#ifdef Z80_EMULATED
	// Sound init
	init_sdl_audio();
#endif

	init_text(1);

	do{
  		run_mainMenu();
	}while(!neogeo_cdrom_init1());

	console_pag();
	console_println();
	console_puts(VERSION1);
	console_puts(VERSION2);
	console_println();

	cdda_current_drive=neogeo_cdrom_current_drive;
	cdda_init();
	
	// Initialize everything
	neogeo_init();
	pd4990a_init();
	neogeo_run();
	neogeo_shutdown();
	return 0;
}

//----------------------------------------------------------------------------
void	neogeo_init(void)
{
	_68k_init();
	_68k_reset();
}

//----------------------------------------------------------------------------
void	neogeo_hreset(void)
{
	neogeo_ipl_done = 0;
	sound_disable();
	while(!neogeo_ipl_done)
	{
		neogeo_cdrom_load_title();
		if (!neogeo_cdrom_process_ipl()) {
			console_println();
			console_printf("Error: Error while processing IPL.TXT.\n");
			console_wait();
			run_mainMenu();
		}
		else
			neogeo_ipl_done = 1;
	}

	FILE * fp;

	/* read game name */
	neogeo_read_gamename();

	/* Special patch for Samurai Spirits RPG */
	if (strcmp(config_game_name, "TEST PROGRAM USA") == 0)
	{
		strcpy(config_game_name, "SAMURAI SPIRITS RPG");
		
		fp = fopen(DATA_PREFIX "patch.prg", "rb");
		if (fp == NULL) {
			console_printf("Fatal Error: Couldnt open patch.prg.\n");
			SDL_Delay(1000);
			exit(1);
		}
		
		fread(neogeo_prg_memory + 0x132000, 1, 112, fp);
		fclose(fp);
		swab(neogeo_prg_memory + 0x132000, neogeo_prg_memory + 0x132000, 112);
	
	}

#ifndef DREAMCAST
	/* update window title with game name */
	strcat(neocd_wm_title," - ");
	strcat(neocd_wm_title,config_game_name);
	SDL_WM_SetCaption(neocd_wm_title,neocd_wm_title);
#endif

	// First time init
	_68k_reset();
	_68k_set_register(_68K_REG_PC,0xc0a822);
	_68k_set_register(_68K_REG_SR,0x2700);
	_68k_set_register(_68K_REG_A7,0x10F300);
//	_68k_set_register(_68K_REG_ASP,0x10F400);
	
	m68k_write_memory_32(0x10F6EE, m68k_read_memory_32(0x68L)); // $68 *must* be copied at 10F6EE

	if (m68k_read_memory_8(0x107)&0x7E)
	{
		if (m68k_read_memory_16(0x13A))
		{
			m68k_write_memory_32(0x10F6EA, (m68k_read_memory_16(0x13A)<<1) + 0xE00000);
		}
		else
		{
			m68k_write_memory_32(0x10F6EA, 0);
			m68k_write_memory_8(0x00013B, 0x01);
		}
	}
	else
		m68k_write_memory_32(0x10F6EA, 0xE1FDF0);

	/* Set System Region */
	m68k_write_memory_8(0x10FD83,neogeo_region);

	cdda_current_track = 0;
	cdda_get_disk_info();

#ifdef Z80_EMULATED
	_z80_init();
#endif
	sound_enable();
}	

//----------------------------------------------------------------------------
void	neogeo_reset(void)
{

	_68k_reset();
	_68k_set_register(_68K_REG_PC,0x122);
	_68k_set_register(_68K_REG_SR,0x2700);
	_68k_set_register(_68K_REG_A7,0x10F300);
//	_68k_set_register(_68K_REG_ASP,0x10F400);

	m68k_write_memory_8(0x10FD80, 0x82);
	m68k_write_memory_8(0x10FDAF, 0x01);
	m68k_write_memory_8(0x10FEE1, 0x0A);
	m68k_write_memory_8(0x10F675, 0x01);
	m68k_write_memory_8(0x10FEBF, 0x00);
	m68k_write_memory_32(0x10FDB6, 0);
	m68k_write_memory_32(0x10FDBA, 0);

	/* System Region */
	m68k_write_memory_8(0x10FD83,neogeo_region);

	cdda_current_track = 0;
#ifdef Z80_EMULATED
	_z80_init();
#endif
}

//----------------------------------------------------------------------------
void	neogeo_shutdown(void)
{
	FILE	*fp;
	
	// Close everything and free memory
	cdda_shutdown();
	neogeo_cdrom_shutdown();
#ifdef Z80_EMULATED
	sound_shutdown();
#endif
	input_shutdown();
	video_shutdown();

	console_printf("NEOGEO: System Shutdown.\n");

	fp = fopen(DATA_PREFIX "memcard.bin", "wb");
	if (fp != NULL) {	
		fwrite(neogeo_memorycard, 1, 8192, fp);
		fclose(fp);
	} else {
		console_printf("Error: Couldn't open memcard.bin for writing.\n");
	}

	free(neogeo_prg_memory);
	free(neogeo_rom_memory);
	free(neogeo_spr_memory);
	free(neogeo_fix_memory);
	free(neogeo_pcm_memory);
	
	SDL_Quit();
	
	return;
}

//----------------------------------------------------------------------------
void	neogeo_exception(void)
{
	console_printf("NEOGEO: Exception Trapped at %08x !\n", previouspc);
	SDL_Delay(1000);
	exit(0);
}	

//----------------------------------------------------------------------------
/*
void MC68000_Cause_Interrupt(int level)
{
	_68k_raise_interrupt(level,M68K_AUTOVECTORED_INT);
	_68k_emulate(10000);
	_68k_lower_interrupt(level);
}
*/

//----------------------------------------------------------------------------
void	neogeo_exit(void)
{
	console_puts("NEOGEO: Exit requested by software...");
	SDL_Delay(1000);
	exit(0);
}

void neogeo_adjust_frameskip(int new_frameskip)
{
    	neocd_time=SDL_GetTicks()+REFRESHTIME;
	neogeo_frameskip=new_frameskip;
	neogeo_frameskip_count=0;
}

void neogeo_adjust_cycles(int new_68k, int new_z80)
{
	switch(new_68k)
	{
		case 0:
			neo4all_68k_cycles=NEO4ALL_68K_UNDER_CYCLES;
			break;
		case 2:
			neo4all_68k_cycles=NEO4ALL_68K_OVER_CYCLES;
			break;
		default:
			neo4all_68k_cycles=NEO4ALL_68K_NORMAL_CYCLES;
	}
	switch(new_z80)
	{
		case 0:
			neo4all_z80_cycles=NEO4ALL_Z80_UNDER_CYCLES;
			break;
		case 2:
			neo4all_z80_cycles=NEO4ALL_Z80_OVER_CYCLES;
			break;
		default:
			neo4all_z80_cycles=NEO4ALL_Z80_NORMAL_CYCLES;
	}
}

#ifdef PROFILER
Uint32 profiler_media1=0;
Uint32 profiler_media2=0;
Uint32 profiler_media3=0;
Uint32 profiler_media4=0;
Uint32 profiler_actual=0;
#endif
//----------------------------------------------------------------------------
void	neogeo_run(void)
{
	Uint32	now;
    	int	i;

	neogeo_emulating=1;
	console_puts("NEOGEO RUN ...");

	// If IPL.TXT not loaded, load it !
	if (!neogeo_ipl_done)
		neogeo_hreset();

	/* get time for speed throttle */
    	neocd_time=SDL_GetTicks()+REFRESHTIME;
	
	tiempoactual=SDL_GetTicks();
	// Main loop
#ifdef Z80_EMULATED
	my_timer();
	z80_cycles = neo4all_z80_cycles >> 8;
	_z80raise(0);
#endif
	unsigned long long ciclo=0;
#ifdef PROFILER
#endif
#ifndef DEBUG_NEO4ALL
#ifndef PROFILER_TIME
	while(1)
#else
	while(ciclo<PROFILER_TIME)
#endif
#endif
	{
//puts("WHILE");fflush(stdout);
//printf("CICLO=%i\n",ciclo);SDL_Delay(250); fflush(stdout); ciclo++;
#ifdef PROFILER
ciclo++;
#endif
#ifdef Z80_EMULATED
#ifdef PROFILER
profiler_actual=SDL_GetTicks();
#endif
	if (neogeo_sound_enable)
	{
/*
		// Execute Z80 timeslice (one VBL)
		_z80raise(0);
		
		if (SDL_GetAudioStatus()==SDL_AUDIO_PLAYING) {
		    for (i = 0; i < 256; i++) {
			if (z80_cycles>0) {
			    _z80exec(z80_cycles);
			    z80_cycles=0;
			    my_timer();
			}
			z80_cycles += neo4all_z80_cycles >> 8;
		    }
		}
*/
		for (i = 0; i < 256; i++) {
			_z80exec(neo4all_z80_cycles >> 8);
			my_timer();
		}
	}
#ifdef PROFILER
profiler_media1+=(SDL_GetTicks()-profiler_actual);
#endif
#endif
//puts("Z80 EMULADO");fflush(stdout);

#ifdef DEBUG_NEO4ALL
int k,j;
for(j=0;j<=TOTRACE;j++)
{
puts("...");;fflush(stdout);
//for(k=0;k<10;k++)
for(i=0;i<CPU_BLOQUE;i++)
{
if ((j>=BEGINTRACE) ) //&&(k==KTOTRACE))
{
trazando=1;
		//M68K_SUP_ADDR_SPACE | M68K_DATA_ADDR_SPACE),
printf("\t%X %X    PC=%X  FPC=%X  SR=%X\n", ciclo, _68k_get_pc(), _68k_fetch(_68k_get_pc(),0), _68k_get_register(_68K_REG_SR)&0xA71F);
#ifdef DEBUG_REGISTERS
printf("\t\tPC+2=%X\n",_68k_fetch(_68k_get_pc()+2,0));
printf("\t\t +2(A5)=%X\n",
	0xFF&_68k_fetch(_68k_get_register(_68K_REG_A5)+_68k_fetch(_68k_get_pc()+2,0),0)
);

//printf("\t\tPC+4=%X\n",_68k_fetch(_68k_get_pc()+4,0));
printf("A0=%X  A1=%X  A2=%X  A3=%X\n",_68k_get_register(_68K_REG_A0),_68k_get_register(_68K_REG_A1),_68k_get_register(_68K_REG_A2),_68k_get_register(_68K_REG_A3));
printf("A4=%X  A5=%X  A6=%X  A7=%X\n",_68k_get_register(_68K_REG_A4),_68k_get_register(_68K_REG_A5),_68k_get_register(_68K_REG_A6),_68k_get_register(_68K_REG_A7));
printf("D0=%X  D1=%X  D2=%X  D3=%X\n",_68k_get_register(_68K_REG_D0),_68k_get_register(_68K_REG_D1),_68k_get_register(_68K_REG_D2),_68k_get_register(_68K_REG_D3));
printf("D4=%X  D5=%X  D6=%X  D7=%X\n",_68k_get_register(_68K_REG_D4),_68k_get_register(_68K_REG_D5),_68k_get_register(_68K_REG_D6),_68k_get_register(_68K_REG_D7));
#endif
fflush(stdout);
}else trazando=0;
_68k_emulate(1); ciclo++;
}
printf("INT %i\n",j);fflush(stdout);
_68k_interrupt(2);
#ifdef USE_FAME_CORE
if (j>=BEGINTRACE) trazando=1;
_68k_emulate(1);
#endif
puts("pd4990a_addretrace...");;fflush(stdout);
pd4990a_addretrace();
puts("video_draw_screen1...");;fflush(stdout);
video_draw_screen1();
puts("neogeo_cdda_check...");;fflush(stdout);
neogeo_cdda_check();
puts("cdda_loop_check...");;fflush(stdout);
cdda_loop_check();
puts("processEvents...");;fflush(stdout);
processEvents();
}
puts("SALIDA ...");;fflush(stdout);

#else



#ifdef PROFILER
profiler_actual=SDL_GetTicks();
#endif
//printf("ciclo=%u\n",ciclo); fflush(stdout);
		// One-vbl timeslice
		_68k_emulate(neo4all_68k_cycles);
		_68k_interrupt(2);
#ifdef PROFILER
profiler_media2+=(SDL_GetTicks()-profiler_actual);
#endif
//puts("68000 EMULADO"); fflush(stdout);
		/* update pd4990a */
		pd4990a_addretrace();
		
		/* check the watchdog */
		if (watchdog_counter > 0) {
		    if (--watchdog_counter == 0) {
			//logerror("reset caused by the watchdog\n");
			neogeo_reset();
		    }
		}
		
		/* check for memcard writes */
		if (memcard_write > 0) {
		   memcard_write--;
		   if(memcard_write==0) {
		     /* write memory card here */
			 /* if you need to keep file up to date */
		   }
		}

		// Call display routine
		if (neogeo_prio_mode)
			video_draw_screen2();
		else
			video_draw_screen1();

		// Check if there are pending commands for CDDA
		neogeo_cdda_check();
		cdda_loop_check();

		/* Update keys and Joystick */
		processEvents();

		/* Speed Throttle */	

		switch(neogeo_frameskip)
		{
			case 0:
				neo4all_skip_next_frame=0;
				break;
			case 1:
				if (!(neogeo_frameskip_count&0x1))
					neo4all_skip_next_frame=0;
				else
					neo4all_skip_next_frame=1;
				break;
			case 2:
				if (!(neogeo_frameskip_count%3))
					neo4all_skip_next_frame=0;
				else
					neo4all_skip_next_frame=1;
				break;
			case 3:
				if (!(neogeo_frameskip_count&0x3))
					neo4all_skip_next_frame=0;
				else
					neo4all_skip_next_frame=1;
				break;
			case 4:
				if (!(neogeo_frameskip_count%5))
					neo4all_skip_next_frame=0;
				else
					neo4all_skip_next_frame=1;
				break;
			case 5:
				if (!(neogeo_frameskip_count%6))
					neo4all_skip_next_frame=0;
				else
					neo4all_skip_next_frame=1;
				break;
			default:
				now=SDL_GetTicks();
				if (now+3 < neocd_time) {
					neo4all_skip_next_frame=0;
#ifndef DREAMCAST
					SDL_Delay(neocd_time-now);
#endif
				}
				else
				if (now > neocd_time-10) {
					if (!neo4all_skip_next_frame)
						neo4all_skip_next_frame=1;
					else
						neocd_time+=10;
				}
				else
					neo4all_skip_next_frame=0;
				neocd_time+=REFRESHTIME;
		}
		neogeo_frameskip_count++;
#endif
	}
#ifdef PROFILER
printf("CICLOS=%f\n",(double)ciclo);
printf("Z80= %i msec, media = %f \n",profiler_media1, ((double)profiler_media1)/(double)ciclo);
printf("68000= %i msec, media = %f\n",profiler_media2, ((double)profiler_media2)/(double)ciclo);
printf("BLITTER= %i msec, media = %f\n",profiler_media3, ((double)profiler_media3)/(double)ciclo);
printf("FLIP= %i msec, media = %f\n",profiler_media4, ((double)profiler_media4)/(double)ciclo);
#endif
	// Stop CDDA
	cdda_stop();
		
	return;
}

//----------------------------------------------------------------------------
// This is a really dirty hack to make SAMURAI SPIRITS RPG work
void	neogeo_prio_switch(void)
{
	if (_68k_get_register(_68K_REG_D7) == 0xFFFF)
		return;
	
	if (_68k_get_register(_68K_REG_D7) == 9 && 
	    _68k_get_register(_68K_REG_A3) == 0x10DED9 &&
		(_68k_get_register(_68K_REG_A2) == 0x1081d0 ||
		(_68k_get_register(_68K_REG_A2)&0xFFF000) == 0x102000)) {
		neogeo_prio_mode = 0;
		return;
	}
	
	if (_68k_get_register(_68K_REG_D7) == 8 && 
	    _68k_get_register(_68K_REG_A3) == 0x10DEC7 && 
		_68k_get_register(_68K_REG_A2) == 0x102900) {
		neogeo_prio_mode = 0;
		return;
	}
	
	if (_68k_get_register(_68K_REG_A7) == 0x10F29C)
	{
		if ((_68k_get_register(_68K_REG_D4)&0x4010) == 0x4010)
		{
			neogeo_prio_mode = 0;
			return;
		}
		
		neogeo_prio_mode = 1;
	}
	else
	{
		if (_68k_get_register(_68K_REG_A3) == 0x5140)
		{
			neogeo_prio_mode = 1;
			return;
		}

		if ( (_68k_get_register(_68K_REG_A3)&~0xF) == (_68k_get_register(_68K_REG_A4)&~0xF) )
			neogeo_prio_mode = 1;
		else
			neogeo_prio_mode = 0;
	}
}

//----------------------------------------------------------------------------
void	not_implemented(void)
{
		console_printf("Error: This function isn't implemented.");
}

//----------------------------------------------------------------------------
void	neogeo_quit(void)
{
		exit(0);
}

//----------------------------------------------------------------------------
void neogeo_cdda_check(void)
{
	int		Offset;
	
	Offset = m68k_read_memory_32(0x10F6EA);
	if (Offset < 0xE00000)	// Invalid addr
		return;

	Offset -= 0xE00000;
	Offset >>= 1;
	
#ifdef Z80_EMULATED
	if (neogeo_sound_enable)
		neogeo_do_cdda(subcpu_memspace[Offset], subcpu_memspace[Offset+1]);

#endif
}


//----------------------------------------------------------------------------
void neogeo_cdda_control(void)
{
#ifdef PROFILER
	Uint32 ahora=SDL_GetTicks();
#endif
	neogeo_do_cdda( (_68k_get_register(_68K_REG_D0)>>8)&0xFF, 
	                 _68k_get_register(_68K_REG_D0)&0xFF );
#ifdef PROFILER
	profiler_actual+=(SDL_GetTicks()-ahora);
#endif
}

//----------------------------------------------------------------------------
void neogeo_do_cdda( int command, int track_number_bcd)
{
	int		track_number;
	int		offset;

	if ((command == 0)&&(track_number_bcd == 0))
		return;

	m68k_write_memory_8(0x10F64B, track_number_bcd);
	m68k_write_memory_8(0x10F6F8, track_number_bcd);
	m68k_write_memory_8(0x10F6F7, command);
	m68k_write_memory_8(0x10F6F6, command);

	offset = m68k_read_memory_32(0x10F6EA);

	if (offset)
	{
		offset -= 0xE00000;
		offset >>= 1;

		m68k_write_memory_8(0x10F678, 1);

#ifdef Z80_EMULATED
		if (neogeo_sound_enable)
		{
			subcpu_memspace[offset] = 0;
			subcpu_memspace[offset+1] = 0;
		}
#endif
	}

	switch( command )
	{
		case	0:
		case	1:
		case	5:
		case	4:
		case	3:
		case	7:
			track_number = ((track_number_bcd>>4)*10) + (track_number_bcd&0x0F);
			if ((track_number == 0)&&(!cdda_playing))
			{
				//sound_mute();
				cdda_resume();
			}
			else if ((track_number>1)&&(track_number<99))
			{
				//sound_mute();
				cdda_play(track_number);
				cdda_autoloop = !(command&1);
			}
			break;
		case	6:
		case	2:
			if (cdda_playing)
			{
				//sound_mute();
				cdda_pause();
			}
			break;
	}
}
//----------------------------------------------------------------------------
void neogeo_read_gamename(void)
{

	unsigned char	*Ptr;
	int				temp;

	Ptr = neogeo_prg_memory + m68k_read_memory_32(0x11A);
	swab(Ptr, config_game_name, 80);

	for(temp=0;temp<80;temp++) {
		if (!isprint(config_game_name[temp])) {
			config_game_name[temp]=0;
			break;
		}
	}
}

