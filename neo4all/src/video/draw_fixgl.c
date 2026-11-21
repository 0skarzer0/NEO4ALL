#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "video.h"
#include "../neocd.h"
#include "videogl.h"
#include "sprgl.h"

#ifdef DREAMCAST
void draw_font(unsigned short *br, unsigned short *paldata, unsigned *gfxdata);
#else
static __inline__ void draw_font(unsigned short *br, unsigned short *paldata, unsigned *gfxdata)
{
    int y;
    for(y=0;y<8;y++) {
	register unsigned int myword = gfxdata[0];
	br[0]=paldata[(myword)&0xf];
	br[1]=paldata[(myword>>4)&0xf];
	br[2]=paldata[(myword>>8)&0xf];
	br[3]=paldata[(myword>>12)&0xf];
	br[4]=paldata[(myword>>16)&0xf];
	br[5]=paldata[(myword>>20)&0xf];
	br[6]=paldata[(myword>>24)&0xf];
	br[7]=paldata[(myword>>28)&0xf];
	
	br+=8;
	gfxdata++;
    }
}
#endif

static __inline__ void create_font(unsigned int tileno,int color,unsigned short *_br)
{
   unsigned short *br=(unsigned short*)neo4all_texture_buffer;
//printf("DRAW_TILE %i\n",tileno);
    unsigned short *paldata=(unsigned short *)&video_paletteram_pc[color];
    unsigned int *gfxdata = (unsigned int *)&neogeo_fix_memory[tileno<<5];

    paldata[0]=0; // BLEND !!!

    draw_font(br,paldata,gfxdata);
#ifdef DREAMCAST
    pvr_txr_load(neo4all_texture_buffer,_br,8*8*2);
#else
    memcpy(_br,neo4all_texture_buffer,8*8*2);
#endif
}

#ifdef DREAMCAST
extern pvr_poly_cxt_t gl_poly_cxt;
static pvr_poly_hdr_t polyhdr;
static pvr_dr_state_t  dr_state;

static __inline__ void prepara_pvr(void *texture_mem)
{
	gl_poly_cxt.txr.filter= PVR_FILTER_NONE;
	gl_poly_cxt.gen.alpha = PVR_ALPHA_DISABLE;
	gl_poly_cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
	gl_poly_cxt.blend.src = PVR_BLEND_SRCALPHA; //PVR_BLEND_ONE;
	gl_poly_cxt.blend.dst = PVR_BLEND_INVSRCALPHA; //PVR_BLEND_ZERO;
	gl_poly_cxt.gen.culling = PVR_CULLING_NONE;
	gl_poly_cxt.txr.width = 8;
	gl_poly_cxt.txr.height = 8;
	gl_poly_cxt.txr.format = GL_ARGB1555;
	gl_poly_cxt.txr.base = texture_mem;
	pvr_poly_compile(&polyhdr, &gl_poly_cxt);
	pvr_prim(&polyhdr, sizeof(pvr_poly_hdr_t));
	pvr_dr_init(dr_state);
}

#endif



//---------------------------------------------------------------------------- 
void   video_draw_font(unsigned int code, unsigned int color, int sx, int sy)
{
//printf("font sx=%i sy=%i\n",sx,sy);

   void *font_buffer;
   unsigned int sel=code&FCACHE_MASK;
//printf("CODE=%i, SEL=%i, COL=%i\n",code,sel,color);

#ifndef DREAMCAST
   glBindTexture(GL_TEXTURE_2D,font_cache[sel].tex);
   loadTextureParams();
#endif

   if (font_cache[sel].tileno!=code || font_cache[sel].pal!=color)
   {
	   if (font_l2_cache[sel])
	   {
		   if (font_l2_ptr>FCACHE_L2_MASK)
				return;
		   else
		   	font_buffer=neo4all_font_l2_buffer+(font_l2_ptr*8*8*2);
		   font_l2_ptr++;
	   }
	   else
	   {
		   font_buffer=font_cache[sel].buffer;
		   font_l2_cache[sel]=1;
	   	   font_cache[sel].tileno=code;
	    	   font_cache[sel].pal=color;
	   }
	   create_font(code,color,font_buffer);
   }
   else
   {
	   font_l2_cache[sel]=1;
	   font_buffer=font_cache[sel].buffer;
   }


#ifndef DREAMCAST
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 8, 8, 0, 
	     GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, font_buffer);
#else
//    glKosTex2D(GL_RGB565,8,8,font_buffer);

    prepara_pvr(font_buffer);
#endif

#ifndef DREAMCAST
   glBegin(GL_QUADS);
#endif
/*
   if (!flipx)
   {
	   if (flipy)
	   {
		   VERTICE_FLIP_XY
	   }
	   else
	   {
		   VERTICE_FLIP_X
	   }
   }
   else
   {
	   if (flipy)
	   {
		   VERTICE_FLIP_Y
	   }
	   else
	   {
*/
int zx=8;
int zy=8;
		   VERTICE_FLIP_NONE
/*
	   }
   }
*/
#ifndef DREAMCAST
   glEnd();
   tile_z+=0.0000001;
#else
   tile_z+=1.1f;
#endif
}


void video_draw_fix(void)
{
	Uint16 x, y;
	Uint16 code, colour;
	Uint16 * fixarea=(Uint16 *)&video_vidram[0xe004];

	for (y=0; y < 28; y++)
	{
		for (x = 0; x < 40; x++)
		{
			code = fixarea[x << 5];
			colour = (code&0xf000)>>8;
			code  &= 0xfff;

			if (video_fix_usage[code])
				video_draw_font(code,colour,(x<<3),(y<<3));
		}
		fixarea++;
	}
}
