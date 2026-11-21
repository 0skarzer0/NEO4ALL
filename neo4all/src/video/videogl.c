#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "video.h"
#include "../neocd.h"
#include "videogl.h"

static SDL_Surface *gl_screen;

TILE_CACHE tile_cache[TCACHE_SIZE];

unsigned char tile_used[TCACHE_SIZE];

TILE_CACHE font_cache[FCACHE_SIZE];
unsigned char font_used[FCACHE_SIZE];

GLint screen_texture;

void *neo4all_texture_buffer=NULL;
void *neo4all_texture_buffer_free=NULL;
void *neo4all_texture_real_buffer=NULL;
void *neo4all_texture_l2_buffer=NULL;
void *neo4all_font_real_buffer=NULL;
void *neo4all_font_l2_buffer=NULL;
unsigned tile_l2_ptr=0;
unsigned font_l2_ptr=0;
float tile_z=1.0;

static __inline__ void init_cache(void) {
    int i;
    GLuint texture;

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);


    if (neo4all_texture_buffer==NULL)
    {
	neo4all_texture_buffer_free=calloc(64+(16*16*2),1);
	neo4all_texture_buffer=(void *)(((((unsigned)neo4all_texture_buffer_free)+32)/32)*32);
#ifdef DREAMCAST
/*
	neo4all_texture_real_buffer=pvr_mem_malloc(16*16*2*TCACHE_SIZE);
	neo4all_texture_l2_buffer=pvr_mem_malloc(16*16*2*TCACHE_L2_SIZE);
	neo4all_font_real_buffer=pvr_mem_malloc(8*8*2*FCACHE_SIZE);
	neo4all_font_l2_buffer=pvr_mem_malloc(8*8*2*FCACHE_L2_SIZE);
*/
	{
		unsigned dcpvrmem=(unsigned)pvr_mem_malloc(
			(16*16*2*(TCACHE_SIZE+TCACHE_L2_SIZE))+
			(8*8*2*(FCACHE_SIZE+FCACHE_L2_SIZE))
				);
		neo4all_texture_real_buffer=(void *)dcpvrmem;
		dcpvrmem+=(16*16*2*TCACHE_SIZE);
		neo4all_texture_l2_buffer=(void *)dcpvrmem;
		dcpvrmem+=(16*16*2*TCACHE_L2_SIZE);
		neo4all_font_real_buffer=(void *)dcpvrmem;
		dcpvrmem+=(8*8*2*FCACHE_SIZE);
		neo4all_font_l2_buffer=(void *)dcpvrmem;
	}
#else
	neo4all_texture_real_buffer=calloc(16*16*2,TCACHE_SIZE);
	neo4all_texture_l2_buffer=calloc(16*16*2,TCACHE_L2_SIZE);
	neo4all_font_real_buffer=calloc(8*8*2,FCACHE_SIZE);
	neo4all_font_l2_buffer=calloc(8*8*2,FCACHE_L2_SIZE);
#endif
    }

    for(i=0;i<TCACHE_SIZE;i++) {
	tile_cache[i].tileno=-1;
#ifdef DREAMCAST
	if (!i)
#endif
		glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	tile_cache[i].tex=texture;
	tile_cache[i].pal=-1;
	tile_cache[i].buffer=(unsigned short *)(((unsigned)neo4all_texture_real_buffer)+i*16*16*2);
    }
    for(i=0;i<FCACHE_SIZE;i++) {
	font_cache[i].tileno=-1;
#ifdef DREAMCAST
	if (!i)
		glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
#endif
	font_cache[i].tex=texture;
	font_cache[i].pal=-1;
	font_cache[i].buffer=(unsigned short *)(((unsigned)neo4all_font_real_buffer)+i*8*8*2);
    }
}

static __inline__ void free_cache(void) {
#ifndef DREAMCAST
    free(neo4all_texture_real_buffer);
#else
    pvr_mem_free(neo4all_texture_real_buffer);
#endif
    free(neo4all_texture_buffer_free);
    neo4all_texture_buffer=NULL;
}

SDL_bool init_video_gl(void) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 1);
    gl_screen = SDL_SetVideoMode(VIDEO_GL_WIDTH, VIDEO_GL_HEIGHT, 16,
		    SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE | SDL_OPENGL);
	
    if ( gl_screen == NULL)
	return SDL_FALSE;

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glClearDepth(1.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, VIDEO_GL_WIDTH, VIDEO_GL_HEIGHT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0, 320.0, 240.0, 0.0, -5.0, 5.0 );
//    glOrtho(16.0, 300.0, 224.0, 0.0, -5.0, 5.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    init_cache();
    blitter();
    screen = SDL_CreateRGBSurfaceFrom(neo4all_texture_real_buffer+TCACHE_SIZE, 320, 240, 16, 1024, 0xF800, 0x7E0, 0x1F, 0);
    glGenTextures(1,&screen_texture);
    glBindTexture(GL_TEXTURE_2D,screen_texture);
    loadTextureParams();
#ifdef DREAMCAST
    glKosFinishFrame();
#endif
    return SDL_TRUE;
}

void video_fullscreen_toggle_gl(void)
{
#ifndef DREAMCAST
	SDL_WM_ToggleFullScreen(gl_screen);
#endif

}
