#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "video.h"
#include "../neocd.h"
#include "videogl.h"

static SDL_Surface *gl_screen;

tcache_node_t *tcache_hash_table[TCACHE_HASH_SIZE];
tcache_node_t cache_tile[TCACHE_SIZE];
tcache_node_t *first_tile, *last_tile;

fcache_node_t *fcache_hash_table[FCACHE_HASH_SIZE];
fcache_node_t cache_font[FCACHE_SIZE];
fcache_node_t *first_font, *last_font;

unsigned neo4all_filter=NEO4ALL_FILTER_NONE;

TILE_LIST tile_list[TCACHE_SIZE+FCACHE_SIZE];
unsigned n_tile_list=0;
unsigned n_font_list=0;

#ifndef DREAMCAST
GLint tile_opengl_tex[TCACHE_SIZE+FCACHE_SIZE];
#endif

GLint screen_texture;

unsigned ntiles=0;
unsigned tiles_fail=0;

void *neo4all_texture_buffer=NULL;
void *neo4all_texture_buffer_free=NULL;
void *neo4all_texture_real_buffer=NULL;
void *neo4all_font_real_buffer=NULL;
float tile_z=1.0;
unsigned neo4all_glframes=8;

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
	neo4all_font_real_buffer=pvr_mem_malloc(8*8*2*FCACHE_SIZE);
*/
	{
		unsigned dcpvrmem=(unsigned)pvr_mem_malloc( 
			(16*16*2*(TCACHE_SIZE))+
			(8*8*2*(FCACHE_SIZE))
				);
		neo4all_texture_real_buffer=(void *)dcpvrmem;
		dcpvrmem+=(16*16*2*TCACHE_SIZE);
		neo4all_font_real_buffer=(void *)dcpvrmem;
	}
#else
	neo4all_texture_real_buffer=calloc(16*16*2,TCACHE_SIZE);
	neo4all_font_real_buffer=calloc(8*8*2,FCACHE_SIZE);
#endif
    }


#ifndef DREAMCAST
    for(i=0;i<TCACHE_SIZE+FCACHE_SIZE;i++)
	glGenTextures(1,&tile_opengl_tex);
#endif
    video_reset_gl();
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
    glOrtho(0.0, 320.0, 240.0, 0.0, -50.0, 50.0 );
//    glOrtho(16.0, 300.0, 224.0, 0.0, -50.0, 50.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    init_cache();
    blitter();
    screen = SDL_CreateRGBSurfaceFrom(neo4all_texture_real_buffer, 320, 240, 16, 1024, 0xF800, 0x7E0, 0x1F, 0);
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


void video_reset_gl(void)
{
    unsigned i;
    tcache_hash_init();
    for(i=0;i<TCACHE_SIZE;i++)
	    cache_tile[i].rec=(unsigned short *)(((unsigned)neo4all_texture_real_buffer)+i*16*16*2);
    tcache_hash_init();

    fcache_hash_init();
    for(i=0;i<FCACHE_SIZE;i++)
	    cache_font[i].rec=(unsigned short *)(((unsigned)neo4all_font_real_buffer)+i*8*8*2);
    fcache_hash_init();
    n_tile_list=0;
    n_font_list=0;
}

void neogeo_adjust_filter(int filter)
{
	if (filter)
		neo4all_filter=NEO4ALL_FILTER_BILINEAR;
	else
		neo4all_filter=NEO4ALL_FILTER_NONE;
}
