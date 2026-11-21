/**
 * NeoCD/SDL main header file
 **
 * 2003 Fosters
 */
 
#ifndef NEOCD_H
#define NEOCD_H

#ifdef DREAMCAST
#include <kos.h>
#endif

#define REFRESHTIME (1000/60)+1


//#define NEO4ALL_Z80_UNDER_CYCLES	 66666
#define NEO4ALL_Z80_UNDER_CYCLES	 33333
//#define NEO4ALL_Z80_UNDER_CYCLES	 22222
#define NEO4ALL_68K_UNDER_CYCLES	166666

//#define NEO4ALL_Z80_NORMAL_CYCLES	100000
#define NEO4ALL_Z80_NORMAL_CYCLES	 66666
//#define NEO4ALL_Z80_NORMAL_CYCLES	 44444
#define NEO4ALL_68K_NORMAL_CYCLES	200000

//#define NEO4ALL_Z80_OVER_CYCLES	133333
#define NEO4ALL_Z80_OVER_CYCLES	 	 99999
//#define NEO4ALL_Z80_OVER_CYCLES	 88888
#define NEO4ALL_68K_OVER_CYCLES		233333


#define SAMPLE_RATE    22050

#define REGION_JAPAN  0
#define REGION_USA    1
#define REGION_EUROPE 2

#define REGION REGION_USA

#define NEO4ALL_MEMCARD_SIZE 8192

#include <SDL.h>

#include "cdaudio/cdaudio.h"
#include "cdrom/cdrom.h"
#include "68k.h"
#include "memory/memory.h"
#include "video/video.h"
#include "input/input.h"
#include "z80/z80intrf.h"
#include "sound/sound.h"
#include "sound/streams.h"
#include "sound/2610intf.h"
#include "sound/timer.h"
#include "pd4990a.h"

/*-- Version, date & time to display on startup ----------------------------*/
#define VERSION1 "NEO4ALL based on NeoCD/SDL 0.3.1"
#define VERSION2 "Compiled on: "__DATE__" "__TIME__

/*-- functions -------------------------------------------------------------*/


/*-- globals ---------------------------------------------------------------*/
extern char	global_error[80];

extern char	*neogeo_rom_memory;
extern char	*neogeo_prg_memory;
extern char	*neogeo_fix_memory;
extern char	*neogeo_spr_memory;
extern char	*neogeo_pcm_memory;

extern unsigned char neogeo_memorycard[];
void memcard_init(void);
void memcard_shutdown(void);
void memcard_update(void);

extern int      neogeo_ipl_done;
extern int	neogeo_sound_enable;
extern int	neogeo_frameskip;

extern Uint32 neo4all_z80_cycles;
extern Uint32 neo4all_68k_cycles;
extern unsigned z80_cycles_inited;

int	neogeo_hreset(void);

void neogeo_adjust_frameskip(int new_frameskip);
void neogeo_adjust_cycles(int new_68k, int new_z80);
void print_frameskip(void);
void show_icon(void);

//void    swab( const void* src1, const void* src2, int isize);
void init_autoframeskip(void);
void      neogeo_exit(void);


extern char	neogeo_region;

extern Uint32 neocd_time;
extern int neo4all_skip_next_frame;

#ifndef PROFILER_NEO4ALL

#define neo4all_prof_start(A)
#define neo4all_prof_end(A)

#else

#define NEO4ALL_PROFILER_MAX 256

extern unsigned long long neo4all_prof_initial[NEO4ALL_PROFILER_MAX];
extern unsigned long long neo4all_prof_sum[NEO4ALL_PROFILER_MAX];
extern unsigned long long neo4all_prof_executed[NEO4ALL_PROFILER_MAX];

static __inline__ void neo4all_prof_start(unsigned a)
{
	neo4all_prof_executed[a]++;
#ifndef DREAMCAST
	neo4all_prof_initial[a]=SDL_GetTicks();
#else
	neo4all_prof_initial[a]=timer_us_gettime64();
#endif
}


static __inline__ void neo4all_prof_end(unsigned a)
{
#ifndef DREAMCAST
	neo4all_prof_sum[a]+=SDL_GetTicks()-neo4all_prof_initial[a];
#else
	extern unsigned neo4all_prof_total;
	unsigned i;
	for(i=0;i<neo4all_prof_total;i++)
		neo4all_prof_initial[i]+=5; // 6;
	neo4all_prof_sum[a]+=timer_us_gettime64()-neo4all_prof_initial[a]+1; //2;
#endif
}

#endif

void neo4all_prof_init(void);
void neo4all_prof_add(char *msg);
void neo4all_prof_show(void);

#define NEO4ALL_PROFILER_MAIN	0
#define NEO4ALL_PROFILER_M68K	1
#define NEO4ALL_PROFILER_Z80	2
#define NEO4ALL_PROFILER_MEM	3
#define NEO4ALL_PROFILER_VECTOR	4
#define NEO4ALL_PROFILER_BLIT	5
#define NEO4ALL_PROFILER_SOUND  6


#endif /* NEOCD_H */
