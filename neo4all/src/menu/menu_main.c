#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "menu.h"

extern int neogeo_emulating;

#ifdef DREAMCAST
extern int __sdl_dc_wait_vblank;
#endif

static char *text_str_title="NEO4ALL - Main Menu --";
static char *text_str_68k="68K MHz";
static char *text_str_z80="Z80 MHz";
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
static char *text_str_separator="------------------------------";
static char *text_str_reset="Reset";
static char *text_str_run="Run";
static char *text_str_exit="Exit - Reboot Dreamcast";

int mainMenu_68k=1;
int mainMenu_z80=0;
int mainMenu_case=-1;
extern int	neogeo_frameskip;
extern char	neogeo_region;
extern int	neogeo_sound_enable;

enum { MAIN_MENU_CASE_REBOOT, MAIN_MENU_CASE_LOAD, MAIN_MENU_CASE_RUN, MAIN_MENU_CASE_RESET, MAIN_MENU_CASE_CANCEL };

static inline void draw_mainMenu(int c)
{
	static int b=0;
	int bb=(b%6)/3;

	text_draw_background();
	text_draw_window(40,24,260,200,text_str_title);
	write_text(6,4,text_str_separator);
	write_text(6,5,text_str_region);
	if ((neogeo_region==0)&&((c!=0)||(bb)))
		write_text_inv(16,5,text_str_jap);
	else
		write_text(16,5,text_str_jap);
	if ((neogeo_region==1)&&((c!=0)||(bb)))
		write_text_inv(20,5,text_str_usa);
	else
		write_text(20,5,text_str_usa);
	if ((neogeo_region==2)&&((c!=0)||(bb)))
		write_text_inv(24,5,text_str_eur);
	else
		write_text(24,5,text_str_eur);
	
	write_text(6,7,text_str_frameskip);
	if ((neogeo_frameskip==0)&&((c!=1)||(bb)))
		write_text_inv(16,7,text_str_0);
	else
		write_text(16,7,text_str_0);
	if ((neogeo_frameskip==1)&&((c!=1)||(bb)))
		write_text_inv(18,7,text_str_1);
	else
		write_text(18,7,text_str_1);
	if ((neogeo_frameskip==2)&&((c!=1)||(bb)))
		write_text_inv(20,7,text_str_2);
	else
		write_text(20,7,text_str_2);
	if ((neogeo_frameskip==3)&&((c!=1)||(bb)))
		write_text_inv(22,7,text_str_3);
	else
		write_text(22,7,text_str_3);
	if ((neogeo_frameskip==4)&&((c!=1)||(bb)))
		write_text_inv(24,7,text_str_4);
	else
		write_text(24,7,text_str_4);
	if ((neogeo_frameskip==5)&&((c!=1)||(bb)))
		write_text_inv(26,7,text_str_5);
	else
		write_text(26,7,text_str_5);
	if ((neogeo_frameskip==-1)&&((c!=1)||(bb)))
		write_text_inv(28,7,text_str_auto);
	else
		write_text(28,7,text_str_auto);

	write_text(6,9,text_str_sound);
	if ((neogeo_sound_enable==0)&&((c!=2)||(bb)))
		write_text_inv(16,9,text_str_off);
	else
		write_text(16,9,text_str_off);
	if ((neogeo_sound_enable!=0)&&((c!=2)||(bb)))
		write_text_inv(22,9,text_str_on);
	else
		write_text(22,9,text_str_on);

	write_text(6,11,text_str_68k);
	if ((mainMenu_68k==0)&&((c!=3)||(bb)))
		write_text_inv(16,11,text_str_under);
	else
		write_text(16,11,text_str_under);
	if ((mainMenu_68k==1)&&((c!=3)||(bb)))
		write_text_inv(22,11,text_str_normal);
	else
		write_text(22,11,text_str_normal);
	if ((mainMenu_68k==2)&&((c!=3)||(bb)))
		write_text_inv(29,11,text_str_over);
	else
		write_text(29,11,text_str_over);
	
	write_text(6,13,text_str_z80);
	if ((mainMenu_z80==0)&&((c!=4)||(bb)))
		write_text_inv(16,13,text_str_under);
	else
		write_text(16,13,text_str_under);
	if ((mainMenu_z80==1)&&((c!=4)||(bb)))
		write_text_inv(22,13,text_str_normal);
	else
		write_text(22,13,text_str_normal);
	if ((mainMenu_z80==2)&&((c!=4)||(bb)))
		write_text_inv(29,13,text_str_over);
	else
		write_text(29,13,text_str_over);
	write_text(6,14,text_str_separator);

	write_text(6,16,text_str_separator);
	if ((c==5)&&(bb))
		write_text_inv(6,17,text_str_reset);
	else
		write_text(6,17,text_str_reset);
	write_text(6,18,text_str_separator);

	write_text(6,20,text_str_separator);
	if ((c==6)&&(bb))
		write_text_inv(6,21,text_str_run);
	else
		write_text(6,21,text_str_run);
	write_text(6,22,text_str_separator);

	write_text(6,24,text_str_separator);
	if ((c==7)&&(bb))
		write_text_inv(6,25,text_str_exit);
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
			switch(event.key.keysym.sym)
			{
				case SDLK_RIGHT: right=1; break;
				case SDLK_LEFT: left=1; break;
				case SDLK_UP: up=1; break;
				case SDLK_DOWN: down=1; break;
				case SDLK_LCTRL: hit0=1; break;
				case SDLK_LALT: hit1=1; break;
			}
			if (hit1)
			{
				mainMenu_case=MAIN_MENU_CASE_CANCEL;
				end=1;
			}
			else if (up)
			{
				if (c>0) c=(c-1)%8;
				else c=7;
			}
			else if (down)
				c=(c+1)%8;
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
				case 5:	// RESET
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_RESET;
						end=1;
					}
					break;
				case 6:	// RUN
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_RUN;
						end=1;
					}
					break;
				case 7:	// EXIT
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

static inline void drawNoRun(void)
{
	int i;
	
	for(i=0;i<20;i++)
	{
		text_draw_background();
		text_draw_window(64,120,160,32,"--- ERROR ---");
		write_text(9,16,"Game not running");
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

int run_mainMenu()
{
	int end,c=0;
	mainMenu_case=-1;
#ifdef DREAMCAST
	__sdl_dc_wait_vblank=0;
#endif
	while(mainMenu_case<0)
	{
		raise_mainMenu();
		end=0;
		while(!end)
		{
			draw_mainMenu(c);
			end=key_mainMenu(&c);
		}
		unraise_mainMenu();
		switch(mainMenu_case)
		{
			case MAIN_MENU_CASE_LOAD:
//				run_menuLoad();	
				mainMenu_case=-1;
				break;
			case MAIN_MENU_CASE_RUN:
				mainMenu_case=1;
				neogeo_adjust_frameskip(neogeo_frameskip);
				neogeo_adjust_cycles(mainMenu_68k, mainMenu_z80);
				break;
			case MAIN_MENU_CASE_RESET:
				if (neogeo_emulating)
				{
					neogeo_adjust_frameskip(neogeo_frameskip);
					neogeo_adjust_cycles(mainMenu_68k, mainMenu_z80);
					mainMenu_case=2;
				}
				else
				{
					drawNoRun();
					mainMenu_case=-1;
				}
				break;
			case MAIN_MENU_CASE_REBOOT:
				SDL_Quit();
#ifdef DREAMCAST
				arch_reboot();
#else
				exit(0);
#endif
				break;
			default:
				mainMenu_case=-1;
		}
	}

	text_draw_window(96,64,140,32,"-------");
	write_text(14,9,"Please wait");
	text_flip();

#ifdef DREAMCAST
//	__sdl_dc_wait_vblank=mainMenu_vsync;
#endif
	return mainMenu_case;
}
