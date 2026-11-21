#ifndef	Z80INTRF_H
#define	Z80INTRF_H

void z80_init(void);


extern Uint32 sound_code;
extern Uint32 result_code;
extern Uint32 pending_command;
extern Uint32 z80_cycles;
extern Uint16 z80_bank[4];
extern Uint8 mame_z80mem[0x10000];
extern Uint8 subcpu_memspace[0x10000];


#ifdef USE_MAMEZ80

#include "z80/mamez80/z80.h"
#define _z80exec z80_execute
#define _z80nmi cpu_z80_nmi
#define _z80raise cpu_z80_raise_irq
#define _z80lower cpu_z80_lower_irq

#else

#ifdef USE_CZ80

#include "z80/cz80/cz80.h"
extern cz80_struc neo4all_cz80_struc;
#define _z80raise(VEC) Cz80_Set_IRQ(&neo4all_cz80_struc, VEC)
#define _z80lower() Cz80_Clear_IRQ(&neo4all_cz80_struc)
#define _z80exec(CIC) Cz80_Exec(&neo4all_cz80_struc,CIC)
#define _z80nmi() Cz80_Set_NMI(&neo4all_cz80_struc)
static __inline__ void _z80reset(void)
{
	memcpy(mame_z80mem,subcpu_memspace,0x10000);
	Cz80_Reset(&neo4all_cz80_struc);
}

void cz80_z80_writeport16(Uint16 PortNo, Uint8 data);
Uint8 cz80_z80_readport16(Uint16 PortNo);

#else

#include "z80/mz80/mz80.h"
#define _z80raise mz80int
#define _z80lower() 
#define _z80exec mz80exec
#define _z80nmi mz80nmi

#endif
#endif


#endif /* Z80INTRF_H */

