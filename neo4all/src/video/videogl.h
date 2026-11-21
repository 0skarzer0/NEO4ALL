#ifndef _VIDEOGL_H_
#define _VIDEOGL_H_

#include "SDL.h"
#include "SDL_opengl.h"

#define TCACHE_SIZE 0x1000
#define TCACHE_MASK (TCACHE_SIZE - 1)

#define TCACHE_L2_SIZE 0x800
#define TCACHE_L2_MASK (TCACHE_L2_SIZE - 1)

#define TCACHE_L3_SIZE 0x800
#define TCACHE_L3_MASK (TCACHE_L3_SIZE - 1)

#define FCACHE_SIZE 0x400
#define FCACHE_MASK (FCACHE_SIZE - 1)

#define FCACHE_L2_SIZE 0x800
#define FCACHE_L2_MASK (FCACHE_L2_SIZE - 1)

#ifdef DREAMCAST
#define VIDEO_GL_WIDTH 320
#define VIDEO_GL_HEIGHT 240
#else
#define VIDEO_GL_WIDTH 800
#define VIDEO_GL_HEIGHT 600
#endif

typedef struct TILE_CACHE{
    Uint32 tileno;
    GLuint tex;
    short pal;
    unsigned short *buffer;
}TILE_CACHE;

extern TILE_CACHE tile_cache[TCACHE_SIZE];
extern unsigned char tile_l2_cache[TCACHE_SIZE];
extern unsigned tile_l2_ptr;

extern TILE_CACHE font_cache[FCACHE_SIZE];
extern unsigned char font_l2_cache[FCACHE_SIZE];
extern unsigned font_l2_ptr;

extern float tile_z;

SDL_bool init_video_gl(void);

#ifndef DREAMCAST
#define NEO4ALL_GL_FILTER GL_LINEAR
//#define NEO4ALL_GL_FILTER GL_NEAREST
#else
//#define NEO4ALL_GL_FILTER GL_FILTER_BILINEAR
#define NEO4ALL_GL_FILTER GL_FILTER_NONE
#endif


extern void *neo4all_texture_buffer;
extern void *neo4all_texture_real_buffer;
extern void *neo4all_texture_l2_buffer;
extern void *neo4all_texture_l3_buffer;
extern unsigned tile_l2_ptr;

extern void *neo4all_font_real_buffer;
extern void *neo4all_font_l2_buffer;

static __inline__ void loadTextureParams(void)
{
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, 16);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, NEO4ALL_GL_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, NEO4ALL_GL_FILTER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE, GL_REPLACE);
}


#endif
