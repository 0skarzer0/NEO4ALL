/**************************************
****   INPUT.C  -  Input devices   ****
**************************************/

/*-- Include Files ---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "../neo4all.h"

#include "../menu/menu.h"

#ifdef DREAMCAST
#include <SDL_dreamcast.h>
#endif

#ifdef INPUT_INLINE
#define INPUT_STATIC_INLINE static __inline__
#else
#define INPUT_STATIC_INLINE static
#endif

/* Joystick definitions */
#define NUMJOYSTICKS    2
#define NUMAXES         2
#define AXISMIN         0

#ifndef DREAMCAST
#define AXISMAX         65536
#define NUMJOYBUTTONS   10
#else
#define AXISMAX         256
#define NUMJOYBUTTONS   1
#endif

#define AXISCENTRE      AXISMAX / 2
#define AXISTHRESHOLD   AXISCENTRE / 2   /* 2% joystick threshold */

/*--------------------------------------------------------------------------*/
#define P1UP    0x00000001
#define P1DOWN  0x00000002
#define P1LEFT  0x00000004
#define P1RIGHT 0x00000008
#define P1A     0x00000010
#define P1B     0x00000020
#define P1C     0x00000040
#define P1D     0x00000080

#define P2UP    0x00000100
#define P2DOWN  0x00000200
#define P2LEFT  0x00000400
#define P2RIGHT 0x00000800
#define P2A     0x00001000
#define P2B     0x00002000
#define P2C     0x00004000
#define P2D     0x00008000

#define P1START 0x00010000
#define P1SEL   0x00020000
#define P2START 0x00040000
#define P2SEL   0x00080000

#define SPECIAL 0x01000000


/*--------------------------------------------------------------------------*/
Uint32 keys   =~0;
Uint32 keyup  [SDLK_LAST];
Uint32 keydown[SDLK_LAST];

SDL_Joystick *joystick[NUMJOYSTICKS];
Uint32 joydown[NUMJOYSTICKS][NUMJOYBUTTONS];
Uint32 joyup  [NUMJOYSTICKS][NUMJOYBUTTONS];

Uint32 joymask[NUMJOYSTICKS][NUMAXES][AXISMAX];
Uint32 joyset [NUMJOYSTICKS][NUMAXES][AXISMAX];

unsigned char neo4all_intern_coin=7;

#ifdef AES
void input_init_aes_system(int system)
{
	if (system)
	{
    		keyup[SDLK_TAB]=joyup[0][9]=P1SEL;
		keyup[SDLK_1]=joyup[1][9]=P2SEL;
	}
	else
	{
		keyup[SDLK_TAB]=joyup[0][9]=SPECIAL;
		keyup[SDLK_1]=joyup[1][9]=SPECIAL;
	}
}
#endif
            
/*--------------------------------------------------------------------------*/
void input_init(void) {
    int i;
    
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    
    /* Player 1 */
    keyup[SDLK_UP]    =P1UP;    
    keyup[SDLK_DOWN]  =P1DOWN;
    keyup[SDLK_LEFT]  =P1LEFT;
    keyup[SDLK_RIGHT] =P1RIGHT;
    keyup[SDLK_LCTRL] =P1A;            joyup[0][0]=P1A;
    keyup[SDLK_LALT]  =P1B;            joyup[0][1]=P1B;
    keyup[SDLK_LSHIFT]=P1C;            joyup[0][2]=P1C;
    keyup[SDLK_SPACE] =P1D;            joyup[0][3]=P1D;
    keyup[SDLK_RETURN]=P1START;        joyup[0][8]=P1START;
    keyup[SDLK_TAB]   =P1SEL;          joyup[0][9]=P1SEL;

    
    /* Player 2 */
    keyup[SDLK_w]     =P2UP;
    keyup[SDLK_s]     =P2DOWN;
    keyup[SDLK_d]     =P2RIGHT;
    keyup[SDLK_a]     =P2LEFT;
    keyup[SDLK_e]     =P2A;            joyup[1][0]=P2A;
    keyup[SDLK_q]     =P2B;            joyup[1][1]=P2B;
    keyup[SDLK_c]     =P2C;            joyup[1][2]=P2C;
    keyup[SDLK_x]     =P2D;            joyup[1][3]=P2D;
    keyup[SDLK_z]     =P2START;        joyup[1][8]=P2START;
    keyup[SDLK_1]     =P2SEL;          joyup[1][9]=P2SEL;
    
    /* Special */
    keyup[SDLK_F1]    =SPECIAL;
    keyup[SDLK_F2]    =SPECIAL;
    keyup[SDLK_F3]    =SPECIAL;
    keyup[SDLK_F4]    =SPECIAL;
    keyup[SDLK_F12]   =SPECIAL;
#ifndef DREAMCAST
    keyup[SDLK_ESCAPE]=SPECIAL;
#endif
    keyup[SDLK_BACKSPACE]=SPECIAL;

    /* set key down mask */
    for(i=0;i<SDLK_LAST;i++) {
        keydown[i]=~keyup[i];
    }
    
    /* set joy button down mask */
    for(i=0; i<NUMJOYBUTTONS; i++) {
        joydown[0][i]=~joyup[0][i];
        joydown[1][i]=~joyup[1][i];
    }
    
    /* configure joystick axes */
    /* left and up */
    for(i=AXISMIN; i<AXISCENTRE-AXISTHRESHOLD; i++) {
        joymask[0][0][i]=~P1LEFT;
        joymask[1][0][i]=~P2LEFT;
        joymask[0][1][i]=~P1UP;
        joymask[1][1][i]=~P2UP;
        joyset[0][0][i]=P1RIGHT;
        joyset[1][0][i]=P2RIGHT;
        joyset[0][1][i]=P1DOWN;
        joyset[1][1][i]=P2DOWN;        
    }
    
    /* centre */
    for(i=AXISCENTRE-AXISTHRESHOLD; i<AXISCENTRE+AXISTHRESHOLD; i++) {
        joymask[0][0][i]=~0;
        joymask[1][0][i]=~0;
        joymask[0][1][i]=~0;
        joymask[1][1][i]=~0;
        joyset[0][0][i]=P1RIGHT|P1LEFT;
        joyset[1][0][i]=P2RIGHT|P2LEFT;
        joyset[0][1][i]=P1DOWN|P1UP;
        joyset[1][1][i]=P2DOWN|P2UP;        
    }
    
    /* right and down */
    for(i=AXISCENTRE+AXISTHRESHOLD; i<AXISMAX; i++) {
        joymask[0][0][i]=~P1RIGHT;
        joymask[1][0][i]=~P2RIGHT;
        joymask[0][1][i]=~P1DOWN;
        joymask[1][1][i]=~P2DOWN;
        joyset[0][0][i]=P1LEFT;
        joyset[1][0][i]=P2LEFT;
        joyset[0][1][i]=P1UP;
        joyset[1][1][i]=P2UP;            
    }
        
    SDL_JoystickEventState(SDL_ENABLE);   
    /* open joysticks */
    for(i=0; i<SDL_NumJoysticks() && i<NUMJOYSTICKS; i++) {
        joystick[i]=SDL_JoystickOpen(i);
        if(joystick[i]!=NULL) {
#ifndef DREAMCAST
            printf("\nOpened Joystick %d\n",i);
            printf("Name: %s\n", SDL_JoystickName(i));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick[i]));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick[i]));
#endif
        }
    }
#ifdef DREAMCAST
    SDL_DC_EmulateKeyboard(SDL_TRUE);
    SDL_DC_EmulateMouse(SDL_FALSE);
#endif

    input_reset();
}


void input_shutdown(void) {
    int i;
    
    /* Close joysticks */
    for(i=0; i<NUMJOYSTICKS; i++) {
        if(joystick[i]!=NULL) {
            SDL_JoystickClose(joystick[i]);    
            joystick[i]=NULL;
        }
    }
}

static int pulsando_menu=0;

INPUT_STATIC_INLINE void goMenu(void)
{
	if (keys==~0)
	{
    		int nook=1;
		pulsando_menu=0;
#ifdef PROFILER_NEO4ALL
		neo4all_prof_show();
#endif
		print_frameskip();
#ifdef SHOW_MENU
		sound_disable();
		cdda_pause();
		input_reset();
		menu_raise();
		while(nook)
		{
			int cas=run_mainMenu();
			if (cas>=2)
			{
#ifndef AES
				cdda_resume();
				SDL_Delay(100);
				cdda_stop();
#else
				if (cas==2)
				{
					aes4all_load();
					neo4all_init_memory();
					neo4all_load_bios();
				}
#endif
				nook=neogeo_hreset();
#ifndef AES
				if (nook)
					drawNoNeoGeoCD();
#endif
			}
			else
			{
				nook=0;
				cdda_resume();
			}
		}
		menu_unraise();
		sound_enable();
#endif
		input_reset();
		init_autoframeskip();
#ifdef PROFILER_NEO4ALL
		neo4all_prof_init();
#endif
	}
}

static void insert_coin(int n)
{
	if (!n)
		neo4all_intern_coin&=0x6;
	else
		neo4all_intern_coin&=0x5;
}

static void update_coin(void)
{
	static unsigned f=0;
	if (!(f&127))
		neo4all_intern_coin=0x7;
	f++;
}

#if !defined(DREAMCAST) || defined(AES)
INPUT_STATIC_INLINE void specialKey (SDLKey key) {
    switch(key) {
#ifndef DREAMCAST
	case SDLK_BACKSPACE:
		pulsando_menu=1;
		break;
	case SDLK_F1: video_fullscreen_toggle(); break;
	case SDLK_F2: insert_coin(0); break;
	case SDLK_F3: insert_coin(1); break;
        case SDLK_F4: sound_toggle(); break;
        case SDLK_F12: video_save_snapshot(); break;
        case SDLK_ESCAPE: exit(0); break;
#else
	case SDLK_TAB: insert_coin(0); break;
	case SDLK_1: insert_coin(1); break;
#endif
        default:
		pulsando_menu=0;
    }
}
#endif


INPUT_STATIC_INLINE void keyDown (SDLKey key) {
#if !defined(DREAMCAST) || defined(AES)
    if(keyup[key]&SPECIAL) {
        specialKey(key);
    } else {
#else
    if(!(keyup[key]&SPECIAL)) {
#endif
	pulsando_menu=0;
        keys &= keydown[key];
    }
}


INPUT_STATIC_INLINE void keyUp (SDLKey key) {
    if(!(keyup[key]&SPECIAL))
    	keys |= keyup[key];
    pulsando_menu=0;
}

#ifndef DREAMCAST
INPUT_STATIC_INLINE void joyDown (int which, int button) {
    if (which<NUMJOYSTICKS && button<NUMJOYBUTTONS) {
        keys &= joydown[which][button];
    }
}


INPUT_STATIC_INLINE void joyUp (int which, int button) {
    if (which<NUMJOYSTICKS && button<NUMJOYBUTTONS) {
        keys |= joyup[which][button];
    }
}
#endif


INPUT_STATIC_INLINE void joyMotion (int which, int axis, int value) {
    value+=AXISCENTRE;
    if (which<NUMJOYSTICKS && axis <NUMAXES && value<AXISMAX) {
        keys &= joymask[which][axis][value];
        keys |= joyset[which][axis][value];
    }
}


void processEvents(void) {
#ifndef AUTO_EVENTS
    SDL_Event event;
    
    update_coin();
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN: keyDown(event.key.keysym.sym); break;
            case SDL_KEYUP:   keyUp(event.key.keysym.sym); break;
#ifndef DREAMCAST
            case SDL_JOYBUTTONDOWN: joyDown(event.jbutton.which, event.jbutton.button); break;
            case SDL_JOYBUTTONUP:   joyUp(event.jbutton.which, event.jbutton.button); break;
#endif
            case SDL_JOYAXISMOTION: joyMotion(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
#ifdef DREAMCAST
				    if (event.jaxis.axis==3)
				    {
					    if (event.jaxis.value>200)
						    pulsando_menu=1;
					    else
						    pulsando_menu=0;
				    }
#endif
				    break;
            case SDL_QUIT:    exit(0); break;

            default:
                break;
        }
    }
    if (pulsando_menu)
    {
    	if (pulsando_menu<15)
		pulsando_menu++;
	else
		goMenu();
    }
#else
    static unsigned cuenta=0;
    unsigned rango=cuenta&127;

    if (rango<64)
	    keys |= P1A;
    else
	    keys &= ~P1A;

    if (cuenta==AUTO_EVENTS)
    {
#ifdef PROFILER_NEO4ALL
	    neo4all_prof_init();
#endif
	    init_autoframeskip();
    }
#ifdef AUTO_MAX_EVENTS
    if (cuenta>=AUTO_MAX_EVENTS)
    {
#ifdef PROFILER_NEO4ALL
	    neo4all_prof_show();
#endif
	    print_frameskip();
	    fflush(stdout);
	    exit(0);
    }
#endif
    cuenta++;
#endif
}
        
/*--------------------------------------------------------------------------*/
unsigned char read_player1(void) {
     return keys&0xff;
}

/*--------------------------------------------------------------------------*/
unsigned char read_player2(void)
{
    return (keys>>8)&0xff;
}

/*--------------------------------------------------------------------------*/
unsigned char read_pl12_startsel(void) {
    return (keys>>16)&0xff;
}


void input_reset(void)
{
    SDL_Event event;

    keys   =~0;
    SDL_Delay(133);
    while(SDL_PollEvent(&event)) 
	    SDL_Delay(10);
}
