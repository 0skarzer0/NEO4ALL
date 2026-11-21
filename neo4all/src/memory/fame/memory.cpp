/********************************************
*   NeoCD Memory Mapping (C version)        *
*********************************************
* Fosters(2001,2003)                        *
********************************************/

// #define ONLY_CHANGES_PATCH

#include <SDL.h>
#include "SDL_endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "fame/fame.h"

#include "z80/z80intrf.h"

#define Z80_TIMESLICE_PER_NMI 192

#ifdef DEBUG_MEMORY
extern int trazando;
#endif

#include "console.h"

#include "../neocd.h"

extern unsigned short    *video_paletteram_ng;
extern unsigned	video_palette_selected;

/*** Globals ***************************************************************************************/
int watchdog_counter=-1;
int memcard_write=0;

void   initialize_memmap(void);
unsigned int m68k_read_memory_dummy(unsigned int address);
unsigned int m68k_read_memory_8(unsigned int address);
unsigned int m68k_read_memory_16(unsigned int address);
unsigned int m68k_read_memory_32(unsigned int address);
void m68k_write_memory_dummy(unsigned int address, unsigned int value);
void m68k_write_memory_8(unsigned int address, unsigned int value);
void m68k_write_memory_8_320000(unsigned int address, unsigned int value);
void m68k_write_memory_8_3c0000(unsigned int address, unsigned int value);
void m68k_write_memory_16(unsigned int address, unsigned int value);
void m68k_write_memory_16_320000(unsigned int address, unsigned int value);
void m68k_write_memory_16_400000(unsigned int address, unsigned int value);
void m68k_write_memory_32(unsigned int address, unsigned int value);
unsigned int m68k_read_memorycard_8(unsigned int offset);
unsigned int m68k_read_memorycard_16(unsigned int offset);


/***Helper Functions********************************************************************************/
static int    read_upload(int);
static void   write_upload(int, int);
static void   write_upload_word(int, int);
static void   write_upload_dword(int, int);

static int    cpu_readvidreg(int);
static void   cpu_writevidreg(int, int);
static void   cpu_writeswitch(int, int);
static int    cpu_readcoin(int);

static void   write_memcard(int,int);

void cdda_control(void);

static void   watchdog_reset_w(void);

static struct M68K_PROGRAM neogeo_program[]= {
	{0x000000, 0x1FFFFF, (unsigned)&neogeo_prg_memory},		// 0
	{0xc00000, 0xc7FFFF, (((unsigned)&neogeo_rom_memory)-0xc00000)},	// 1
	{(unsigned)-1,(unsigned)-1,(unsigned)NULL}
};


unsigned int m68k_read_memory_dummy(unsigned int address){
#ifdef DEBUG_MEMORY
if (trazando) printf("m68k_read_memory_dummy(0x%X)\n",address);
#endif
	return 0;
}

void m68k_write_memory_dummy(unsigned int address, unsigned int value){
#ifdef DEBUG_MEMORY
if (trazando) printf("m68k_write_memory_dummy(0x%X,0x%X)\n",address,value);
#endif
}

#if !defined(DEBUG_MEMORY) && !defined(SINGLE_MEMORY)
static struct M68K_DATA neogeo_memmap_read_16[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			//0
	{0x200000, 0x3bFFFF, (void *)m68k_read_memory_dummy, NULL},		//1
	{0x3c0000, 0x3cFFFF, (void *)cpu_readvidreg, NULL},			//2
	{0x3d0000, 0x3FFFFF, (void *)m68k_read_memory_dummy, NULL},		//3
	{0x400000, 0x401FFF, NULL, (void *)(((unsigned)&video_paletteram_ng)-0x400000)},	//4
	{0x402000, 0x7FFFFF, (void *)m68k_read_memory_dummy, NULL},		//5
	{0x800000, 0x803FFF, (void *)m68k_read_memorycard_16, NULL},		//6
	{0x804000, 0xbFFFFF, (void *)m68k_read_memory_dummy, NULL},		//7
	{0xc00000, 0xc7FFFF, NULL, (void *)(((unsigned)&neogeo_rom_memory)-0xc00000)},	//8
	{0xc80000, 0xFFFFFF, (void *)m68k_read_memory_dummy, NULL},		//9
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#ifdef SINGLE_MEMORY
static struct M68K_DATA neogeo_memmap_read_16[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			//0
	{0x200000, 0xbFFFFF, (void *)m68k_read_memory_16, NULL},		//1
	{0xc00000, 0xc7FFFF, NULL, (void *)(((unsigned)&neogeo_rom_memory)-0xc00000)},  //2
	{0xc80000, 0xFFFFFF, (void *)m68k_read_memory_16, NULL},		//3
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#warning neogeo_memmap_read_16 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_read_16[] = {
	{0x000000, 0xFFFFFF, (void *)m68k_read_memory_16, NULL},	// 0
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#endif
#endif

#if !defined(DEBUG_MEMORY) && !defined(SINGLE_MEMORY)
static struct M68K_DATA neogeo_memmap_read_8[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			// 0
	{0x200000, 0x2FFFFF, (void *)m68k_read_memory_dummy, NULL},		// 1
	{0x300000, 0x30FFFF, (void *)read_player1, NULL},			// 2
	{0x310000, 0x31FFFF, (void *)m68k_read_memory_dummy, NULL},		// 3
	{0x320000, 0x32FFFF, (void *)cpu_readcoin, NULL},			// 4
	{0x330000, 0x33FFFF, (void *)m68k_read_memory_dummy, NULL},		// 5
	{0x340000, 0x34FFFF, (void *)read_player2, NULL},			// 6
	{0x350000, 0x37FFFF, (void *)m68k_read_memory_dummy, NULL},		// 7
	{0x380000, 0x38FFFF, (void *)read_pl12_startsel, NULL},			// 8
	{0x390000, 0x39FFFF, (void *)m68k_read_memory_dummy, NULL},		// 9
	{0x400000, 0x401FFF, NULL, (void *)(((unsigned)&video_paletteram_ng)-0x400000)},	// 10
	{0x402000, 0x7FFFFF, (void *)m68k_read_memory_dummy, NULL},		// 11
	{0x800000, 0x803FFF, (void *)m68k_read_memorycard_8, NULL},		// 12
	{0x804000, 0xbFFFFF, (void *)m68k_read_memory_dummy, NULL},		// 13
	{0xc00000, 0xc7FFFF, NULL, (void *)(((unsigned)&neogeo_rom_memory)-0xc00000)},	// 14
	{0xc80000, 0xdFFFFF, (void *)m68k_read_memory_dummy, NULL},		// 15
	{0xe00000, 0xe3FFFF, (void *)read_upload, NULL},			// 16
	{0xe40000, 0xFFFFFF, (void *)m68k_read_memory_dummy, NULL},		// 17
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#ifdef SINGLE_MEMORY
static struct M68K_DATA neogeo_memmap_read_8[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			//0
	{0x200000, 0xbFFFFF, (void *)m68k_read_memory_8, NULL},		//1
	{0xc00000, 0xc7FFFF, NULL, (void *)(((unsigned)&neogeo_rom_memory)-0xc00000)},  //2
	{0xc80000, 0xFFFFFF, (void *)m68k_read_memory_8, NULL},		//3
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#warning neogeo_memmap_read_8 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_read_8[] = {
	{0x000000, 0xFFFFFF, (void *)m68k_read_memory_8, NULL},	// 0
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#endif
#endif


#if !defined(DEBUG_MEMORY) && !defined(SINGLE_MEMORY)
static struct M68K_DATA neogeo_memmap_write_16[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},		// 0
	{0x200000, 0x31FFFF, (void *)m68k_write_memory_dummy, NULL},	// 1
	{0x320000, 0x32FFFF, (void *)m68k_write_memory_16_320000, NULL},// 2
	{0x330000, 0x39FFFF, (void *)m68k_write_memory_dummy, NULL},	// 3
	{0x3a0000, 0x3aFFFF, (void *)cpu_writeswitch, NULL},		// 4
	{0x3b0000, 0x3bFFFF, (void *)m68k_write_memory_dummy, NULL},	// 5
	{0x3c0000, 0x3cFFFF, (void *)cpu_writevidreg, NULL},		// 6
	{0x3d0000, 0x3fFFFF, (void *)m68k_write_memory_dummy, NULL},	// 7
	{0x400000, 0x40FFFF, (void *)m68k_write_memory_16_400000, NULL},// 8
	{0x410000, 0x7fFFFF, (void *)m68k_write_memory_dummy, NULL},	// 9
	{0x800000, 0x80FFFF, (void *)write_memcard, NULL},		// 10
	{0x810000, 0xdfFFFF, (void *)m68k_write_memory_dummy, NULL},	// 11
	{0xe00000, 0xe3FFFF, (void *)write_upload_word, NULL},		// 12
	{0xe40000, 0xFFFFFF, (void *)m68k_write_memory_dummy, NULL},	// 13
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#ifdef SINGLE_MEMORY
static struct M68K_DATA neogeo_memmap_write_16[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			//0
	{0x200000, 0xFFFFFF, (void *)m68k_write_memory_16, NULL},		//1
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#warning neogeo_memmap_write_16 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_write_16[] = {
	{0x000000, 0xFFFFFF, (void *)m68k_write_memory_16, NULL},	// 0
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#endif
#endif


#if !defined(DEBUG_MEMORY) && !defined(SINGLE_MEMORY)
static struct M68K_DATA neogeo_memmap_write_8[] = {
	{0x000000, 0x1FFFFF, NULL, &neogeo_prg_memory},		// 0
	{0x200000, 0x2FFFFF, (void *)m68k_write_memory_dummy, NULL},	// 1
	{0x300000, 0x30FFFF, (void *)watchdog_reset_w, NULL},		// 2
	{0x310000, 0x31FFFF, (void *)m68k_write_memory_dummy, NULL},	// 3
	{0x320000, 0x32FFFF, (void *)m68k_write_memory_8_320000, NULL},	// 4
	{0x330000, 0x39FFFF, (void *)m68k_write_memory_dummy, NULL},	// 5
	{0x3a0000, 0x3aFFFF, (void *)cpu_writeswitch, NULL},		// 6
	{0x3b0000, 0x3fFFFF, (void *)m68k_write_memory_dummy, NULL},	// 7
	{0x3c0000, 0x3cFFFF, (void *)m68k_write_memory_8_3c0000, NULL},	// 8
	{0x3d0000, 0x7fFFFF, (void *)m68k_write_memory_dummy, NULL},	// 9
	{0x800000, 0x80FFFF, (void *)write_memcard, NULL},		// 10
	{0x810000, 0xdfFFFF, (void *)m68k_write_memory_dummy, NULL},	// 11
	{0xe00000, 0xe3FFFF, (void *)write_upload, NULL},		// 12
	{0xe40000, 0xFFFFFF, (void *)m68k_write_memory_dummy, NULL},	// 13
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#ifdef SINGLE_MEMORY
static struct M68K_DATA neogeo_memmap_write_8[] = {
	{0x000000, 0x1FFFFF, NULL, (void *)&neogeo_prg_memory},			//0
	{0x200000, 0xFFFFFF, (void *)m68k_write_memory_8, NULL},		//1
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#else

#warning neogeo_memmap_write_8 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_write_8[] = {
	{0x000000, 0xFFFFFF, (void *)m68k_write_memory_8, NULL},	// 0
	{(unsigned)-1,(unsigned)-1,NULL,NULL}
};
#endif
#endif

static struct M68K_CONTEXT neogeo_context;
static unsigned neogeo_fpa[256];

void    neogeo_cdrom_load_files(void);
void    neogeo_cdda_control(void);
void    neogeo_prio_switch(void);
void    neogeo_upload(void);

extern int img_display;

void neogeo_chk_handler_10(unsigned vector)
{
	neo4all_prof_start(NEO4ALL_PROFILER_VECTOR);
#ifdef DEBUG_MEMORY
puts("CAPTURADO !!!!!!!!");
printf("\tPC=%X  FPC=%X\n", m68k_get_pc(), m68k_fetch(m68k_get_pc(),0));
printf("\tVECTOR=%X\n",vector);
fflush(stdout);
#endif

	switch(m68k_fetch(m68k_get_pc(),0))
	{
		case 0xfabe: neogeo_exit(); break;
		case 0xfabf: img_display=1; neogeo_cdrom_load_files(); break;
		case 0xfac0: img_display=0; neogeo_cdrom_load_files(); break;
		case 0xfac1: neogeo_upload(); break;
		case 0xfac2: neogeo_prio_switch(); break;
		case 0xfac3: neogeo_cdda_control(); break;
	}
	m68k_set_register(M68K_REG_PC,m68k_get_pc()+2);
	neo4all_prof_end(NEO4ALL_PROFILER_VECTOR);
}

/***************************************************************************************************/
void initialize_memmap(void) {
	console_puts("INIT MEMMAP");
	memset(&neogeo_context,0,sizeof(struct M68K_CONTEXT));
	memset(&neogeo_fpa,0,sizeof(unsigned)*256);
	neogeo_fpa[0x10]=(unsigned)&neogeo_chk_handler_10;
	neogeo_fpa[0x2c]=(unsigned)&neogeo_chk_handler_10;
	neogeo_context.icust_handler = (unsigned int *)&neogeo_fpa;

	neogeo_context.fetch=(M68K_PROGRAM*)&neogeo_program;
	neogeo_context.read_byte=(M68K_DATA*)&neogeo_memmap_read_8;
	neogeo_context.read_word=(M68K_DATA*)&neogeo_memmap_read_16;
	neogeo_context.write_byte=(M68K_DATA*)&neogeo_memmap_write_8;
	neogeo_context.write_word=(M68K_DATA*)&neogeo_memmap_write_16;

	neogeo_context.sv_fetch=(M68K_PROGRAM*)&neogeo_program;
	neogeo_context.sv_read_byte=(M68K_DATA*)&neogeo_memmap_read_8;
	neogeo_context.sv_read_word=(M68K_DATA*)&neogeo_memmap_read_16;
	neogeo_context.sv_write_byte=(M68K_DATA*)&neogeo_memmap_write_8;
	neogeo_context.sv_write_word=(M68K_DATA*)&neogeo_memmap_write_16;

	neogeo_context.user_fetch=(M68K_PROGRAM*)&neogeo_program;
	neogeo_context.user_read_byte=(M68K_DATA*)&neogeo_memmap_read_8;
	neogeo_context.user_read_word=(M68K_DATA*)&neogeo_memmap_read_16;
	neogeo_context.user_write_byte=(M68K_DATA*)&neogeo_memmap_write_8;
	neogeo_context.user_write_word=(M68K_DATA*)&neogeo_memmap_write_16;

	neogeo_context.reset_handler=NULL;
	neogeo_context.iack_handler=NULL;

	neogeo_program[0].offset=(unsigned int)neogeo_prg_memory;
	neogeo_program[1].offset=((unsigned)neogeo_rom_memory)-0xc00000;

#if !defined(DEBUG_MEMORY) && !defined(SINGLE_MEMORY)
	neogeo_memmap_read_16[0].data=neogeo_prg_memory;
	neogeo_memmap_read_16[4].data=(void *)(((unsigned)video_paletteram_ng)-0x400000);
	neogeo_memmap_read_16[8].data=(void *)(((unsigned)neogeo_rom_memory)-0xc00000);

	neogeo_memmap_read_8[0].data=neogeo_prg_memory;
	neogeo_memmap_read_8[10].data=(void *)(((unsigned)video_paletteram_ng)-0x400000);
	neogeo_memmap_read_8[14].data=(void *)(((unsigned)neogeo_rom_memory)-0xc00000);

	neogeo_memmap_write_16[0].data=neogeo_prg_memory;
	neogeo_memmap_write_8[0].data=neogeo_prg_memory;
#else
#ifdef SINGLE_MEMORY
	neogeo_memmap_read_16[0].data=neogeo_prg_memory;
	neogeo_memmap_read_16[2].data=(void *)(((unsigned)neogeo_rom_memory)-0xc00000);
	neogeo_memmap_read_8[0].data=neogeo_prg_memory;
	neogeo_memmap_read_8[2].data=(void *)(((unsigned)neogeo_rom_memory)-0xc00000);
	neogeo_memmap_write_16[0].data=neogeo_prg_memory;
	neogeo_memmap_write_8[0].data=neogeo_prg_memory;
#endif
#endif

	m68k_set_context(&neogeo_context);
//	m68k_set_irq_type(NULL,M68K_AUTO_LOWER_IRQ);
}



/***************************************************************************************************/
#ifdef DEBUG_MEMORY
unsigned int  _m68k_read_memory_8(unsigned int offset) {
#else
unsigned int m68k_read_memory_8(unsigned int offset) {
#endif
#ifdef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    register unsigned char ret=0;
    offset&=0xffffff;
    if(offset<0x200000)
        ret=*((Uint8*)&neogeo_prg_memory[offset^1]);
    else
    switch(offset>>16)
    {
        case    0x30:    ret=read_player1(); break;
        case    0x32:    ret=cpu_readcoin(offset); break;
        case    0x34:    ret=read_player2(); break;
        case    0x38:    ret=read_pl12_startsel(); break;
        case    0x80:    if(offset&0x01) ret=neogeo_memorycard[(offset&0x3fff)>>1];
                         else ret=(unsigned char)-1; break;

        /* BIOS ROM */
        case    0xc0:
        case    0xc1:
        case    0xc2:
        case    0xc3:
        case    0xc4:
        case    0xc5:
        case    0xc6:
        case    0xc7:    ret=neogeo_rom_memory[(offset^1)&0x0fffff];  break;

        /* upload region */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    ret=read_upload(offset&0xfffff); break;

    }
#ifdef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
    return (unsigned int)ret;
}

#ifdef DEBUG_MEMORY
unsigned int  m68k_read_memory_8(unsigned int offset) {
unsigned int result=_m68k_read_memory_8(offset);
if (trazando) printf("read_8(0x%X)=0x%X\n",offset,result&0xFF);
return result;
}
#endif


/***************************************************************************************************/

#ifdef DEBUG_MEMORY
unsigned int  _m68k_read_memory_16(unsigned int offset) {
#else
unsigned int  m68k_read_memory_16(unsigned int offset) {
#endif
#ifdef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    register unsigned short ret=0;
    offset&=0xffffff;
    if(offset<0x200000)
        ret=*((Uint16*)&neogeo_prg_memory[offset]);
    else
    switch(offset>>16)
    {
        case    0x3c:    ret=cpu_readvidreg(offset); break;
        case    0x40:    ret=video_paletteram_ng[offset&0x1fff]; break;
        case    0x80:    ret=0xff00|neogeo_memorycard[(offset&0x3fff)>>1]; break;

        /* BIOS ROM */
        case    0xc0:
        case    0xc1:
        case    0xc2:
        case    0xc3:
        case    0xc4:
        case    0xc5:
        case    0xc6:
        case    0xc7:    ret=*(Uint16*)&neogeo_rom_memory[offset&0x0fffff]; break;
		
//	case	0xff:	 break;

    }
#ifdef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
    return (unsigned int)ret;
}


#ifdef DEBUG_MEMORY
unsigned int  m68k_read_memory_16(unsigned int offset) {
unsigned int result=_m68k_read_memory_16(offset);
if (trazando) printf("read_16(0x%X)=0x%X\n",offset,result&0xFFFF);
return result;
}
#endif

/***************************************************************************************************/
#ifdef DEBUG_MEMORY
unsigned int _m68k_read_memory_32(unsigned int offset) {
#else
unsigned int  m68k_read_memory_32(unsigned int offset) {
#endif
    unsigned int data;
    offset&=0xfffffe;
    data=m68k_read_memory_16(offset)<<16;
    data|=m68k_read_memory_16(offset+2);
    return data;
}

#ifdef DEBUG_MEMORY
unsigned int  m68k_read_memory_32(unsigned int offset) {
unsigned int result=_m68k_read_memory_32(offset);
if (trazando) printf("read_32(0x%X)=0x%X\n",offset,result);
return result;
}
#endif

unsigned int m68k_read_memorycard_8(unsigned int offset) {
#ifdef DEBUG_MEMORY
if (trazando) printf("read_memorycard_8(0x%X)\n",offset);
#endif
	if(offset&0x01) return neogeo_memorycard[(offset&0x3fff)>>1];
	return (unsigned)-1;
}

unsigned int m68k_read_memorycard_16(unsigned int offset) {
#ifdef DEBUG_MEMORY
if (trazando) printf("read_memorycard_16(0x%X)\n",offset);
#endif
	return 0xff00|neogeo_memorycard[(offset&0x3fff)>>1];
}


void m68k_write_memory_8_320000(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_8_320000(0x%X)=0x%X\n",offset,data&0xFF);
#endif
#ifdef Z80_EMULATED
    neo4all_prof_start(NEO4ALL_PROFILER_Z80);
    	   if (neogeo_sound_enable)
	   {
                sound_code=data&0xff;
                pending_command=1;
                _z80nmi();
                _z80exec(Z80_TIMESLICE_PER_NMI);
                my_timer();
           }
    neo4all_prof_end(NEO4ALL_PROFILER_Z80);
#endif
}

void m68k_write_memory_16_320000(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_16_320000(0x%X)=0x%X\n",offset,data&0xFFFF);
#endif
#ifdef Z80_EMULATED
    neo4all_prof_start(NEO4ALL_PROFILER_Z80);
    	   if (neogeo_sound_enable)
	   {
                sound_code=(data>>8)&0xff;
                pending_command=1;
                _z80nmi();
                _z80exec(Z80_TIMESLICE_PER_NMI);
                my_timer();
           }
    neo4all_prof_end(NEO4ALL_PROFILER_Z80);
#endif
}

void m68k_write_memory_8_3c0000(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_8_3c0000(0x%X)=0x%X\n",offset,data&0xFF);
#endif
	    int temp=cpu_readvidreg(offset);
            if(offset&0x01) cpu_writevidreg(offset, (temp&0xff)|(data<<8));
            else cpu_writevidreg(offset, (temp&0xff00)|data);
}

/***************************************************************************************************/
void m68k_write_memory_8(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_8(0x%X)=0x%X\n",offset,data&0xFF);
#endif
#ifdef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    data&=0xff;
    offset&=0xffffff;

    if(offset<0x200000)
        neogeo_prg_memory[offset^1]=(char)data;
    else
    switch(offset>>16)
    {

        case    0x30:    watchdog_reset_w(); break;
        case    0x32:	 m68k_write_memory_8_320000(offset,data); break;
//	case	0x38:	 break;
        case    0x3a:    cpu_writeswitch(offset, data); break;
        case    0x3c:    m68k_write_memory_8_3c0000(offset,data); break;
        case    0x80:    if(offset&0x01) write_memcard(offset,data); break;


        /* upload */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:   write_upload(offset&0xfffff,data); break;
		
		/* cdrom */
//	case	0xff:	break;
    }
#ifdef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}

/***************************************************************************************************/

void m68k_write_memory_16_400000(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_16_400000(0x%X)=0x%X\n",offset,data&0xFFFF);
#endif
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
	offset =(offset&0x1fff)>>1;
        data  &=0x7fff;
#ifdef ONLY_CHANGES_PATCH
	if (video_paletteram_ng[offset]!=data)
#endif
	{
        	video_paletteram_ng[offset]=(Uint16)data;
        	video_paletteram_pc[offset]=video_color_lut[data];
		video_palette_dirty=-1;
		video_palette_use[video_palette_selected+(offset>>4)]=-1;
	}
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}

void m68k_write_memory_16(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_16(0x%X)=0x%X\n",offset,data&0xFFFF);
#endif
#ifdef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    data&=0xffff;
	offset&=0xffffff;

    if(offset<0x200000)
        *(Uint16*)&neogeo_prg_memory[offset]=(Uint16)data;
    else
    switch(offset>>16)
    {
//	case	0x2f:	break;
        case    0x3a:   cpu_writeswitch(offset, data); break;
        case    0x3c:   cpu_writevidreg(offset, data); break;
        case    0x40:   m68k_write_memory_16_400000(offset,data); break;
        case    0x80:   write_memcard(offset,data); break;

        /* upload */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    write_upload_word(offset&0xfffff,data); break;
		
//	case	0xff: break;
    }
#ifdef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}


/***************************************************************************************************/


#if 1
void m68k_write_memory_32(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_32(0x%X)=0x%X\n",offset,data);
#endif
	unsigned int word1=(data>>16)&0xffff;
	unsigned int word2=data&0xffff;
	m68k_write_memory_16(offset,word1);
	m68k_write_memory_16(offset+2,word2);
}

#else
void m68k_write_memory_32(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_32(0x%X)=0x%X\n",offset,data);
#endif
    int temp=((data&0xffff)<<16)|((data>>16)&0xffff);
    offset&=0xfffffe;

    if(offset<0x200000)
        *(Uint32*)&neogeo_prg_memory[offset]=temp;
    else
    switch(offset>>16)
    {
        case    0x3a:    cpu_writeswitch(offset, temp); break;
        case    0x3c:    cpu_writevidreg(offset, temp);
                         cpu_writevidreg(offset+2, data); break;
        case    0x40:    offset =(offset&0x1fff)>>1;
                         data  &=0x7fff;
                         temp  &=0x7fff;
#ifdef ONLY_CHANGES_PATCH
			 if ((video_paletteram_ng[offset]==(Uint16)temp)&&(video_paletteram_ng[offset+1]==(Uint16)data))
				 break;
#endif
                         video_paletteram_ng[offset]  =(Uint16)temp;
                         video_paletteram_pc[offset]  =video_color_lut[temp];
			 video_palette_dirty=-1;
			 video_palette_use[video_palette_selected+(offset>>4)]=-1;
                         video_paletteram_ng[offset+1]=(Uint16)data;
                         video_paletteram_pc[offset+1]=video_color_lut[data]; break;

        case    0x80:    offset =(offset&0x3fff)>>1;
                         neogeo_memorycard[offset]  =(char)temp;
                         neogeo_memorycard[offset+1]=(char)data; break;

        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    write_upload_dword(offset&0xfffff,temp); break;
		
//	case	0xff:	break;

    }
}
#endif

/***************************************************************************************************/
static int    cpu_readvidreg(int offset)
{
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    register int ret=0;
    switch(offset)
    {
        case    0x3c0000:    ret=*(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c0002:    ret=*(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c0004:    ret=video_modulo; break;
        case    0x3c0006:    ret=((((neogeo_frame_counter_speed-1)&0xffff)<<8)|
                               (neogeo_frame_counter&7)|128); break;
        case    0x3c0008:    ret=*(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c000a:    ret=*(Uint16*)&video_vidram[video_pointer<<1]; break;

    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
    return ret;
}



/***************************************************************************************************/
static void    cpu_writevidreg(int offset, int data)
{
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    switch(offset)
    {
        case    0x3c0000:    video_pointer=(Uint16)data; break;
        case    0x3c0002:    *(Uint16*)&video_vidram[video_pointer<<1]=(Uint16)data;
                             video_pointer+=video_modulo; break;
        case    0x3c0004:    video_modulo=(Sint16)data; break;

        case    0x3c0006:    neogeo_frame_counter_speed=((data>>8)&0xff)+1; break;

        case    0x3c0008:    /* IRQ position */    break;
        case    0x3c000a:    /* IRQ position */    break;
        case    0x3c000c:    /* IRQ acknowledge */ break;

    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}


/***************************************************************************************************/

static void     neogeo_setpalbank0 (void) {
    video_palette_selected=0;
    video_paletteram_ng=video_palette_bank0_ng;
    video_paletteram_pc=video_palette_bank0_pc;
}


static void     neogeo_setpalbank1 (void) {
    video_palette_selected=0x100;
    video_paletteram_ng=video_palette_bank1_ng;
    video_paletteram_pc=video_palette_bank1_pc;
}


static void    neogeo_select_bios_vectors (void) {
    memcpy(neogeo_prg_memory, neogeo_rom_memory, 0x80);
}

/*
static void    neogeo_select_game_vectors (void) {
}
*/


static void    cpu_writeswitch(int offset, int data)
{
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    switch(offset)
    {
/*
        case 0x3a0000:
        case 0x3a0001:  break;
*/

        case 0x3a0002:
        case 0x3a0003: neogeo_select_bios_vectors(); break;

        case 0x3a000e: 
        case 0x3a000f: neogeo_setpalbank1(); break;

/*
        case 0x3a0010:
        case 0x3a0011:  break;
*/

/*
        case 0x3a0012:
        case 0x3a0013: neogeo_select_game_vectors(); break;
*/

        case 0x3a001e: 
        case 0x3a001f: neogeo_setpalbank0(); break;

    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}

/***************************************************************************************************/


void neogeo_sound_irq(int irq)
{
#ifdef Z80_EMULATED
   if (neogeo_sound_enable)
   {
    if (irq) 
        _z80raise(0);
     else 
	_z80lower();
   }
#endif
}



static int cpu_readcoin(int addr)
{
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    register int res=0;
    addr &= 0xFFFF;
    if (addr == 0x1) {
        register int coinflip = pd4990a_testbit_r();
        register int databit = pd4990a_databit_r();
        res=0xff ^ (coinflip << 6) ^ (databit << 7);
    }
    if (addr == 0x0) {
#ifdef Z80_EMULATED
	if (neogeo_sound_enable)
	{
		res |= result_code;
		if (pending_command)
			res &= 0x7f;
	}
	else
		res |= 0x01;
#endif
    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
    return res;
}


static void watchdog_reset_w (void)
{
    watchdog_counter=3 * 60;  /* 3s * 60 fps */
}


static int read_upload(int offset) {
    register int ret=-1;
#if 0
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    /* int bank = m68k_read_memory_8(0x10FEDB); */

    /* read_upload is disabled for now.*/
	/* fixes Metal Slug */

    switch (m68k_read_memory_8(0x10FEDA)) {
        case 0x00: /* 68000 */
            return neogeo_prg_memory[offset^1];
        case 0x01: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	return mame_z80mem[(offset>>1)&0xFFFF];
#endif
	    break;
    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
#endif
    return ret;
}


static void write_upload(int offset, int data) {
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    switch (m68k_read_memory_8(0x10FEDA)) {
        case 0x00: /* 68000 */
            neogeo_prg_memory[offset^1]=(char)data;
            break;
        case 0x01: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	mame_z80mem[(offset>>1)&0xFFFF]=(char)data;
#endif
            break;
        case 0x11: /* FIX */
            neogeo_fix_memory[offset>>1]=(char)data;
            break;
    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}

static char _sprbuffer[4];

static void write_upload_word(int offset, int data) {
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    register int bank = m68k_read_memory_8(0x10FEDB);
    data&=0xffff;

    switch (m68k_read_memory_8(0x10FEDA)) {
        case 0x12: /* SPR */
	    {
            	register int offset2=(offset & ~0x02)+(bank<<20);
            	if((offset2&0x7f)<64)
               		offset2=(offset2&0xfff80)+((offset2&0x7f)<<1)+4;
           	 else
               		offset2=(offset2&0xfff80)+((offset2&0x7f)<<1)-128;

           	register char* dest=&neogeo_spr_memory[offset2];

            	if (offset & 0x02) {
               		/* second word */
			   *(Uint16*)(&dest[2])=(Uint16)data;
			   /* reformat sprite data */
               		swab(dest, _sprbuffer, sizeof(_sprbuffer));
               		extract8(_sprbuffer, dest);
	        } else {
			   /* first word */
			   *(Uint16*)(&dest[0])=(Uint16)data;
            	}
	    }
            break;

        case 0x13: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	mame_z80mem[(offset>>1)&0xFFFF]=(char)data;
#endif
            break;
        case 0x14: /* PCM */
            neogeo_pcm_memory[(offset>>1)+(bank<<19)]=(char)data;
            break;
    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}


static void write_upload_dword(int offset, int data) {
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    switch (m68k_read_memory_8(0x10FEDA)) {
        case 0x12: /* SPR */
            offset+=((int)(m68k_read_memory_8(0x10FEDB))<<20);

            if((offset&0x7f)<64)
               offset=(offset&0xfff80)+((offset&0x7f)<<1)+4;
            else
               offset=(offset&0xfff80)+((offset&0x7f)<<1)-128;
	    {
            	register char *dest=&neogeo_spr_memory[offset];
            	swab((char*)&data, _sprbuffer, sizeof(_sprbuffer));
            	extract8(_sprbuffer, dest);
	    }
            break;
    }
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}


static void write_memcard(int offset, int data) {
#ifndef SINGLE_MEMORY
    neo4all_prof_start(NEO4ALL_PROFILER_MEM);
#endif
    neogeo_memorycard[(offset&0x3fff)>>1]=(char)(data&0xFF); 

    /* signal that memory card has been written */
    memcard_write=4; 
#ifndef SINGLE_MEMORY
    neo4all_prof_end(NEO4ALL_PROFILER_MEM);
#endif
}

