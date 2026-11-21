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
void draw_tile(unsigned short *br, unsigned short *paldata, unsigned *gfxdata);
#else
static __inline__ void draw_tile(unsigned short *br, unsigned short *paldata, unsigned *gfxdata)
{
    int y;
    for(y=0;y<16;y++) {
	register unsigned int myword = gfxdata[1];
	br[0]=paldata[(myword>>28)&0xf];
	br[1]=paldata[(myword>>24)&0xf];
	br[2]=paldata[(myword>>20)&0xf];
	br[3]=paldata[(myword>>16)&0xf];
	br[4]=paldata[(myword>>12)&0xf];
	br[5]=paldata[(myword>>8)&0xf];
	br[6]=paldata[(myword>>4)&0xf];
	br[7]=paldata[(myword)&0xf];
	
	myword = gfxdata[0];
	br[8]=paldata[(myword>>28)&0xf];
	br[9]=paldata[(myword>>24)&0xf];
	br[10]=paldata[(myword>>20)&0xf];
	br[11]=paldata[(myword>>16)&0xf];
	br[12]=paldata[(myword>>12)&0xf];
	br[13]=paldata[(myword>>8)&0xf];
	br[14]=paldata[(myword>>4)&0xf];
	br[15]=paldata[(myword)&0xf];
	br+=16;
	gfxdata+=2;
    }
}
#endif

static __inline__ void create_tile(unsigned int tileno,int color,unsigned short *_br)
{
   unsigned short *br=(unsigned short*)neo4all_texture_buffer;
//printf("DRAW_TILE %i\n",tileno);
    unsigned short *paldata=(unsigned short *)&video_paletteram_pc[color<<4];
    unsigned int *gfxdata = (unsigned int *)&neogeo_spr_memory[tileno<<7];

    paldata[0]=0; // BLEND !!!

    draw_tile(br,paldata,gfxdata);
#ifdef DREAMCAST
    pvr_txr_load(neo4all_texture_buffer,_br,16*16*2);
#else
    memcpy(_br,neo4all_texture_buffer,16*16*2);
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
	gl_poly_cxt.txr.width = 16;
	gl_poly_cxt.txr.height = 16;
	gl_poly_cxt.txr.format = GL_ARGB1555;
	gl_poly_cxt.txr.base = texture_mem;
	pvr_poly_compile(&polyhdr, &gl_poly_cxt);
	pvr_prim(&polyhdr, sizeof(pvr_poly_hdr_t));
	pvr_dr_init(dr_state);
}

#endif



//---------------------------------------------------------------------------- 
void   video_draw_spr(unsigned int code, unsigned int color, int flipx, 
         int flipy, int sx, int sy, int zx, int zy) 
{
//ntiles++;
//printf("spr fx=%i fy=%i sx=%i sy=%i zx=%i zy=%i\n",flipx,flipy,sx,sy,zx,zy);
  if (zx<=0 || zy<=0)
	  return;
  zx++;
//  if (zx==15) zx=16;

   void *texture_buffer;
   unsigned int sel=code&TCACHE_MASK;
//printf("CODE=%i, SEL=%i, COL=%i\n",code,sel,color);

#ifndef DREAMCAST
   glBindTexture(GL_TEXTURE_2D,tile_cache[sel].tex);
   loadTextureParams();
#endif

   if (tile_cache[sel].tileno!=code || tile_cache[sel].pal!=color)
   {
	   if (tile_l2_cache[sel])
	   {
		   if (tile_l2_ptr>TCACHE_L2_MASK)
		   {
			register unsigned l3=tile_l2_ptr-TCACHE_L2_SIZE;
			if (l3>TCACHE_L3_MASK)
				return;
		   	texture_buffer=neo4all_texture_l3_buffer+(l3*16*16*2);
		   }
		   else
		   	texture_buffer=neo4all_texture_l2_buffer+(tile_l2_ptr*16*16*2);
		   tile_l2_ptr++;
	   }
	   else
	   {
		   texture_buffer=tile_cache[sel].buffer;
		   tile_l2_cache[sel]=1;
	   	   tile_cache[sel].tileno=code;
	    	   tile_cache[sel].pal=color;
	   }
	   create_tile(code,color,texture_buffer);
   }
   else
   {
	   tile_l2_cache[sel]=1;
	   texture_buffer=tile_cache[sel].buffer;
   }

#ifndef DREAMCAST
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, 
	     GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, texture_buffer);
#else
//    glKosTex2D(GL_RGB565,16,16,texture_buffer);

    prepara_pvr(texture_buffer);
#endif

#ifndef DREAMCAST
   glBegin(GL_QUADS);
#endif
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
		   VERTICE_FLIP_NONE
	   }
   }
#ifndef DREAMCAST
   glEnd();
   tile_z+=0.0000001;
#else
   tile_z+=1.1f;
#endif
}

