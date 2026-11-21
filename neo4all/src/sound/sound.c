/*  gngeo a neogeo emulator
 *  Copyright (C) 2001 Peponas Mathieu
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

  
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../neocd.h"
#include "streams.h"

static unsigned char sound_muted=0x3;
static SDL_AudioSpec desired;

#include "sound.h"

#define MIXER_MAX_CHANNELS 16

//#define CPU_FPS 60
#define BUFFER_LEN 16384
extern Uint16 play_buffer[BUFFER_LEN];

#define NB_SAMPLES 512 /* better resolution */
//#define NB_SAMPLES 1024

void update_sdl_stream(void *userdata, Uint8 * stream, int len)
{
#ifdef Z80_EMULATED
    if (!sound_muted)
    {
    	streamupdate(len);
    	memcpy(stream, (Uint8 *) play_buffer, len);
    }
#endif
}

int init_sdl_audio(void)
{
#ifdef Z80_EMULATED
    desired.freq = SAMPLE_RATE;
    desired.samples = NB_SAMPLES;
    
#ifdef WORDS_BIGENDIAN
    desired.format = AUDIO_S16MSB;
#else	
    desired.format = AUDIO_S16;
#endif	
    desired.channels = 2;
    desired.callback = update_sdl_stream;
    desired.userdata = NULL;
    SDL_OpenAudio(&desired, NULL);
    SDL_PauseAudio(1);
    
    sound_reset();
#endif    
    return 1;
}

void sound_enable(void) {
#ifdef Z80_EMULATED
	if ((sound_muted&0x1)&&(!(sound_muted&0x2)))
		SDL_PauseAudio(0);
	sound_muted&=0xFE;
#endif
}

void sound_disable(void) {
#ifdef Z80_EMULATED
	if (!(sound_muted&0x3))
		SDL_PauseAudio(1);
	sound_muted|=0x1;
#endif
}

void sound_toggle(void) {
#ifdef Z80_EMULATED
	if (!(sound_muted&0x2))
	{
		if (sound_muted&0x1)
			sound_enable();
		else
			sound_disable();
	}
#endif
}

void sound_shutdown(void) {
#ifdef Z80_EMULATED
    sound_stop();
    SDL_CloseAudio();
#endif
}

void sound_stop(void) {
#ifdef Z80_EMULATED
    sound_mute();
    streams_sh_stop();
    YM2610_sh_stop();
    SDL_Delay(100);
#endif
}

void sound_reset(void) {
#ifdef Z80_EMULATED
    streams_sh_start();
    YM2610_sh_start();
    sound_unmute();
#endif
}

void sound_mute(void){
#ifdef Z80_EMULATED
	if (!(sound_muted&0x2))
		SDL_PauseAudio(1);
	sound_muted=0x3;
#endif
}

void sound_unmute(void){
#ifdef Z80_EMULATED
	if (sound_muted&0x2)
		SDL_PauseAudio(0);
	sound_muted=0;
#endif
}
