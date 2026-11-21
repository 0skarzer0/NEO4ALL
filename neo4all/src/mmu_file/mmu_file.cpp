#include "mmu_file.h"

// #define MMU_FILE_DEBUG

#include<stdio.h>

static FILE *mmu_file_fhandle[MMU_FILE_MAX_FILES];
static unsigned mmu_file_offset[MMU_FILE_MAX_FILES];
static unsigned mmu_file_pos[MMU_FILE_MAX_FILES];
static unsigned mmu_file_id[MMU_FILE_MAX_FILES];
static unsigned mmu_file_nfiles=0;
static int mmu_file_initted=0;

static void *mmu_file_memprefetch=NULL;

#ifdef MMU_FILE_SOUND_PATCH

#define SDL_DC_SPU_RAM_BASE     (0xa0800000+0x11000)

static void mmu_file_sound_silence(void)
{
	register uint32 *dat  =  (uint32*)SDL_DC_SPU_RAM_BASE;
	register unsigned size = MMU_FILE_SOUND_PATCH >> 5;
	while(size--)
	{
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
		*((vuint32*)dat++) = 0;
	}
}

#endif


static void mmu_file_real_handler(unsigned n, void *mem, unsigned start, unsigned size)
{
#ifdef MMU_FILE_DEBUG
	printf("mmu_file: Reading %i bytes, position %i\n",size,start+offset);
#endif
#ifdef MMU_FILE_SOUND_PATCH
	if (!mmu_handle_prefetching)
		mmu_file_sound_silence();
#endif
	unsigned offset=mmu_file_offset[n];
	FILE *fhandle=mmu_file_fhandle[n];
	if ((offset+start)!=mmu_file_pos[n])
		fseek(fhandle,offset+start,SEEK_SET);
	fread(mem,1,size,fhandle);
	mmu_file_pos[n]=offset+start+size;
}

static void mmu_file_prefetch_handler(void *mem, unsigned start, unsigned size)
{
	register unsigned *d=(unsigned *)mem;
	register unsigned *s=(unsigned *)mmu_file_memprefetch;
	register unsigned n=size>>5;
	while(n--){
		asm("pref @%0" : : "r" (s + 8));
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


static void mmu_file_real_handler_0(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x0,mem,start,size); }
static void mmu_file_real_handler_1(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x1,mem,start,size); }
static void mmu_file_real_handler_2(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x2,mem,start,size); }
static void mmu_file_real_handler_3(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x3,mem,start,size); }
static void mmu_file_real_handler_4(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x4,mem,start,size); }
static void mmu_file_real_handler_5(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x5,mem,start,size); }
static void mmu_file_real_handler_6(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x6,mem,start,size); }
static void mmu_file_real_handler_7(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x7,mem,start,size); }
static void mmu_file_real_handler_8(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x8,mem,start,size); }
static void mmu_file_real_handler_9(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0x9,mem,start,size); }
static void mmu_file_real_handler_a(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xa,mem,start,size); }
static void mmu_file_real_handler_b(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xb,mem,start,size); }
static void mmu_file_real_handler_c(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xc,mem,start,size); }
static void mmu_file_real_handler_d(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xd,mem,start,size); }
static void mmu_file_real_handler_e(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xe,mem,start,size); }
static void mmu_file_real_handler_f(void *mem, unsigned start, unsigned size) {
	mmu_file_real_handler(0xf,mem,start,size); }



static mmu_handle_func_t mmu_file_handler[16]={
	mmu_file_real_handler_0, mmu_file_real_handler_1,
	mmu_file_real_handler_2, mmu_file_real_handler_3,
	mmu_file_real_handler_4, mmu_file_real_handler_5,
	mmu_file_real_handler_6, mmu_file_real_handler_7,
	mmu_file_real_handler_8, mmu_file_real_handler_9,
	mmu_file_real_handler_a, mmu_file_real_handler_b,
	mmu_file_real_handler_c, mmu_file_real_handler_d,
	mmu_file_real_handler_e, mmu_file_real_handler_f
};





void mmu_file_init(void)
{
	unsigned i;
	if (mmu_file_initted)
		mmu_file_quit();
	else
		for(i=0;i<MMU_FILE_MAX_FILES;i++)
			mmu_file_fhandle[i]=NULL;
	mmu_file_memprefetch=malloc(PAGESIZE<<MMU_HANDLE_SLOT_SHIFT);
	mmu_handle_init();
	mmu_file_initted=1;
}

void mmu_file_restart(void)
{
	unsigned i;
	if (mmu_file_initted)
	{
		mmu_handle_restart();
		for(i=0;i<MMU_FILE_MAX_FILES;i++)
			if (mmu_file_fhandle[i])
			{
				fclose(mmu_file_fhandle[i]);
				mmu_file_fhandle[i]=NULL;
			}
		mmu_file_nfiles=0;
	}
	else
		mmu_file_init();
}

void mmu_file_quit(void)
{
	if (mmu_file_initted)
	{
		mmu_file_restart();
		mmu_handle_quit();
	}
	if (mmu_file_memprefetch)
	{
		free(mmu_file_memprefetch);
		mmu_file_memprefetch=NULL;
	}
	mmu_file_nfiles=0;
}

void mmu_file_set_pos(unsigned id, unsigned pos)
{
	unsigned i;
	for(i=0;i<mmu_file_nfiles;i++)
		if (mmu_file_id[i]==id)
			break;
	if (i<mmu_file_nfiles)
	{
		fseek(mmu_file_fhandle[i],pos,SEEK_SET);
		mmu_file_pos[i]=pos;
	}
}

void *mmu_file_add(char *filename, unsigned max_real, unsigned offset)
{
	void *ret=NULL;
	if (mmu_file_nfiles!=MMU_FILE_MAX_FILES)
	{
		mmu_file_fhandle[mmu_file_nfiles]=fopen(filename,"rb");
		if (mmu_file_fhandle[mmu_file_nfiles])
		{
			unsigned filesize;
			fseek(mmu_file_fhandle[mmu_file_nfiles],0,SEEK_END);
			filesize=ftell(mmu_file_fhandle[mmu_file_nfiles]);
			fseek(mmu_file_fhandle[mmu_file_nfiles],offset,SEEK_SET);
			if (filesize>offset)
				ret=mmu_handle_add(max_real, filesize-offset, mmu_file_handler[mmu_file_nfiles]);
			if (!ret)
			{
				fclose(mmu_file_fhandle[mmu_file_nfiles]);
				mmu_file_fhandle[mmu_file_nfiles]=NULL;
			}
			else
			{
				mmu_file_pos[mmu_file_nfiles]=offset;
				mmu_file_id[mmu_file_nfiles]=(unsigned)ret;
				mmu_handle_set_prefetch_func((unsigned)ret,mmu_file_prefetch_handler,mmu_file_memprefetch);
				mmu_file_offset[mmu_file_nfiles]=offset;
				mmu_file_nfiles++;
			}
		}
	}
	return ret;
}
