#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "neo4all.h"
#include "video/video.h"
#include "video/console.h"
#include "menu/menu.h"

#include "aes.h"

unsigned char aes4all_pen_usage[0x10000];
unsigned *aes4all_prefetch_bufffer=NULL;
void *aes4all_prealloc=NULL;

#ifdef DREAMCAST
#include "mmu_file/mmu_file.h"

unsigned __aes4all_prefetched__=1;


#define MAX_LOADING ((AES4ALL_TOTAL_MEMORY<<1)+1024)
static unsigned preft_ac=0;

static void prefetchme(unsigned size,unsigned max,void *ptr)
{
	unsigned i;
	if (max>size)
		max=size;
	mmu_handle_prefetching=1;
	if ((max)&&(ptr))
		for(i=0;i<max;i+=4096)
		{
			static unsigned loading_func_time=0;
			if (!(loading_func_time&15))
				text_draw_loading((preft_ac+(unsigned)i)>>2,MAX_LOADING>>2);
			loading_func_time++;
			__aes4all_prefetched__+=((unsigned char *)ptr)[i];
			mmu_handle_inc_frame();
		}
	mmu_handle_prefetching=0;
	preft_ac+=max;
}

static void prefetchme_inv(unsigned size,unsigned max,void *ptr)
{
	int i;
	if (max>size)
		max=size;
	mmu_handle_prefetching=1;
	if ((max>=4095)&&(ptr))
		for(i=max-4095;i>=0;i-=4096)
		{
			static unsigned loading_func_time=0;
			if (!(loading_func_time&15))
				text_draw_loading((preft_ac+(max-(unsigned)i))>>2,MAX_LOADING>>2);
			loading_func_time++;
			__aes4all_prefetched__+=((unsigned char *)ptr)[i];
			mmu_handle_inc_frame();
		}
	mmu_handle_prefetching=0;
	preft_ac+=max;
}

#endif

static void load_pen_usage(FILE *f, unsigned size)
{
	unsigned char *p=(unsigned char *)malloc(4*1024);
	size>>=10;
	unsigned addr=0;
	do{
		bzero(p,4*1024);
		fread(p,1,4*1024,f);
		unsigned i;
		for(i=0;i<4*1024;i++,addr++)
			if (p[i])
				aes4all_pen_usage[addr>>3]|=(1<<(addr&0x7));
			else
				aes4all_pen_usage[addr>>3]&=0xFF-(1<<(addr&0x7));
	}while(size--);
	free(p);
}

unsigned char aes4all_ram[0x10000];
unsigned char aes4all_nvram[0x10000];

void *aes4all_memory;
void *aes4all_memory_cpu;
void *aes4all_memory_bios;
void *aes4all_memory_sfix_game;
void *aes4all_memory_sfix_board;
void *aes4all_memory_sm1;
void *aes4all_memory_sound1;
void *aes4all_memory_sound2;
void *aes4all_memory_gfx;
void *aes4all_memory_ng_lo;
void *aes4all_memory_fix_board_usage;
void *aes4all_memory_fix_game_usage;
void *aes4all_memory_pen_usage=NULL;

unsigned aes4all_bankaddress = 0;

int aes4all_prefetch_all(void)
{
	FILE *f;
	if (!aes4all_filename)
		return 1;
	f=fopen(aes4all_filename,"rb");
	if (!f)
		return 2;

	console_puts("Prefetching ROM...");
//puts("Prefetching ROM...");
	neogeo_emulating=0;
#ifdef DREAMCAST
	mmu_handle_reset();

#ifndef AES_PREFETCHING
	memset(aes4all_prefetch_bufffer,0,4*65537);
	fseek(f,aes4all_prefetch_init,SEEK_SET);
	unsigned lastpos=fread(aes4all_prefetch_bufffer,4,65536,f);
	if (lastpos<=65536)
		aes4all_prefetch_bufffer[lastpos]=0xFFFFFFFF;
	if (aes4all_prefetch_bufffer[0]==0xFFFFFFFF)
	{
		if ((unsigned)aes4all_mmu<0x80000000)
		{
			preft_ac=0;
			prefetchme(aes4all_memory_cpu_size,((2*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_cpu);
			prefetchme(aes4all_memory_bios_size,((1*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_bios);
			prefetchme(aes4all_memory_sound1_size,((2*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_sound1);
			prefetchme(aes4all_memory_gfx_size,((5*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_gfx);
			prefetchme_inv(aes4all_memory_sound1_size,((2*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_sound1);
			prefetchme_inv(aes4all_memory_gfx_size,((5*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_gfx);
			prefetchme_inv(aes4all_memory_cpu_size,((2*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_cpu);
			prefetchme_inv(aes4all_memory_bios_size,((1*AES4ALL_TOTAL_MEMORY)/10),aes4all_memory_bios);
		}
		else
			mmu_handle_prefetch_all(text_draw_loading);
	}
	else
		mmu_handle_prefetch_by_array((unsigned)aes4all_mmu,(unsigned *)aes4all_prefetch_bufffer,text_draw_loading);
#else
	mmu_handle_disable_slice(1);
#endif

#else
	text_draw_loading(1,12);
	fseek(f,aes4all_memory_cpu_init,SEEK_SET);
	fread(aes4all_memory_cpu,1,aes4all_memory_cpu_size,f);

	text_draw_loading(2,12);
	fseek(f,aes4all_memory_bios_init,SEEK_SET);
	fread(aes4all_memory_bios,1,aes4all_memory_bios_size,f);

	text_draw_loading(3,12);
	fseek(f,aes4all_memory_sfix_game_init,SEEK_SET);
	fread(aes4all_memory_sfix_game,1,aes4all_memory_sfix_game_size,f);

	text_draw_loading(4,12);
	fseek(f,aes4all_memory_sfix_board_init,SEEK_SET);
	fread(aes4all_memory_sfix_board,1,aes4all_memory_sfix_board_size,f);

	text_draw_loading(5,12);
	fseek(f,aes4all_memory_sm1_init,SEEK_SET);
	fread(aes4all_memory_sm1,1,aes4all_memory_sm1_size,f);

	text_draw_loading(6,12);
	fseek(f,aes4all_memory_sound1_init,SEEK_SET);
	fread(aes4all_memory_sound1,1,aes4all_memory_sound1_size,f);

	text_draw_loading(7,12);
	if (aes4all_memory_sound2_size)
	{
		fseek(f,aes4all_memory_sound2_init,SEEK_SET);
		fread(aes4all_memory_sound2,1,aes4all_memory_sound2_size,f);
	}

	text_draw_loading(8,12);
	fseek(f,aes4all_memory_gfx_init,SEEK_SET);
	fread(aes4all_memory_gfx,1,aes4all_memory_gfx_size,f);

	text_draw_loading(9,12);
	fseek(f,aes4all_memory_ng_lo_init,SEEK_SET);
	fread(aes4all_memory_ng_lo,1,aes4all_memory_ng_lo_size,f);

	text_draw_loading(10,12);
	fseek(f,aes4all_memory_fix_board_usage_init,SEEK_SET);
	fread(aes4all_memory_fix_board_usage,1,aes4all_memory_fix_board_usage_size,f);

	text_draw_loading(11,12);
	fseek(f,aes4all_memory_fix_game_usage_init,SEEK_SET);
	fread(aes4all_memory_fix_game_usage,1,aes4all_memory_fix_game_usage_size,f);
#endif
	text_draw_loading(12,12);
	fseek(f,aes4all_memory_pen_usage_init,SEEK_SET);
	load_pen_usage(f,aes4all_memory_pen_usage_size&(AES4ALL_PEN_USAGE_SIZE-1));

	fclose(f);

	video_fix_usage=(unsigned char*)aes4all_memory_fix_game_usage;
	neogeo_fix_memory=(char*)aes4all_memory_sfix_game;

	memcpy(mame_z80mem, aes4all_memory_sm1, 0xf800);
	sound_emulate_start();

	return 0;
}

