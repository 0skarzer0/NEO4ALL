#ifdef DREAMCAST
#include<kos.h>
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "neo4all.h"
#include "menu.h"

#include "sound/sound.h"

#if defined (DREAMCAST) && defined(AES_PREFETCHING)
#include "mmu_file/mmu_file.h"
#endif

#ifdef USE_VIDEO_GL
#include "video/videogl.h"
#endif


#ifdef DREAMCAST
#include <SDL_dreamcast.h>
#endif

#ifndef AES
static char *text_str_title="--- NEO4ALL/CD rc1 ----";
#else
static char *text_str_title="-- NEO4ALL/AES beta1 --";
#endif
static char *text_str_68k="68K CLK";
static char *text_str_z80="Z80 CLK";
static char *text_str_under="Under";
static char *text_str_normal="Normal";
static char *text_str_over="Over";
static char *text_str_region="Region ";
static char *text_str_usa="USA";
static char *text_str_jap="JAP";
static char *text_str_eur="EUR";
static char *text_str_frameskip="Frameskip";
static char *text_str_0="0";
static char *text_str_1="1";
static char *text_str_2="2";
static char *text_str_3="3";
static char *text_str_4="4";
static char *text_str_5="5";
static char *text_str_auto="auto";
static char *text_str_sound="Sound";
static char *text_str_on="on";
static char *text_str_off="off";
static char *text_str_filter="Filter";
static char *text_str_filter_none="None";
static char *text_str_filter_bilinear="Bilinear";
static char *text_str_accurate="Sync SFX";
static char *text_str_cdaudio="CDDA";
static char *text_str_separator="------------------------------";
#ifndef AES
static char *text_str_reset="Reset";
#else
static char *text_str_reset="Select ROM";
#endif
static char *text_str_run="Run";
static char *text_str_exit="Exit - Reboot Dreamcast";

int mainMenu_filter=0;
int mainMenu_68k=1;
int mainMenu_z80=1;
int mainMenu_case=-1;
extern int	neogeo_frameskip;
extern char	neogeo_region;
extern int	neogeo_sound_enable;
extern int	menu_moving;
extern int	neogeo_accurate;
#ifndef AES
extern int	cdda_disabled;
#else
static int	cdda_disabled=1;
#endif

enum { MAIN_MENU_CASE_REBOOT, MAIN_MENU_CASE_LOAD, MAIN_MENU_CASE_RUN, MAIN_MENU_CASE_RESET, MAIN_MENU_CASE_CANCEL };


#if defined(DREAMCAST) && !defined(AES)
#include<dirent.h>
static int list_files(void)
{
	int found=0;
	DIR *d=opendir("/cd");
	unsigned i;
	while(d!=NULL && !found)
	{
		struct dirent *actual=readdir(d);
		if (actual==NULL)
			break;
		found++;
	}
	closedir(d);
	return found;
}
static int try_to_list_files(void)
{
	int rt;
	for(rt=0;(rt<20)&&(!list_files());rt++)
		SDL_Delay(500);
	return list_files();
}
#endif

static inline void draw_mainMenu(int c)
{
	static int b=0;
	int bb=(b%9)/3;

	text_draw_background();
	text_draw_window(40,24,260,200,text_str_title);
	write_text(6,3,text_str_separator);
	write_text(6,4,text_str_region);
	if ((neogeo_region==0)&&((c!=0)||(bb)))
		write_text_sel(16,4,text_str_jap);
	else
		write_text(16,4,text_str_jap);
	if ((neogeo_region==1)&&((c!=0)||(bb)))
		write_text_sel(20,4,text_str_usa);
	else
		write_text(20,4,text_str_usa);
	if ((neogeo_region==2)&&((c!=0)||(bb)))
		write_text_sel(24,4,text_str_eur);
	else
		write_text(24,4,text_str_eur);
	
	write_text(6,6,text_str_frameskip);
	if ((neogeo_frameskip==0)&&((c!=1)||(bb)))
		write_text_sel(16,6,text_str_0);
	else
		write_text(16,6,text_str_0);
	if ((neogeo_frameskip==1)&&((c!=1)||(bb)))
		write_text_sel(18,6,text_str_1);
	else
		write_text(18,6,text_str_1);
	if ((neogeo_frameskip==2)&&((c!=1)||(bb)))
		write_text_sel(20,6,text_str_2);
	else
		write_text(20,6,text_str_2);
	if ((neogeo_frameskip==3)&&((c!=1)||(bb)))
		write_text_sel(22,6,text_str_3);
	else
		write_text(22,6,text_str_3);
	if ((neogeo_frameskip==4)&&((c!=1)||(bb)))
		write_text_sel(24,6,text_str_4);
	else
		write_text(24,6,text_str_4);
	if ((neogeo_frameskip==5)&&((c!=1)||(bb)))
		write_text_sel(26,6,text_str_5);
	else
		write_text(26,6,text_str_5);
	if ((neogeo_frameskip==-1)&&((c!=1)||(bb)))
		write_text_sel(28,6,text_str_auto);
	else
		write_text(28,6,text_str_auto);

	write_text(6,8,text_str_sound);
	if ((neogeo_sound_enable==0)&&((c!=2)||(bb)))
		write_text_sel(16,8,text_str_off);
	else
		write_text(16,8,text_str_off);
	if ((neogeo_sound_enable!=0)&&((c!=2)||(bb)))
		write_text_sel(22,8,text_str_on);
	else
		write_text(22,8,text_str_on);

	write_text(6,10,text_str_68k);
	if ((mainMenu_68k==0)&&((c!=3)||(bb)))
		write_text_sel(16,10,text_str_under);
	else
		write_text(16,10,text_str_under);
	if ((mainMenu_68k==1)&&((c!=3)||(bb)))
		write_text_sel(22,10,text_str_normal);
	else
		write_text(22,10,text_str_normal);
	if ((mainMenu_68k==2)&&((c!=3)||(bb)))
		write_text_sel(29,10,text_str_over);
	else
		write_text(29,10,text_str_over);
	
	write_text(6,12,text_str_z80);
	if ((mainMenu_z80==0)&&((c!=4)||(bb)))
		write_text_sel(16,12,text_str_under);
	else
		write_text(16,12,text_str_under);
	if ((mainMenu_z80==1)&&((c!=4)||(bb)))
		write_text_sel(22,12,text_str_normal);
	else
		write_text(22,12,text_str_normal);
	if ((mainMenu_z80==2)&&((c!=4)||(bb)))
		write_text_sel(29,12,text_str_over);
	else
		write_text(29,12,text_str_over);

	write_text(6,14,text_str_filter);
	if ((mainMenu_filter==0)&&((c!=5)||(bb)))
		write_text_sel(16,14,text_str_filter_none);
	else
		write_text(16,14,text_str_filter_none);
	if ((mainMenu_filter!=0)&&((c!=5)||(bb)))
		write_text_sel(22,14,text_str_filter_bilinear);
	else
		write_text(22,14,text_str_filter_bilinear);

	write_text(6,16,text_str_cdaudio);
	if ((cdda_disabled!=0)&&((c!=6)||(bb)))
		write_text_sel(16,16,text_str_off);
	else
		write_text(16,16,text_str_off);
	if ((cdda_disabled==0)&&((c!=6)||(bb)))
		write_text_sel(22,16,text_str_on);
	else
		write_text(22,16,text_str_on);

	write_text(6,18,text_str_accurate);
	if ((neogeo_accurate==0)&&((c!=7)||(bb)))
		write_text_sel(16,18,text_str_off);
	else
		write_text(16,18,text_str_off);
	if ((neogeo_accurate!=0)&&((c!=7)||(bb)))
		write_text_sel(22,18,text_str_on);
	else
		write_text(22,18,text_str_on);

	write_text(6,20,text_str_separator);
	if ((c==8)&&(bb))
		write_text_sel(6,21,text_str_reset);
	else
		write_text(6,21,text_str_reset);

	write_text(6,22,text_str_separator);
	if ((c==9)&&(bb))
		write_text_sel(6,23,text_str_run);
	else
		write_text(6,23,text_str_run);

	write_text(6,24,text_str_separator);
	if ((c==10)&&(bb))
		write_text_sel(6,25,text_str_exit);
	else
		write_text(6,25,text_str_exit);
	write_text(6,26,text_str_separator);

	text_flip();
	b++;
}

static inline int key_mainMenu(int *cp)
{
	int c=(*cp);
	int end=0;
	int left=0, right=0, up=0, down=0, hit0=0, hit1=0;
	SDL_Event event;

	while (SDL_PollEvent(&event) > 0)
	{
		if (event.type == SDL_QUIT)
		{
			mainMenu_case=MAIN_MENU_CASE_REBOOT;
			end=-1;
		}
		else
		if (event.type == SDL_KEYDOWN)
		{
			sound_play_beep();
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE: mainMenu_case=MAIN_MENU_CASE_REBOOT; end=-1; break;
				case SDLK_d:
				case SDLK_RIGHT: right=1; break;
				case SDLK_a:
				case SDLK_LEFT: left=1; break;
				case SDLK_w:
				case SDLK_UP: up=1; break;
				case SDLK_s:
				case SDLK_DOWN: down=1; break;
				case SDLK_z:
				case SDLK_RETURN:
				case SDLK_q:
				case SDLK_LALT:
				case SDLK_e:
				case SDLK_LCTRL: hit0=1; break;
				case SDLK_1:
				case SDLK_TAB:
				case SDLK_BACKSPACE: hit1=1; break;
			}
			if (hit1)
			{
				mainMenu_case=MAIN_MENU_CASE_RUN;
				end=1;
			}
			else if (up)
			{
				if (c>0) c=(c-1)%11;
				else c=10;
			}
			else if (down)
				c=(c+1)%11;
			else
			switch(c)
			{
				case 0:	// REGION
					if (left)
					{
						if (neogeo_region>0)
							neogeo_region--;
					}
					else if (right)
					{
						if (neogeo_region<2)
							neogeo_region++;
					}
					break;
				case 1:	// FRAMESKIP
					if (left)
					{
						if (neogeo_frameskip>-1)
							neogeo_frameskip--;
						else
							neogeo_frameskip=5;
					}
					else if (right)
					{
						if (neogeo_frameskip<5)
							neogeo_frameskip++;
						else
							neogeo_frameskip=-1;
					}
					break;
				case 2:	// SOUND
					if (left)
					{
						if (neogeo_sound_enable)
							neogeo_sound_enable=0;
					}
					else if (right)
					{
						if (!neogeo_sound_enable)
							neogeo_sound_enable=1;
					}
					break;
				case 3: // 68K
					if (left)
					{
						if (mainMenu_68k>0)
							mainMenu_68k--;
					}
					else if (right)
					{
						if (mainMenu_68k<2)
							mainMenu_68k++;
					}
					break;
				case 4:	// Z80
					if (left)
					{
						if (mainMenu_z80>0)
							mainMenu_z80--;
					}
					else if (right)
					{
						if (mainMenu_z80<2)
							mainMenu_z80++;
					}
					break;
				case 5:	// FILTER
#ifdef USE_VIDEO_GL
					if (left)
					{
						if (mainMenu_filter)
							mainMenu_filter=0;
					}
					else if (right)
					{
						if (!mainMenu_filter)
							mainMenu_filter=1;
					}
					neogeo_adjust_filter(mainMenu_filter);
#endif
					break;
				case 6:	// CDAUDIO
					if (left)
					{
						if (!cdda_disabled)
							cdda_disabled=1;
					}
					else if (right)
					{
						if (cdda_disabled)
							cdda_disabled=0;
					}
					break;
				case 7:	// ACCURATE
					if (left)
					{
						if (neogeo_accurate)
							neogeo_accurate=0;
					}
					else if (right)
					{
						if (!neogeo_accurate)
							neogeo_accurate=1;
					}
					break;
				case 8:	// RESET
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_RESET;
						end=1;
					}
					break;
				case 9:	// RUN
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_RUN;
						end=1;
					}
					break;
				case 10:// EXIT
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_REBOOT;
						end=1;
					}
					break;
			}
		}
	}


	(*cp)=c;
	return end;
}

static inline void raise_mainMenu()
{
	int i;

	text_draw_background();
	text_flip();
	for(i=0;i<10;i++)
	{
		text_draw_background();
		text_draw_window(40,(10-i)*24,260,200,text_str_title);
		text_flip();
	}
}

static inline void unraise_mainMenu()
{
	int i;

	for(i=9;i>=0;i--)
	{
		text_draw_background();
		text_draw_window(40,(10-i)*24,260,200,text_str_title);
		text_flip();
	}
	text_draw_background();
	text_flip();
}

void drawNoCD(void)
{
	int i;
	sound_play_error();
	for(i=0;i<200;i++)
	{
		text_draw_background();
		text_draw_window(64,60,160,32,"--- ERROR ---");
#ifndef AES
		write_text(9,8,"No disk in drive");
#else
		write_text(9,8,"No ROM selected");
#endif
		text_flip();
	}
}

#ifndef AES
void drawNoNeoGeoCD(void)
{
	int i;
	sound_play_error();
	for(i=0;i<200;i++)
	{
		text_draw_background();
		text_draw_window(64,60,160,32,"--- ERROR ---");
		write_text(9,8,"No NeoGeo/CD disk");
		text_flip();
	}
}

void drawNoBIOS(void)
{
	int i;
	sound_play_error();
	for(i=0;i<400;i++)
	{
		text_draw_background();
		text_draw_window(48,60,224,32,"--- FATAL ERROR ---");
		write_text(8,8,"Could not load NEOCD.BIN");
		text_flip();
	}
}

void drawNoRom(void)
{
	int i;
	SDL_Event ev;

	for(i=0;i<20;i++)
	{
		text_draw_background();
		text_draw_window(54,110,250,64,"--- ERROR ---");
		write_text(14,14,"ROM not found");
		write_text(8,16,"Press any button to retry");
		text_flip();
	}
	SDL_Delay(333);
    	while(SDL_PollEvent(&ev))
		if (ev.type==SDL_QUIT)
			exit(1);
		else
		    SDL_Delay(10);
    	while(!SDL_PollEvent(&ev))
		    SDL_Delay(10);
    	while(SDL_PollEvent(&ev))
		if (ev.type==SDL_QUIT)
			exit(1);
	text_draw_background();
	text_flip();
	SDL_Delay(333);
}
#endif

int run_mainMenu()
{
	static int c=8;
	int end,need_reset=-1;
	mainMenu_case=-1;
#ifdef DREAMCAST
	SDL_DC_VerticalWait(SDL_FALSE);
#endif
	sound_play_menu();
	while(mainMenu_case<0)
	{
		raise_mainMenu();
		menu_moving=1;
		end=0;
		while(!end)
		{
			draw_mainMenu(c);
			end=key_mainMenu(&c);
		}
		menu_moving=0;
		unraise_mainMenu();
		switch(mainMenu_case)
		{
/*
			case MAIN_MENU_CASE_LOAD:
//				run_menuLoad();	
				mainMenu_case=-1;
				break;
*/
			case MAIN_MENU_CASE_RUN:
#if defined(AES)
				if (neogeo_yet_running)
				{
					if (need_reset>0)
						mainMenu_case=need_reset+1;
					else
						mainMenu_case=1;
				}
				else
					if (need_reset>0)
						mainMenu_case=2;
					else
					{
						mainMenu_case=-1;
						drawNoCD();
						break;
					}
#else
#if defined(DREAMCAST) && !defined(AES)
				if (!try_to_list_files())
				{
					mainMenu_case=-1;
					drawNoCD();
					break;
				}
#endif
				mainMenu_case=1;
#endif
				neogeo_adjust_frameskip(neogeo_frameskip);
				neogeo_adjust_cycles(mainMenu_68k, mainMenu_z80);
				break;
			case MAIN_MENU_CASE_RESET:
#ifdef AES
				if (need_reset>0)
					run_menuLoad();
				else
					need_reset=run_menuLoad();
				mainMenu_case=-1;
#else
#if defined(DREAMCAST) && !defined(AES)
				if (!try_to_list_files())
				{
					mainMenu_case=-1;
					drawNoCD();
					break;
				}
#endif
				neogeo_adjust_frameskip(neogeo_frameskip);
				neogeo_adjust_cycles(mainMenu_68k, mainMenu_z80);
				if (neogeo_emulating)
					mainMenu_case=2;
				else
					mainMenu_case=1;
#endif
				break;
			case MAIN_MENU_CASE_REBOOT:
#if defined (DREAMCAST) && defined(AES_PREFETCHING)
				mmu_handle_dump_memaccess();
#endif
				sound_play_bye();
				menu_unraise();
				SDL_Delay(333);
				SDL_Quit();
#if defined(DREAMCAST) && defined(REBOOT_DREAMCAST)
				arch_reboot();
#else
				exit(0);
#endif
				break;
			default:
				mainMenu_case=-1;
		}
	}

/*
	text_draw_window(96,64,140,32,"-------");
	write_text(14,9,"Please wait");
	text_flip();
*/

#ifdef DREAMCAST
//	__sdl_dc_wait_vblank=mainMenu_vsync;
#endif
	return mainMenu_case;
}
