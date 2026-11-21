/*******************************************
**** VIDEO.C - Video Hardware Emulation ****
*******************************************/

//-- Include Files -----------------------------------------------------------
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "video.h"
#include "../neocd.h"
#ifdef USE_VIDEO_GL
#include "videogl.h"
#endif

#include "console.h"

unsigned char	video_shrinky[17];

#ifdef DREAMCAST
#include<kos.h>
#else
#include<string.h>
#define sq_set16(aaa,bbb,ccc)	{ \
				int sqinc; \
				short *sqsrc=(short *)aaa; \
				short sqset=bbb; \
				for(sqinc=0;sqinc<ccc;sqinc++) \
					sqsrc[sqinc]=sqset; \
				}
#endif


static unsigned int   fc=0;

static int used_blitter=0;


#ifdef PROFILER
extern Uint32 profiler_media3;
extern Uint32 profiler_media4;
#endif

/*
unsigned ntiles=0;
unsigned cacheados=0;
*/


void blitter(void) {
	used_blitter=1;
#ifdef PROFILER
profiler_media3+=(SDL_GetTicks()-prof_actual);
Uint32 prof_actual=SDL_GetTicks();
#endif
#ifndef USE_VIDEO_GL
#ifdef DOUBLE_BUFFER
	SDL_BlitSurface(video_buffer,&src,screen,NULL); // &dest);
#endif
	SDL_Flip(screen);
#ifdef PROFILER
profiler_media4+=(SDL_GetTicks()-prof_actual);
#endif
#else
#ifndef DREAMCAST
    SDL_GL_SwapBuffers();
#else
    glKosFinishFrame();
#endif
#endif
/*
if (ntiles)
printf("NTILES=%i, CACHEADOS=%i, EFICIENCIA %i%%\n",ntiles,cacheados,(cacheados*100)/ntiles);
puts("NTILES=0");
*/
//printf("tile_l2_ptr=%i\n",tile_l2_ptr); fflush(stdout);
}

void video_draw_blank(void)
{
#ifndef USE_VIDEO_GL
	SDL_FillRect(screen,NULL,0);
#else
    	glClearColor(0.0, 0.0, 0.0, 0.0);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	blitter();
}

static __inline__ void video_draw_screen_first(void)
{
//ntiles=cacheados=0;
#ifdef PROFILER
Uint32 prof_actual=SDL_GetTicks();
#endif

   int    a;

#ifndef USE_VIDEO_GL
  for (a=0; a<224; a++) 
  	sq_set16(video_line_ptr[a], video_paletteram_pc[4095], 320*2);  
#else
#ifndef DREAMCAST
  glClearColor( ((video_paletteram_pc[4095]>>0)&0x1F),
		((video_paletteram_pc[4095]>>5)&0x1F),
		((video_paletteram_pc[4095]>>10)&0x1F), 255.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
  glKosBeginFrame();
  {
  	unsigned r=(video_paletteram_pc[4095]>>10)&0x1F;
  	unsigned g=(video_paletteram_pc[4095]>>5)&0x1F;
  	unsigned b=(video_paletteram_pc[4095]>>0)&0x1F;
  	pvr_set_bg_color(	((float)r)/31.9f,
			  	((float)g)/31.9f,
				((float)b)/31.9f
		  	);
//  	vid_border_color(r*8,g*8,b*8);
  }
  glKosFinishList();
#endif

  if (video_palette_dirty)
  {
  	for(a=0;a<TCACHE_SIZE;a++)
		if (video_palette_use[tile_cache[a].pal])
			tile_cache[a].pal=-1;
	memset(video_palette_use,0,0x100);
	video_palette_dirty=0;
  }
  tile_l2_ptr=0;
  font_l2_ptr=0;
  memset(tile_l2_cache,0,TCACHE_SIZE);
  memset(font_l2_cache,0,FCACHE_SIZE);
  tile_z=1.0;

#endif

}

//---------------------------------------------------------------------------- 
void video_draw_screen1() 
{
   int         sx =0,sy =0,oy =0,my =0,zx = 1, rzy = 1; 
   int         offs,i,count,y; 
   int         tileno,tileatr,t1,t2,t3; 
   char         fullmode=0; 
   int         ddax=0,dday=0,rzx=15,yskip=0; 

   if (!neo4all_skip_next_frame)
   	video_draw_screen_first();

   for (count=0;count<0x300;count+=2) { 
      t3 = *((unsigned short *)( &video_vidram[0x10000 + count] )); 
      t1 = *((unsigned short *)( &video_vidram[0x10400 + count] )); 
      t2 = *((unsigned short *)( &video_vidram[0x10800 + count] )); 

      // If this bit is set this new column is placed next to last one 
      if (t1 & 0x40) { 
         sx += (rzx + 1); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Get new zoom for this column 
         zx = (t3 >> 8)&0x0F; 

         sy = oy; 
      } else {   // nope it is a new block 
         // Sprite scaling 
         zx = (t3 >> 8)&0x0F; 

         rzy = t3 & 0xff; 

         sx = (t2 >> 7); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Number of tiles in this strip 
         my = t1 & 0x3f; 
         if (my == 0x20) 
            fullmode = 1; 
         else if (my >= 0x21) 
            fullmode = 2;   // most games use 0x21, but 
         else 
            fullmode = 0;   // Alpha Mission II uses 0x3f 

         sy = 0x1F0 - (t1 >> 7); 
         if (sy > 0x100) sy -= 0x200; 
          
         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            while (sy < -16) sy += 2 * (rzy + 1); 
         } 
         oy = sy; 

           if(my==0x21) my=0x20; 
         else if(rzy!=0xff && my!=0) 
            my=((my*16*256)/(rzy+1) + 15)/16; 

                  if(my>0x20) my=0x20; 

         ddax=0;   // setup x zoom 
      } 

      rzx = zx; 

      // No point doing anything if tile strip is 0 
      if ((my==0)||(sx>311)) 
         continue; 

      // Setup y zoom 
      if(rzy==255) 
         yskip=16; 
      else 
         dday=0;   // =256; NS990105 mslug fix 

      offs = count<<6; 

      // my holds the number of tiles in each vertical multisprite block 
      for (y=0; y < my ;y++) { 
         tileno  = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 
         tileatr = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 

         if (tileatr&0x8) 
            tileno = (tileno&~7)|(neogeo_frame_counter&7); 
         else if (tileatr&0x4) 
            tileno = (tileno&~3)|(neogeo_frame_counter&3); 
              
//         tileno &= 0x7FFF; 
         if (tileno>0x7FFF) 
            continue; 

         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            if (sy >= 248) sy -= 2 * (rzy + 1); 
         } 
         else if (fullmode == 1) 
         { 
            if (y == 0x10) sy -= 2 * (rzy + 1); 
         } 
         else if (sy > 0x110) sy -= 0x200; 

         if(rzy!=255) 
         { 
            yskip=0; 
            video_shrinky[0]=0; 
            for(i=0;i<16;i++) 
            { 
               video_shrinky[i+1]=0; 
               dday-=rzy+1; 
               if(dday<=0) 
               { 
                  dday+=256; 
                  yskip++; 
                  video_shrinky[yskip]++; 
               } 
               else 
                  video_shrinky[yskip]++; 
            } 
         } 

	 if (!neo4all_skip_next_frame)
         	if (((tileatr>>8)||(tileno!=0))&&(sy<224)) 
         	{ 
         		video_draw_spr( 
               			tileno,
				tileatr >> 8, 
               			tileatr & 0x01,tileatr & 0x02, 
               			sx,sy,rzx,yskip); 
         	}

         sy +=yskip; 
      }  // for y 
   }  // for count 

   if (fc >= neogeo_frame_counter_speed) { 
      neogeo_frame_counter++; 
      fc=3; 
   } 

   fc++; 
    
   if (!neo4all_skip_next_frame)
   {
   	video_draw_fix(); 
   	blitter();
   }
} 


//---------------------------------------------------------------------------- 
void video_draw_screen2() 
{ 
   static int      pass1_start; 
   int         sx =0,sy =0,oy =0,my =0,zx = 1, rzy = 1; 
   int         offs,i,count,y; 
   int         tileno,tileatr,t1,t2,t3; 
   char         fullmode=0; 
   int         ddax=0,dday=0,rzx=15,yskip=0; 

   if (!neo4all_skip_next_frame)
   	video_draw_screen_first();
 
   t1 = *((unsigned short *)( &video_vidram[0x10400 + 4] )); 

   if ((t1 & 0x40) == 0) 
   { 
      for (pass1_start=6;pass1_start<0x300;pass1_start+=2) 
      { 
         t1 = *((unsigned short *)( &video_vidram[0x10400 + pass1_start] )); 

         if ((t1 & 0x40) == 0) 
            break; 
      } 
        
      if (pass1_start == 6) 
         pass1_start = 0; 
   } 
   else 
      pass1_start = 0;    

   for (count=pass1_start;count<0x300;count+=2) { 
      t3 = *((unsigned short *)( &video_vidram[0x10000 + count] )); 
      t1 = *((unsigned short *)( &video_vidram[0x10400 + count] )); 
      t2 = *((unsigned short *)( &video_vidram[0x10800 + count] )); 

      // If this bit is set this new column is placed next to last one 
      if (t1 & 0x40) { 
         sx += (rzx + 1); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Get new zoom for this column 
         zx = (t3 >> 8)&0x0F; 

         sy = oy; 
      } else {   // nope it is a new block 
         // Sprite scaling 
         zx = (t3 >> 8)&0x0F; 

         rzy = t3 & 0xff; 

         sx = (t2 >> 7); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Number of tiles in this strip 
         my = t1 & 0x3f; 
         if (my == 0x20) 
            fullmode = 1; 
         else if (my >= 0x21) 
            fullmode = 2;   // most games use 0x21, but 
         else 
            fullmode = 0;   // Alpha Mission II uses 0x3f 

         sy = 0x1F0 - (t1 >> 7); 
         if (sy > 0x100) sy -= 0x200; 
          
         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            while (sy < -16) sy += 2 * (rzy + 1); 
         } 
         oy = sy; 

           if(my==0x21) my=0x20; 
         else if(rzy!=0xff && my!=0) 
            my=((my*16*256)/(rzy+1) + 15)/16; 

                  if(my>0x20) my=0x20; 

         ddax=0;   // setup x zoom 
      } 

      rzx = zx; 

      // No point doing anything if tile strip is 0 
      if ((my==0)||(sx>311)) 
         continue; 

      // Setup y zoom 
      if(rzy==255) 
         yskip=16; 
      else 
         dday=0;   // =256; NS990105 mslug fix 

      offs = count<<6; 

      // my holds the number of tiles in each vertical multisprite block 
      for (y=0; y < my ;y++) { 
         tileno  = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 
         tileatr = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 

         if (tileatr&0x8) 
            tileno = (tileno&~7)|(neogeo_frame_counter&7); 
         else if (tileatr&0x4) 
            tileno = (tileno&~3)|(neogeo_frame_counter&3); 
              
//         tileno &= 0x7FFF; 
         if (tileno>0x7FFF) 
            continue; 

         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            if (sy >= 224) sy -= 2 * (rzy + 1); 
         } 
         else if (fullmode == 1) 
         { 
            if (y == 0x10) sy -= 2 * (rzy + 1); 
         } 
         else if (sy > 0x100) sy -= 0x200; 

         if(rzy!=255) 
         { 
            yskip=0; 
            video_shrinky[0]=0; 
            for(i=0;i<16;i++) 
            { 
               video_shrinky[i+1]=0; 
               dday-=rzy+1; 
               if(dday<=0) 
               { 
                  dday+=256; 
                  yskip++; 
                  video_shrinky[yskip]++; 
               } 
               else 
                  video_shrinky[yskip]++; 
            } 
         } 

    	 if (!neo4all_skip_next_frame)
         	if (((tileatr>>8)||(tileno!=0))&&(sy<224)) 
         	{ 
         		video_draw_spr( 
               			tileno, 
               			tileatr >> 8, 
               			tileatr & 0x01,tileatr & 0x02, 
               			sx,sy,rzx,yskip); 
         	} 

         sy +=yskip; 
      }  // for y 
   }  // for count 

   for (count=0;count<pass1_start;count+=2) { 
      t3 = *((unsigned short *)( &video_vidram[0x10000 + count] )); 
      t1 = *((unsigned short *)( &video_vidram[0x10400 + count] )); 
      t2 = *((unsigned short *)( &video_vidram[0x10800 + count] )); 

      // If this bit is set this new column is placed next to last one 
      if (t1 & 0x40) { 
         sx += (rzx + 1); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Get new zoom for this column 
         zx = (t3 >> 8)&0x0F; 

         sy = oy; 
      } else {   // nope it is a new block 
         // Sprite scaling 
         zx = (t3 >> 8)&0x0F; 

         rzy = t3 & 0xff; 

         sx = (t2 >> 7); 
         if ( sx >= 0x1F0 ) 
            sx -= 0x200; 

         // Number of tiles in this strip 
         my = t1 & 0x3f; 
         if (my == 0x20) 
            fullmode = 1; 
         else if (my >= 0x21) 
            fullmode = 2;   // most games use 0x21, but 
         else 
            fullmode = 0;   // Alpha Mission II uses 0x3f 

         sy = 0x1F0 - (t1 >> 7); 
         if (sy > 0x100) sy -= 0x200; 
          
         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            while (sy < -16) sy += 2 * (rzy + 1); 
         } 
         oy = sy; 

           if(my==0x21) my=0x20; 
         else if(rzy!=0xff && my!=0) 
            my=((my*16*256)/(rzy+1) + 15)/16; 

                  if(my>0x20) my=0x20; 

         ddax=0;   // setup x zoom 
      } 

      rzx = zx; 

      // No point doing anything if tile strip is 0 
      if ((my==0)||(sx>311)) 
         continue; 

      // Setup y zoom 
      if(rzy==255) 
         yskip=16; 
      else 
         dday=0;   // =256; NS990105 mslug fix 

      offs = count<<6; 

      // my holds the number of tiles in each vertical multisprite block 
      for (y=0; y < my ;y++) { 
         tileno  = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 
         tileatr = *((unsigned short *)(&video_vidram[offs])); 
         offs+=2; 

         if (tileatr&0x8) 
            tileno = (tileno&~7)|(neogeo_frame_counter&7); 
         else if (tileatr&0x4) 
            tileno = (tileno&~3)|(neogeo_frame_counter&3); 
              
//         tileno &= 0x7FFF; 
         if (tileno>0x7FFF) 
            continue; 

         if (fullmode == 2 || (fullmode == 1 && rzy == 0xff)) 
         { 
            if (sy >= 248) sy -= 2 * (rzy + 1); 
         } 
         else if (fullmode == 1) 
         { 
            if (y == 0x10) sy -= 2 * (rzy + 1); 
         } 
         else if (sy > 0x110) sy -= 0x200; 

         if(rzy!=255) 
         { 
            yskip=0; 
            video_shrinky[0]=0; 
            for(i=0;i<16;i++) 
            { 
               video_shrinky[i+1]=0; 
               dday-=rzy+1; 
               if(dday<=0) 
               { 
                  dday+=256; 
                  yskip++; 
                  video_shrinky[yskip]++; 
               } 
               else 
                  video_shrinky[yskip]++; 
            } 
         } 

    	 if (!neo4all_skip_next_frame)
         	if (((tileatr>>8)||(tileno!=0))&&(sy<224)) 
         	{ 
         		video_draw_spr( 
         			tileno, 
         			tileatr >> 8, 
         			tileatr & 0x01,tileatr & 0x02, 
         			sx,sy,rzx,yskip); 
         	} 

         sy +=yskip; 
      }  // for y 
   }  // for count 

   if (fc >= neogeo_frame_counter_speed) { 
      neogeo_frame_counter++; 
      fc=3; 
   } 

   fc++; 
    
   if (!neo4all_skip_next_frame)
   {
   	video_draw_fix(); 
   	blitter(); 
   }
}

void video_flip(SDL_Surface *surface)
{
#ifndef USE_VIDEO_GL
	SDL_Flip(surface);
#else
  if (used_blitter)
  {
	unsigned a;
  	for(a=0;a<TCACHE_SIZE;a++)
		tile_cache[a].pal=-1;
	if (view_console)
		console_draw_all_background();
  }
#ifdef DREAMCAST
  glKosBeginFrame();
#endif
  glClearColor( 0.0,0.0,0.0, 255.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  extern GLint screen_texture;

  glBindTexture(GL_TEXTURE_2D,screen_texture);
  loadTextureParams();

#ifndef DREAMCAST
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, 
    GL_RGB, GL_UNSIGNED_SHORT_5_6_5, neo4all_texture_real_buffer+TCACHE_SIZE);
#else
  glKosTex2D(GL_RGB565,512,512,neo4all_texture_real_buffer+TCACHE_SIZE);
#endif
  glBegin(GL_QUADS);
  	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,0.0,1.5);
	
  	glTexCoord2f(1.0,0.0);
	glVertex3f(512.0,0.0,1.5);

  	glTexCoord2f(1.0,1.0);
	glVertex3f(512.0,512.0,1.5);

  	glTexCoord2f(0.0,1.0);
	glVertex3f(0.0,512.0,1.5);
  glEnd();

#ifndef DREAMCAST
  SDL_GL_SwapBuffers();
#else
    glKosFinishFrame();
#endif
  used_blitter=0;
#endif
}

