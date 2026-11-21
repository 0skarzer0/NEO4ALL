#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "neocd.h"
#include "menu.h"

#include "msg.h"
#include "fade.h"

SDL_Surface *text_screen, *text_image, *text_background, *text_window_background;

static Uint32 menu_inv_color=0, menu_win0_color=0, menu_win1_color=0;

void write_text_pos(int x, int y, char * str);
void write_num(int x, int y, int v);
int menu_msg_pos=330;
Uint32 menu_msg_time;

static inline void text_draw_menu_msg()
{
/*
	Uint32 now=SDL_GetTicks();
	menu_msg_pos=330-((now-menu_msg_time)&0x7);
*/
	if (menu_msg_pos<MAX_SCROLL_MSG)
	{
//		menu_msg_time=now;
		menu_msg_pos=330;
	}
	else
		menu_msg_pos--;
	write_text_pos(menu_msg_pos,0,menu_msg);
}

void text_draw_background()
{
	SDL_Rect r;
	int i,j;

	for(i=0;i<320;i+=text_background->w)
		for(j=0;j<240;j+=text_background->h)
		{
			r.x=i;
			r.y=j;
			r.w=text_background->w;
			r.h=text_background->h;
			SDL_BlitSurface(text_background,NULL,text_screen,&r);
		}
	text_draw_menu_msg();
}

void text_flip(void)
{
	SDL_Delay(10);
	SDL_BlitSurface(text_screen,NULL,screen,NULL);
	video_flip(screen);
}

void init_text(int splash)
{
	SDL_Surface *tmp;

	text_screen=SDL_CreateRGBSurface(screen->flags,screen->w,screen->h,screen->format->BitsPerPixel,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
	tmp=SDL_LoadBMP(MENU_FILE_TEXT);
	if (text_screen==NULL || tmp==NULL)
	{
		puts("No se pudo crear el SDL_Surface text_screen o cargar " MENU_FILE_TEXT);
		exit(-1);
	}
	text_image=SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if (text_image==NULL)
	{
		puts("Fue imposible convertir text_image");
		exit(-2);
	}
	SDL_SetColorKey(text_image,(SDL_SRCCOLORKEY | SDL_RLEACCEL),SDL_MapRGB(text_image -> format, 0, 0, 0));
	tmp=SDL_LoadBMP(MENU_FILE_BACKGROUND);
	if (tmp==NULL)
	{
		puts("No se pudo cargar " MENU_FILE_BACKGROUND);
		exit(-3);
	}
	text_background=SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if (text_background==NULL)
	{
		puts("No se pudo convertir text_background");
		exit(-3);
	}
	tmp=SDL_LoadBMP(MENU_FILE_WINDOW);
	if (tmp==NULL)
	{
		puts("No se pudo cargar " MENU_FILE_WINDOW);
		exit(-4);
	}
	text_window_background=SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if (text_window_background==NULL)
	{
		puts("No se pudo convertir text_window_background");
		exit(-5);
	}
	if (splash)
	{
		SDL_Surface *sur;
		SDL_Rect r;
		int i,j;

		tmp=SDL_LoadBMP(MENU_FILE_SPLASH);
		if (tmp==NULL)
		{
			puts("No se pudo cargar " MENU_FILE_SPLASH);
			exit(-6);
		}
		sur = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		r.x=(text_screen->w - sur->w)/2;
		r.y=(text_screen->h - sur->h)/2;
		r.h=sur->w;
		r.w=sur->h;
		SDL_FillRect(text_screen,NULL,0);
		for (i=128;i>0;i-=4)
		{
#ifdef DREAMCAST
			vid_waitvbl();
#else
			SDL_Delay(50);
#endif
			SDL_BlitSurface(sur,NULL,text_screen,&r);
			fade16(text_screen,i);
			text_flip();
		}
		SDL_Delay(3000);
		for(i=0;i<128;i+=8)
		{
#ifdef DREAMCAST
			vid_waitvbl();
#else
			SDL_Delay(50);
#endif
			SDL_BlitSurface(sur,NULL,text_screen,&r);
			fade16(text_screen,i);
			text_flip();
		}
		SDL_FreeSurface(sur);
		for(i=128;i>0;i-=4)
		{
#ifdef DREAMCAST
			vid_waitvbl();
#else
			SDL_Delay(50);
#endif
			text_draw_background();
			fade16(text_screen,i);
			text_flip();
		}

	}
	else
	{
		text_draw_background();
		text_flip();
	}
	menu_inv_color=SDL_MapRGB(text_screen->format, 0x20, 0x20, 0x40);
	menu_win0_color=SDL_MapRGB(text_screen->format, 0x10, 0x08, 0x08);
	menu_win1_color=SDL_MapRGB(text_screen->format, 0x20, 0x10, 0x10);
	menu_msg_time=SDL_GetTicks();
}


void quit_text(void)
{
	SDL_FreeSurface(text_image);
	SDL_FreeSurface(text_background);
	SDL_FreeSurface(text_window_background);
	SDL_FreeSurface(text_screen);
}

void write_text_pos(int x, int y, char * str)
{
  int i, c;
  SDL_Rect src, dest;
  
  for (i = 0; i < strlen(str); i++)
    {
      c = -1;
      
      if (str[i] >= '0' && str[i] <= '9')
	c = str[i] - '0';
      else if (str[i] >= 'A' && str[i] <= 'Z')
	c = str[i] - 'A' + 10;
      else if (str[i] >= 'a' && str[i] <= 'z')
	c = str[i] - 'a' + 36;
      else if (str[i] == '#')
	c = 62;
      else if (str[i] == '=')
	c = 63;
      else if (str[i] == '.')
	c = 64;
      else if (str[i] == '_')
	c = -2;
      else if (str[i] == '-')
	c = -3;
      else if (str[i] == '/')
	c = 65;
      else if (str[i] == ',')
	c = 66;
      else if (str[i] == ':')
	c = 67;
      
      if (c >= 0)
	{
	  src.x = c * 8;
	  src.y = 0;
	  src.w = 8;
	  src.h = 8;
	  
	  dest.x = x + (i * 8);
	  dest.y = y;
	  dest.w = 8;
	  dest.h = 8;
	  
	  SDL_BlitSurface(text_image, &src,
			  text_screen, &dest);
	}
      else if (c == -2 || c == -3)
	{
	  dest.x = x + (i * 8);
	  
	  if (c == -2)
	    dest.y = y  + 7;
	  else if (c == -3)
	    dest.y = y  + 3;
	  
	  dest.w = 8;
	  dest.h = 1;
	  
	  SDL_FillRect(text_screen, &dest, menu_inv_color);
	}
    }
}

void write_text(int x, int y, char * str)
{
  int i, c;
  SDL_Rect src, dest;
  
  for (i = 0; i < strlen(str); i++)
    {
      c = -1;
      
      if (str[i] >= '0' && str[i] <= '9')
	c = str[i] - '0';
      else if (str[i] >= 'A' && str[i] <= 'Z')
	c = str[i] - 'A' + 10;
      else if (str[i] >= 'a' && str[i] <= 'z')
	c = str[i] - 'a' + 36;
      else if (str[i] == '#')
	c = 62;
      else if (str[i] == '=')
	c = 63;
      else if (str[i] == '.')
	c = 64;
      else if (str[i] == '_')
	c = -2;
      else if (str[i] == '-')
	c = -3;
      else if (str[i] == '/')
	c = 65;
      else if (str[i] == ',')
	c = 66;
      else if (str[i] == ':')
	c = 67;
      
      if (c >= 0)
	{
	  src.x = c * 8;
	  src.y = 0;
	  src.w = 8;
	  src.h = 8;
	  
	  dest.x = (x + i) * 8;
	  dest.y = y * 8; //10;
	  dest.w = 8;
	  dest.h = 8;
	  
	  SDL_BlitSurface(text_image, &src,
			  text_screen, &dest);
	}
      else if (c == -2 || c == -3)
	{
	  dest.x = (x + i) * 8;
	  
	  if (c == -2)
	    dest.y = y * 8 /*10*/ + 7;
	  else if (c == -3)
	    dest.y = y * 8 /*10*/ + 3;
	  
	  dest.w = 8;
	  dest.h = 1;
	  
	  SDL_FillRect(text_screen, &dest, menu_inv_color);
	}
    }
}


/* Write text, inverted: */

void write_text_inv(int x, int y, char * str)
{
  SDL_Rect dest;
  
  
  dest.x = (x * 8) -2 ;
  dest.y = (y * 8) /*10*/ - 2;
  dest.w = (strlen(str) * 8) + 4;
  dest.h = 12;

  SDL_FillRect(text_screen, &dest, menu_inv_color);

  write_text(x, y, str);
}


/* Write text, horizontally centered... */

void write_centered_text(int y, char * str)
{
  write_text(20 - (strlen(str) / 2), y/2, str);
}


/* Write numbers on the option screen: */

void write_num(int x, int y, int v)
{
  char str[24];
  
  sprintf(str, "%d", v);
  write_text(x, y, str);
}

void write_num_inv(int x, int y, int v)
{
  SDL_Rect dest;
  int i,l=1;

  for(i=10;i<1000000;i*=10)
	if (v/i)
		l++;
  	else
		break;
  	
  dest.x = (x * 8) -2 ;
  dest.y = (y * 8) /*10*/ - 2;
  dest.w = (l * 8) + 4;
  dest.h = 12;

  SDL_FillRect(text_screen, &dest, menu_inv_color);

  write_num(x, y, v);
}


void text_draw_window(int x, int y, int w, int h, char *title)
{
	int i,j;
	int r8x = x / 8;
	int r8y = y / 8;
	int rx = r8x * 8;
	int ry = r8y * 8;
	int r32w =  w / 32;
	int r24h =  h / 24;
	int rw = r32w * 32;
	int rh = r24h * 24;
	int r8w = rw / 8;

	SDL_Rect dest;

	dest.x = rx + 6;
	dest.y = ry - 4;
	dest.w = rw + 6;
	dest.h = rh + 18;
	SDL_FillRect(text_screen, &dest, menu_win0_color);

	dest.x = rx - 2;
	dest.y = ry - 10; //12;
	dest.w = rw + 4;
	dest.h = rh + 14; //16;
	SDL_FillRect(text_screen, &dest, menu_win1_color);

	for(i=0;i<r32w;i++)
		for(j=0;j<r24h;j++)
		{
			dest.x=rx+i*32;
			dest.y=ry+j*24;
			dest.w=32;
			dest.h=24;
			SDL_BlitSurface(text_window_background,NULL,text_screen,&dest);

		}

	write_text(r8x, r8y - 1, "o -");
	write_text(r8x + r8w - 6, r8y - 1, "-- =ox");
	write_text(r8x + ((r8w-strlen(title)) / 2), r8y - 1, title);

}

