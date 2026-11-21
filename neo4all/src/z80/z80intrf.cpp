#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// #define ONLY_CHANGES_PATCH


#include	"../neo4all.h"
#include 	<stdio.h>
#include 	<stdlib.h>
#include	"z80intrf.h"
#include	"sound/2610intf.h"


Uint16 z80_bank[4];
static Uint8 *z80map1, *z80map2, *z80map3, *z80map4;

Uint8 mame_z80mem[0x10000];
#ifndef AES
Uint8 subcpu_memspace[0x10000];
#define memory_sm1 ((unsigned)subcpu_memspace)
#else
#include "aes/aes.h"
#define memory_sm1 ((unsigned)aes4all_memory_sm1)
#endif

Uint32 sound_code;
Uint32 result_code;
Uint32 pending_command;
Uint32 z80_cycles;

static void _fast_memcpy(void *OUT, const void *IN, size_t N)
{
	register unsigned *d=(unsigned *)OUT;
	register unsigned *s=(unsigned *)IN;
	register unsigned n=N;
	while(n--){
#ifdef DREAMCAST
		asm("pref @%0" : : "r" (s + 8));
#endif
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
		*d++=*s++;
	}
}

#define fast_memcpy(OUT, IN, N) _fast_memcpy(OUT,IN,N>>5)

#ifndef USE_MAMEZ80

#ifdef USE_CZ80

cz80_struc neo4all_cz80_struc;


static __inline__ unsigned char _cpu_readmem8(unsigned int address)
{
//printf("cpu_readmem8(%i), PC=0x%X\n",address,z80_get_reg(Z80_PC));
	return (mame_z80mem[address&0xFFFF]);
}

static unsigned char cpu_readmem8(unsigned int address)
{
	return _cpu_readmem8(address);
}

#if CZ80_USE_WORD_HANDLER
static unsigned short cpu_readmem16(unsigned int address)
{
	return _cpu_readmem8(address) | (_cpu_readmem8(address + 1) << 8);
}
#endif

static __inline__ void _cpu_writemem8(unsigned int address, unsigned int data)
{
//printf("cpu_writemem8(%i,%i), PC=0x%X\n",address,data,z80_get_reg(Z80_PC));
	mame_z80mem[address&0xFFFF]=data;
}

static void cpu_writemem8(unsigned int address, unsigned int data)
{
	_cpu_writemem8(address,data);
}

#if CZ80_USE_WORD_HANDLER
static void cpu_writemem16(unsigned int address, unsigned int data)
{
	_cpu_writemem8(address, data & 0xFF);
	_cpu_writemem8(address + 1, data >> 8);
}
#endif


#else

void	PortWrite(UINT16 PortNo, UINT8 data, struct z80PortWrite *ptr);
UINT16	PortRead(UINT16 PortNo, struct z80PortRead *ptr);

struct	z80PortRead ReadPorts[] =
{
	{0x0000,	0xffff,		PortRead,	NULL},
	{(UINT16)-1,	(UINT16)-1,	NULL,		NULL}
};

struct	z80PortWrite WritePorts[] =
{
	{0x0000,	0xffff,		PortWrite,	NULL},
	{(UINT16)-1,	(UINT16)-1,	NULL,		NULL}
};

struct MemoryReadByte	MemRead[] =
{
	{(UINT16)-1,	(UINT16)-1,	NULL,		NULL}
};

struct MemoryWriteByte	MemWrite[] =
{
	{(UINT16)-1,	(UINT16)-1,	NULL,		NULL}
};

CONTEXTMZ80	subcpu_context;

#endif
#endif

int cpu_z80_irq_callback(int a)
{
//printf("cpu_z80_irq_callback(%i), PC=0x%X\n",a,z80_get_reg(Z80_PC));
	neogeo_sound_irq(a);
	return 0;
}


void _z80_init(void)
{
#ifdef AES
	fast_memcpy(mame_z80mem, aes4all_memory_sm1, 0xf800);
#endif
	z80map1 = (Uint8 *)(memory_sm1 + 0x8000);
	z80map2 = (Uint8 *)(memory_sm1 + 0xc000);
	z80map3 = (Uint8 *)(memory_sm1 + 0xe000);
	z80map4 = (Uint8 *)(memory_sm1 + 0xf000);
	z80_bank[0]=0x8000;
	z80_bank[1]=0xc000;
	z80_bank[2]=0xe000;
	z80_bank[3]=0xf000;

#ifndef USE_MAMEZ80
#ifdef USE_CZ80
	Cz80_Init(&neo4all_cz80_struc);
	Cz80_Set_Fetch(&neo4all_cz80_struc,0x0000,0xFFFF,(Uint32)((void *)&mame_z80mem));
	Cz80_Set_ReadB(&neo4all_cz80_struc,(unsigned int (*)(unsigned int))&cpu_readmem8);
	Cz80_Set_WriteB(&neo4all_cz80_struc,&cpu_writemem8);
#if CZ80_USE_WORD_HANDLER
	Cz80_Set_ReadW(&neo4all_cz80_struc,&cpu_readmem16);
	Cz80_Set_WriteW(&neo4all_cz80_struc,&cpu_writemem16);
#endif
	Cz80_Set_INPort(&neo4all_cz80_struc,(CZ80_READ *)&cz80_z80_readport16);
	Cz80_Set_OUTPort(&neo4all_cz80_struc,(CZ80_WRITE *)&cz80_z80_writeport16);
	Cz80_Set_IRQ_Callback(&neo4all_cz80_struc,cpu_z80_irq_callback);
	Cz80_Reset(&neo4all_cz80_struc);
#ifndef AES
//	Cz80_Exec(&neo4all_cz80_struc,100000);
#endif
#else
	subcpu_context.z80Base = mame_z80mem;
	
	subcpu_context.z80IoRead  = ReadPorts;
	subcpu_context.z80IoWrite = WritePorts;
	
	subcpu_context.z80MemRead = MemRead;
	subcpu_context.z80MemWrite = MemWrite;
	
	mz80SetContext((void *)&subcpu_context);

	mz80reset();

	// Let Z80 do its initialization
#ifndef AES
	mz80exec(100000);
#endif
#endif
#else
	z80_init();
	z80_reset(NULL);
	z80_set_irq_callback(cpu_z80_irq_callback);
#ifndef AES
	z80_execute(100000);
#endif
#endif
//printf("_z80_init PC=0x%X\n",z80_get_reg(Z80_PC));
}

//---------------------------------------------------------------------------
#ifndef USE_MAMEZ80
#ifdef USE_CZ80
void cz80_z80_writeport16(Uint16 PortNo, Uint8 data)
#else
void PortWrite(UINT16 PortNo, UINT8	data, struct z80PortWrite *ptr)
#endif
#else
void mame_z80_writeport16(Uint16 PortNo, Uint8 data)
#endif
{
//printf("mame_z80_writeport16(%i,%i) PC=0x%X\n",PortNo,data,z80_get_reg(Z80_PC));
	switch( PortNo & 0xff)
	{
	case	0x4:
		YM2610_control_port_0_A_w(0,data);
		break;

	case	0x5:
		YM2610_data_port_0_A_w(0,data);
		break;

	case	0x6:
		YM2610_control_port_0_B_w(0,data);
		break;

	case	0x7:
		YM2610_data_port_0_B_w(0,data);
		break;
/*
	case	0x8:
		// NMI enable / acknowledge? (the data written doesn't matter) 
		break;
*/	
	case	0xc:
		result_code = data;
		break;
/*	
	case	0x18:
		// NMI disable? (the data written doesn't matter)
		break;
*/
/*
	default:
		//printf("Unimplemented Z80 Write Port: %x data: %x\n",PortNo&0xff,data);
		break;
*/
	}
}


//---------------------------------------------------------------------------
#ifndef USE_MAMEZ80
#ifdef USE_CZ80
Uint8 cz80_z80_readport16(Uint16 PortNo)
#else
UINT16 PortRead(UINT16 PortNo, struct z80PortRead *ptr)
#endif
#else
Uint8 mame_z80_readport16(Uint16 PortNo)
#endif
{
//printf("mame_z80_readport16(%i) PC=0x%X\n",PortNo,z80_get_reg(Z80_PC));
	switch( PortNo & 0xff)
	{
	case	0x0:
		pending_command = 0;
		return sound_code;
	
	case	0x4:
		return YM2610_status_port_0_A_r(0);
	
	case	0x5:
		return YM2610_read_port_0_r(0);
	
	case	0x6:
		return YM2610_status_port_0_B_r(0);

#ifdef AES
	case 0x08:
#ifdef ONLY_CHANGES_PATCH
		if (z80_bank[3]!=PortNo)
#endif
		{
			z80_bank[3]=PortNo;
			z80map4 = (Uint8 *)(memory_sm1 + (0x0800 * ((PortNo >> 8) & 0x7f)));
			fast_memcpy(mame_z80mem + 0xf000, z80map4, 0x0800);
		}
		break;
		
	case 0x09:
#ifdef ONLY_CHANGES_PATCH
		if (z80_bank[2]!=PortNo)
#endif
		{
			z80_bank[2]=PortNo;
			z80map3 = (Uint8 *)(memory_sm1 + (0x1000 * ((PortNo >> 8) & 0x3f)));
			fast_memcpy(mame_z80mem + 0xe000, z80map3, 0x1000);
		}
		break;
		
	case 0x0a:
#ifdef ONLY_CHANGES_PATCH
		if (z80_bank[1]!=PortNo)
#endif
		{
			z80_bank[1]=PortNo;
			z80map2 = (Uint8 *)(memory_sm1 + (0x2000 * ((PortNo >> 8) & 0x1f)));
			fast_memcpy(mame_z80mem + 0xc000, z80map2, 0x2000);
		}
		break;

	case 0x0b:
#ifdef ONLY_CHANGES_PATCH
		if (z80_bank[0]!=PortNo)
#endif
		{
			z80_bank[0]=PortNo;
			z80map1 = (Uint8 *)(memory_sm1 + (0x4000 * ((PortNo >> 8) & 0x0f)));
			fast_memcpy(mame_z80mem + 0x8000, z80map1, 0x4000);
		}
		break;
#endif

/*
	default:
		//printf("Unimplemented Z80 Read Port: %d\n",PortNo&0xff);
		break;
*/
	};	
	return 0;
}


#ifdef USE_MAMEZ80
void cpu_z80_nmi(void)
{
//printf("cpu_z80_nmi PC=0x%X\n",z80_get_reg(Z80_PC));
	z80_set_irq_line(IRQ_LINE_NMI, ASSERT_LINE);
	z80_set_irq_line(IRQ_LINE_NMI, CLEAR_LINE);
}

void cpu_z80_init(void)
{
//printf("cpu_z80_init PC=0x%X\n",z80_get_reg(Z80_PC));
	z80_set_irq_callback(cpu_z80_irq_callback);
}

void cpu_z80_raise_irq(int l)
{
//printf("cpu_z80_raise(%i) PC=0x%X\n",l,z80_get_reg(Z80_PC));
	z80_set_irq_line(l, ASSERT_LINE);
}
void cpu_z80_lower_irq(void)
{
//printf("cpu_z80_lower_irq PC=0x%X\n",z80_get_reg(Z80_PC));
	z80_set_irq_line(0, CLEAR_LINE);
}

void cpu_z80_reset(void)
{
	z80_set_irq_callback(cpu_z80_irq_callback);
	z80_reset(NULL);
	z80_set_irq_callback(cpu_z80_irq_callback);
#ifndef AES
	z80_execute(100000);
#endif
//printf("cpu_z80_reset PC=0x%X\n",z80_get_reg(Z80_PC));
}
#endif
