/********************************************
*   NeoCD Memory Mapping (C version)        *
*********************************************
* Fosters(2001,2003)                        *
********************************************/

#include <SDL.h>
#include "SDL_endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <fame/fame.h>

#ifdef DEBUG_MEMORY
extern int trazando;
#endif

#include "console.h"

#include "../neocd.h"

extern unsigned short    *video_paletteram_ng;

#if 0
    #define logaccess(...) if (trazando) printf(__VA_ARGS__)
#else
    #define logaccess(...)
#endif

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
void m68k_write_memory_16_400000(unsigned int address, unsigned int value);
void m68k_write_memory_32(unsigned int address, unsigned int value);


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
	{0x000000, 0x1FFFFF, &neogeo_prg_memory},		// 0
	{0xc00000, 0xc7FFFF, &neogeo_rom_memory-0xc00000},	// 1
	{-1,-1,NULL}
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

#ifndef DEBUG_MEMORY
static struct M68K_DATA neogeo_memmap_read_16[] = {
	{0x000000, 0x1FFFFF, NULL, &neogeo_prg_memory},		// 0
	{0x3c0000, 0x3cFFFF, cpu_readvidreg, NULL},		// 1
	{0x400000, 0x40FFFF, NULL, &video_paletteram_ng-0x400000},// 2
	{0x800000, 0x80FFFF, NULL, neogeo_memorycard-0x800000},	// 3
	{0xc00000, 0xc7FFFF, NULL, &neogeo_rom_memory-0xc00000},// 4
	{0xff0000, 0xFFFFFF, m68k_read_memory_dummy, NULL},	// 5
	{-1,-1,NULL,NULL}
};
#else

#warning neogeo_memmap_read_16 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_read_16[] = {
	{0x000000, 0xFFFFFF, m68k_read_memory_16, NULL},	// 0
	{-1,-1,NULL,NULL}
};
#endif

#ifndef DEBUG_MEMORY
static struct M68K_DATA neogeo_memmap_read_8[] = {
	{0x000000, 0x1FFFFF, NULL, &neogeo_prg_memory},		// 0
	{0x300000, 0x30FFFF, read_player1, NULL},		// 1
	{0x320000, 0x32FFFF, cpu_readcoin, NULL},		// 2
	{0x340000, 0x34FFFF, read_player2, NULL},		// 3
	{0x380000, 0x38FFFF, read_pl12_startsel, NULL},		// 4
	{0x400000, 0x40FFFF, NULL, &video_paletteram_ng-0x400000},// 5
	{0x800000, 0x80FFFF, NULL, neogeo_memorycard-0x800000},	// 6
	{0xc00000, 0xc7FFFF, NULL, &neogeo_rom_memory-0xc00000},// 7
	{0xe00000, 0xe3FFFF, read_upload, NULL},		// 8
	{0xff0000, 0xFFFFFF, m68k_read_memory_dummy, NULL},	// 9
	{-1,-1,NULL,NULL}
};
#else

#warning neogeo_memmap_read_8 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_read_8[] = {
	{0x000000, 0xFFFFFF, m68k_read_memory_8, NULL},	// 0
	{-1,-1,NULL,NULL}
};
#endif


#ifndef DEBUG_MEMORY
static struct M68K_DATA neogeo_memmap_write_16[] = {
	{0x000000, 0x1FFFFF, NULL, &neogeo_prg_memory},		// 0
	{0x2f0000, 0x2FFFFF, m68k_write_memory_dummy, NULL},	// 1
	{0x3a0000, 0x3aFFFF, cpu_writeswitch, NULL},		// 2
	{0x3c0000, 0x3cFFFF, cpu_writevidreg, NULL},		// 3
	{0x400000, 0x40FFFF, m68k_write_memory_16_400000, NULL},// 4
	{0x800000, 0x80FFFF, write_memcard, NULL},		// 5
	{0xe00000, 0xe3FFFF, write_upload_word, NULL},		// 6
	{0xff0000, 0xFFFFFF, m68k_write_memory_dummy, NULL},	// 7
	{-1,-1,NULL,NULL}
};
#else

#warning neogeo_memmap_write_16 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_write_16[] = {
	{0x000000, 0xFFFFFF, m68k_write_memory_16, NULL},	// 0
	{-1,-1,NULL,NULL}
};
#endif


#ifndef DEBUG_MEMORY
static struct M68K_DATA neogeo_memmap_write_8[] = {
	{0x000000, 0x1FFFFF, NULL, &neogeo_prg_memory},
	{0x300000, 0x30FFFF, watchdog_reset_w, NULL},
	{0x320000, 0x32FFFF, m68k_write_memory_8_320000, NULL},
	{0x3a0000, 0x3aFFFF, cpu_writeswitch, NULL},
	{0x3c0000, 0x3cFFFF, m68k_write_memory_8_3c0000, NULL},
	{0x380000, 0x38FFFF, m68k_write_memory_dummy, NULL},
	{0x800000, 0x80FFFF, write_memcard, NULL},
	{0xe00000, 0xe3FFFF, write_upload, NULL},
	{0xff0000, 0xFFFFFF, m68k_write_memory_dummy, NULL},
	{-1,-1,NULL,NULL}
};
#else


#warning neogeo_memmap_write_8 DEBUG MODE !!!!!!!
static struct M68K_DATA neogeo_memmap_write_8[] = {
	{0x000000, 0xFFFFFF, m68k_write_memory_8, NULL},	// 0
	{-1,-1,NULL,NULL}
};
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
}

/***************************************************************************************************/
void initialize_memmap(void) {
	console_puts("INIT MEMMAP");
	memset(&neogeo_context,0,sizeof(struct M68K_CONTEXT));
	memset(&neogeo_fpa,0,sizeof(unsigned)*256);
	neogeo_fpa[0x10]=(unsigned)&neogeo_chk_handler_10;
	neogeo_fpa[0x2c]=(unsigned)&neogeo_chk_handler_10;
	neogeo_context.icust_handler = (void *)&neogeo_fpa;

	neogeo_context.fetch=&neogeo_program;
	neogeo_context.read_byte=&neogeo_memmap_read_8;
	neogeo_context.read_word=&neogeo_memmap_read_16;
	neogeo_context.write_byte=&neogeo_memmap_write_8;
	neogeo_context.write_word=&neogeo_memmap_write_16;

	neogeo_context.sv_fetch=&neogeo_program;
	neogeo_context.sv_read_byte=&neogeo_memmap_read_8;
	neogeo_context.sv_read_word=&neogeo_memmap_read_16;
	neogeo_context.sv_write_byte=&neogeo_memmap_write_8;
	neogeo_context.sv_write_word=&neogeo_memmap_write_16;

	neogeo_context.user_fetch=&neogeo_program;
	neogeo_context.user_read_byte=&neogeo_memmap_read_8;
	neogeo_context.user_read_word=&neogeo_memmap_read_16;
	neogeo_context.user_write_byte=&neogeo_memmap_write_8;
	neogeo_context.user_write_word=&neogeo_memmap_write_16;

	neogeo_context.reset_handler=NULL;
	neogeo_context.iack_handler=NULL;

	neogeo_program[0].offset=neogeo_prg_memory;
	neogeo_program[1].offset=neogeo_rom_memory-0xc00000;

#ifndef DEBUG_MEMORY
	neogeo_memmap_read_16[0].data=neogeo_prg_memory;
	neogeo_memmap_read_16[2].data=video_paletteram_ng-0x400000;
	neogeo_memmap_read_16[4].data=neogeo_rom_memory-0xc00000;

	neogeo_memmap_read_8[0].data=neogeo_prg_memory;
	neogeo_memmap_read_8[5].data=video_paletteram_ng-0x400000;
	neogeo_memmap_read_8[7].data=neogeo_rom_memory-0xc00000;

	neogeo_memmap_write_16[0].data=neogeo_prg_memory;
	neogeo_memmap_write_8[0].data=neogeo_prg_memory;
#endif

#ifndef DREAMCAST
neogeo_program[1].offset+=0xc00000;
neogeo_memmap_read_16[2].data+=0x400000;
neogeo_memmap_read_16[3].data+=0x800000;
neogeo_memmap_read_16[4].data+=0xc00000;
neogeo_memmap_read_8[5].data+=0x400000;
neogeo_memmap_read_8[6].data+=0x800000;
neogeo_memmap_read_8[7].data+=0xc00000;
#endif


	m68k_set_context(&neogeo_context);
	m68k_set_irq_type(M68K_AUTO_LOWER_IRQ);
}



/***************************************************************************************************/
#ifdef DEBUG_MEMORY
unsigned int  _m68k_read_memory_8(unsigned int offset) {
#else
unsigned int  m68k_read_memory_8(unsigned int offset) {
#endif
    offset&=0xffffff;
    if(offset<0x200000)
        return neogeo_prg_memory[offset^1];

    switch(offset>>16)
    {
        case    0x30:    return read_player1();
        case    0x32:    return cpu_readcoin(offset);
        case    0x34:    return read_player2();
        case    0x38:    return read_pl12_startsel();
        case    0x80:    if(offset&0x01) return neogeo_memorycard[(offset&0x3fff)>>1];
                         else return -1;

        /* BIOS ROM */
        case    0xc0:
        case    0xc1:
        case    0xc2:
        case    0xc3:
        case    0xc4:
        case    0xc5:
        case    0xc6:
        case    0xc7:    return neogeo_rom_memory[(offset^1)&0x0fffff];

        /* upload region */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    return read_upload(offset&0xfffff);

        default:
            logaccess("m68k_read_memory_8(0x%x) PC=%x\n",offset,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
    return 0;
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
    offset&=0xffffff;
    if(offset<0x200000)
        return *((Uint16*)&neogeo_prg_memory[offset]);

    switch(offset>>16)
    {
        case    0x3c:    return cpu_readvidreg(offset);
        case    0x40:    return video_paletteram_ng[offset&0x1fff];
        case    0x80:    return 0xff00|neogeo_memorycard[(offset&0x3fff)>>1];

        /* BIOS ROM */
        case    0xc0:
        case    0xc1:
        case    0xc2:
        case    0xc3:
        case    0xc4:
        case    0xc5:
        case    0xc6:
        case    0xc7:    return *(Uint16*)&neogeo_rom_memory[offset&0x0fffff];
		
	case	0xff:	 break;


        default:
            logaccess("m68k_read_memory_16(0x%x) PC=%x\n",offset,m68k_get_reg(NULL,M68K_REG_PPC));
            break;
    }
    return 0;
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


void m68k_write_memory_8_320000(unsigned int offset, unsigned int data) {
#ifdef Z80_EMULATED
    	   if (neogeo_sound_enable)
	    if(!(offset&0xffff)) {
                sound_code=data&0xff;
                pending_command=1;
                _z80nmi();
                z80_cycles-=neo4all_z80_cycles >> 8;
                _z80exec(neo4all_z80_cycles >> 8);
                my_timer();
            }
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
    data&=0xff;
    offset&=0xffffff;

    if(offset<0x200000) {
        neogeo_prg_memory[offset^1]=(char)data;
        return;
    }

    switch(offset>>16)
    {
        case    0x30:    watchdog_reset_w(); break;
        case    0x32:	 m68k_write_memory_8_320000(offset,data); break;
	case	0x38:	 break;
        case    0x3a:    cpu_writeswitch(offset, data); break;
        case    0x3c:    m68k_write_memory_8_3c0000(offset,data); break;
        case    0x80:    if(offset&0x01) write_memcard(offset,data); break;


        /* upload */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:   write_upload(offset&0xfffff,data); break;
		
		/* cdrom */
	case	0xff:	break;

        default:
            logaccess("m68k_write_memory_8(0x%x,0x%x) PC=%x\n",offset,data,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
}

/***************************************************************************************************/

void m68k_write_memory_16_400000(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_16_400000(0x%X)=0x%X\n",offset,data&0xFFFF);
#endif
	offset =(offset&0x1fff)>>1;
        data  &=0x7fff;
        video_paletteram_ng[offset]=(Uint16)data;
        video_paletteram_pc[offset]=video_color_lut[data];
	video_palette_dirty=-1;
	video_palette_use[offset>>4]=-1;
}

void m68k_write_memory_16(unsigned int offset, unsigned int data) {
#ifdef DEBUG_MEMORY
if (trazando) printf("write_16(0x%X)=0x%X\n",offset,data&0xFFFF);
#endif
    data&=0xffff;
	offset&=0xffffff;

    if(offset<0x200000) {
        *(Uint16*)&neogeo_prg_memory[offset]=(Uint16)data;
        return;
    }

    switch(offset>>16)
    {
	case	0x2f:	break;
        case    0x3a:   cpu_writeswitch(offset, data); break;
        case    0x3c:   cpu_writevidreg(offset, data); break;
        case    0x40:   m68k_write_memory_16_400000(offset,data); break;
        case    0x80:   write_memcard(offset,data); break;

        /* upload */
        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    write_upload_word(offset&0xfffff,data); break;
		
	case	0xff: break;

        default:
            logaccess("m68k_write_memory_16(0x%x,0x%x) PC=%x\n",offset,data,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
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

    if(offset<0x200000) {
        *(Uint32*)&neogeo_prg_memory[offset]=temp;
        return;
    }

    switch(offset>>16)
    {
        case    0x3a:    cpu_writeswitch(offset, temp); break;
        case    0x3c:    cpu_writevidreg(offset, temp);
                         cpu_writevidreg(offset+2, data); break;
        case    0x40:    offset =(offset&0x1fff)>>1;
                         data  &=0x7fff;
                         temp  &=0x7fff;
                         video_paletteram_ng[offset]  =(Uint16)temp;
                         video_paletteram_pc[offset]  =video_color_lut[temp];
			 video_palette_dirty=-1;
			 video_palette_use[offset>>4]=-1;
                         video_paletteram_ng[offset+1]=(Uint16)data;
                         video_paletteram_pc[offset+1]=video_color_lut[data]; break;

        case    0x80:    offset =(offset&0x3fff)>>1;
                         neogeo_memorycard[offset]  =(char)temp;
                         neogeo_memorycard[offset+1]=(char)data; break;

        case    0xe0:
        case    0xe1:
        case    0xe2:
        case    0xe3:    write_upload_dword(offset&0xfffff,temp); break;
		
		case	0xff:	break;

        default:
            logaccess("m68k_write_memory_32(0x%x,0x%x) PC=%x\n",offset,data,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
}
#endif

/***************************************************************************************************/
static int    cpu_readvidreg(int offset)
{
    switch(offset)
    {
        case    0x3c0000:    return *(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c0002:    return *(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c0004:    return video_modulo; break;
        case    0x3c0006:    return ((((neogeo_frame_counter_speed-1)&0xffff)<<8)|
                               (neogeo_frame_counter&7)|128); break;
        case    0x3c0008:    return *(Uint16*)&video_vidram[video_pointer<<1]; break;
        case    0x3c000a:    return *(Uint16*)&video_vidram[video_pointer<<1]; break;

        default:
            logaccess("cpu_readvidreg(0x%x) PC=%x\n",offset,m68k_get_reg(NULL,M68K_REG_PC));
            return 0;
            break;
    }
}
/*
static int    cpu_readvidreg(int offset)
{
if (trazando) printf("CPU_READVIDREG(0x%X)",offset);
	unsigned int result=_cpu_readvidreg(offset);
if (trazando) printf("=0x%X\n",result);
	return result;
}
*/



/***************************************************************************************************/
static void    cpu_writevidreg(int offset, int data)
{
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

        default:
            logaccess("cpu_writevidreg(0x%x,0x%x) PC=%x\n",offset,data,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
}

/*
static void    cpu_writevidreg(int offset, int data)
{
if (trazando) printf("CPU_WRITEVIDREG(0x%X,0x%X)\n",offset,data);
	_cpu_writevidreg(offset,data);
}
*/

/***************************************************************************************************/

static void     neogeo_setpalbank0 (void) {
    video_paletteram_ng=video_palette_bank0_ng;
    video_paletteram_pc=video_palette_bank0_pc;
}


static void     neogeo_setpalbank1 (void) {
    video_paletteram_ng=video_palette_bank1_ng;
    video_paletteram_pc=video_palette_bank1_pc;
}


static void    neogeo_select_bios_vectors (void) {
    memcpy(neogeo_prg_memory, neogeo_rom_memory, 0x80);
}


static void    neogeo_select_game_vectors (void) {
    logaccess("write game vectors stub\n");
}


static void    cpu_writeswitch(int offset, int data)
{
    switch(offset)
    {
        case 0x3a0000: /* NOP */ break;
        case 0x3a0001: /* NOP */ break;

        case 0x3a0002: neogeo_select_bios_vectors(); break;
        case 0x3a0003: neogeo_select_bios_vectors(); break;

        case 0x3a000e: neogeo_setpalbank1(); break;
        case 0x3a000f: neogeo_setpalbank1(); break;

        case 0x3a0010: /* NOP */ break;
        case 0x3a0011: /* NOP */ break;

        case 0x3a0012: neogeo_select_game_vectors(); break;
        case 0x3a0013: neogeo_select_game_vectors(); break;

        case 0x3a001e: neogeo_setpalbank0(); break;
        case 0x3a001f: neogeo_setpalbank0(); break;

        default:
            logaccess("cpu_writeswitch(0x%x,0x%x) PC=%x\n",offset,data,m68k_get_reg(NULL,M68K_REG_PC));
            break;
    }
}

/***************************************************************************************************/


void neogeo_sound_irq(int irq)
{
#ifdef Z80_EMULATED
   if (neogeo_sound_enable)
   {
    if (irq) {
        //logaccess("neogeo_sound_irq %d\n",irq);
        _z80raise(0);
    } else {
	_z80lower();
        //_z80ClearPendingInterrupt();
        //logaccess("neogeo_sound_end %d\n",irq);
    }
   }
#endif
}



static int cpu_readcoin(int addr)
{
    addr &= 0xFFFF;
    if (addr == 0x1) {
        int coinflip = pd4990a_testbit_r();
        int databit = pd4990a_databit_r();
        return 0xff ^ (coinflip << 6) ^ (databit << 7);
    }
    if (addr == 0x0) {
        int res = 0;
#ifdef Z80_EMULATED
   	if (neogeo_sound_enable)
	{
        	res |= result_code;
        	if (!pending_command)
            		res &= 0x7f;
        	else
            		res |= 0x01;
        }
#endif
        return res;
    }
    return 0;
}


static void watchdog_reset_w (void)
{
    //if (watchdog_counter == -1) logaccess("Watchdog Armed!\n");
    watchdog_counter=3 * 60;  /* 3s * 60 fps */
}


static int read_upload(int offset) {
    return -1;
#if 0
    int zone = m68k_read_memory_8(0x10FEDA);
    /* int bank = m68k_read_memory_8(0x10FEDB); */

    /* read_upload is disabled for now.*/
	/* fixes Metal Slug */

    switch (zone) {
        case 0x00: /* 68000 */
            return neogeo_prg_memory[offset^1];
        case 0x01: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	return subcpu_memspace[(offset>>1)&0xFFFF];
#endif
	    return -1;
        default:
            logaccess ("read_upload unimplemented zone %x\n",zone);
            return -1;
    }
#endif
}


static void write_upload(int offset, int data) {
    int zone = m68k_read_memory_8(0x10FEDA);
    /* int bank = m68k_read_memory_8(0x10FEDB); */

    switch (zone) {
        case 0x00: /* 68000 */
            neogeo_prg_memory[offset^1]=(char)data;
            break;
        case 0x01: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	subcpu_memspace[(offset>>1)&0xFFFF]=(char)data;
#endif
            break;
        case 0x11: /* FIX */
            neogeo_fix_memory[offset>>1]=(char)data;
            break;
        default:
            logaccess ("write_upload unimplemented zone %x\n",zone); break;
    }
}


static void write_upload_word(int offset, int data) {
    int zone = m68k_read_memory_8(0x10FEDA);
    int bank = m68k_read_memory_8(0x10FEDB);
    int offset2;
    char *dest;
    char sprbuffer[4];
	
  	data&=0xffff;

    switch (zone) {
        case 0x12: /* SPR */

            offset2=offset & ~0x02;

            offset2+=(bank<<20);

            if((offset2&0x7f)<64)
               offset2=(offset2&0xfff80)+((offset2&0x7f)<<1)+4;
            else
               offset2=(offset2&0xfff80)+((offset2&0x7f)<<1)-128;

            dest=&neogeo_spr_memory[offset2];

            if (offset & 0x02) {
               /* second word */
			   *(Uint16*)(&dest[2])=(Uint16)data;
			   /* reformat sprite data */
               swab(dest, sprbuffer, sizeof(sprbuffer));
               extract8(sprbuffer, dest);
			} else {
			   /* first word */
			   *(Uint16*)(&dest[0])=(Uint16)data;
            }
            break;

        case 0x13: /* Z80 */
#ifdef Z80_EMULATED
   	    if (neogeo_sound_enable)
            	subcpu_memspace[(offset>>1)&0xFFFF]=(char)data;
#endif
            break;
        case 0x14: /* PCM */
            neogeo_pcm_memory[(offset>>1)+(bank<<19)]=(char)data;
            break;
        default:
            logaccess ("write_upload_word unimplemented zone %x\n",zone); break;
    }
}


static void write_upload_dword(int offset, int data) {
    int zone = m68k_read_memory_8(0x10FEDA);
    int bank = m68k_read_memory_8(0x10FEDB);
    char *dest;
    char sprbuffer[4];

    switch (zone) {
        case 0x12: /* SPR */
            offset+=(bank<<20);

            if((offset&0x7f)<64)
               offset=(offset&0xfff80)+((offset&0x7f)<<1)+4;
            else
               offset=(offset&0xfff80)+((offset&0x7f)<<1)-128;

            dest=&neogeo_spr_memory[offset];
            swab((char*)&data, sprbuffer, sizeof(sprbuffer));
            extract8(sprbuffer, dest);

            break;
        default:
            logaccess ("write_upload_dword unimplemented zone %x\n",zone); break;
    }
}


static void write_memcard(int offset, int data) {
    data&=0xff;
    neogeo_memorycard[(offset&0x3fff)>>1]=(char)data; 

    /* signal that memory card has been written */
    memcard_write=3; 
}

