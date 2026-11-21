#include<stdio.h>
#include<SDL/SDL.h>

#ifdef SHOW_MENU

#ifdef AES
#define MENU_FILE_SPLASH DATA_PREFIX "splash_aes.bmp"
#define MENU_FILE_BACKGROUND DATA_PREFIX "background_aes.bmp"
#else
#define MENU_FILE_SPLASH DATA_PREFIX "splash_cd.bmp"
#define MENU_FILE_BACKGROUND DATA_PREFIX "background_cd.bmp"
#endif
#define MENU_FILE_WINDOW DATA_PREFIX "window.bmp"
#define MENU_FILE_TEXT DATA_PREFIX "text.bmp"
#define MENU_FILE_TEXT_INV DATA_PREFIX "text2.bmp"
#define MENU_FILE_LOADING DATA_PREFIX "loading.bmp"
#ifdef DREAMCAST
#define MENU_DIR_DEFAULT "/cd"
#else
#define MENU_DIR_DEFAULT "."
#endif

void text_draw_background();
void init_text(int splash);
void quit_text(void);
void write_text(int x, int y, char * str);
void write_text_inv(int x, int y, char * str);
void write_text_sel(int x, int y, char * str);
void write_centered_text(int y, char * str);
void write_centered_text_inv(int y, char * str);
void write_num(int x, int y, int v);
void write_num_inv(int x, int y, int v);
void write_text_pos(int x, int y, char * str);
void write_text_pos_inv(int x, int y, char * str);
void text_draw_barra(int x, int y, int w, int h, int per, int max);
void text_draw_window(int x, int y, int w, int h, char *title);
void text_draw_loading(int per, int max);
void text_draw_cdload(void);
void text_draw_saving(int per, int max);
// void text_draw_menu_msg();
void text_flip(void);
void menu_raise(void);
void menu_unraise(void);


int run_mainMenu();
int run_menuLoad();
int run_menuGame();
int run_menuControl();
void drawNoNeoGeoCD(void);
void drawNoCD(void);
void drawNoBIOS(void);

#else

#define init_text(A) 
#define text_draw_loading(A,B)
#define text_draw_saving(A,B)

#endif
