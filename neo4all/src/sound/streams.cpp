/***************************************************************************

  streams.c

  Handle general purpose audio streams

***************************************************************************/
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include "../neocd.h"

#define MIXER_MAX_CHANNELS 16
#define BUFFER_LEN 16384

#define CPU_FPS 60

extern int frame;

Uint16 play_buffer[BUFFER_LEN];
static Sint16 *tmp_sound_buf[MIXER_MAX_CHANNELS];
static Uint16 left_right_buffer[BUFFER_LEN*2];
static Uint16 *left_buffer=&left_right_buffer[0];
static Uint16 *right_buffer=&left_right_buffer[BUFFER_LEN];




static int SamplePan[] = { 0, 255, 128, 0, 255 };


static int stream_joined_channels[MIXER_MAX_CHANNELS];

static int stream_vol[MIXER_MAX_CHANNELS];

struct {
    Sint16 *buffer;
    int param;
    void (*callback) (int param, Sint16 ** buffer, int length);
} stream[MIXER_MAX_CHANNELS];

void mixer_set_volume(int channel,int volume) {
    stream_vol[channel]=volume;
}

int streams_sh_start(void)
{
    int i;

    for (i = 0; i < MIXER_MAX_CHANNELS; i++) {
	stream_joined_channels[i] = 1;
	stream[i].buffer = 0;
    }
    return 0;
}


void streams_sh_stop(void)
{
    int i;

    for (i = 0; i < MIXER_MAX_CHANNELS; i++) {
	if (stream[i].buffer)
		free(stream[i].buffer);
	stream[i].buffer = 0;
    }
}

#define MAX_AUDIOVAL ((1<<(16-1))-1)
#define MIN_AUDIOVAL -(1<<(16-1))

static __inline__ void mixaudio(Sint16 *dst, const Sint16 *src, Uint32 len)
{
	register unsigned l=len;
	while ( l-- ) {
		register int dst_sample=(*src++)+(*dst);
		if (dst_sample>MAX_AUDIOVAL)
			dst_sample=MAX_AUDIOVAL;
		else if (dst_sample<MIN_AUDIOVAL)
			dst_sample=MIN_AUDIOVAL;
		*dst++=dst_sample;
	}
}

void streamupdate(int len)
{
    /*static int current_pos;*/
    int channel;
    int buflen = len >> 2;

    if (buflen<=0)
	return;

    /* update all the output buffers */
    bzero(left_buffer, len);
    bzero(right_buffer, len);

    for (channel = 0; channel < MIXER_MAX_CHANNELS;
	 channel += stream_joined_channels[channel]) {
	if (stream[channel].buffer) {

	    if (stream_joined_channels[channel] > 1) {
		    {
		    	register int i;
		    	register Sint16 **buf=tmp_sound_buf;
			register int max=stream_joined_channels[channel];
		    	for (i = 0; i < max; i++)
				*buf++ = stream[channel + i].buffer;
		    }

		    (*stream[channel].callback) (stream[channel].param,
						 tmp_sound_buf, buflen);
	    }
	}
    }

    for (channel = 0; channel < MIXER_MAX_CHANNELS;
	 channel += stream_joined_channels[channel]) {

	if (stream[channel].buffer) {
	    register int i;
	    register Uint16 *bl=left_buffer;
	    register Uint16 *br=right_buffer;
	    register int max=stream_joined_channels[channel];
	    for (i = 0; i < max; i++) {

		if (SamplePan[channel + i] <= 128)
		    mixaudio((Sint16 *)bl, (Sint16 *)stream[channel + i].buffer, buflen);
		if (SamplePan[channel + i] >= 128)
		    mixaudio((Sint16 *)br, (Sint16 *)stream[channel + i].buffer, buflen);

	    }
	}
    }
#ifndef DREAMCAST
    SDL_LockAudio();
#endif
    {
    	register Uint16 *pl = play_buffer;
	register Uint16 *bl = left_buffer;
	register Uint16 *br = right_buffer;
	register int i;
	register int max = len >> 2;
    	for (i = 0; i < max; ++i) {
		*pl++ = *bl++;
		*pl++ = *br++;
    	}
    }
#ifndef DREAMCAST
    SDL_UnlockAudio();
#endif
}

int stream_init_multi(int channels, int param,
		      void (*callback) (int param, Sint16 ** buffer,
					int length))
{
    static int channel, i;

    stream_joined_channels[channel] = channels;

    for (i = 0; i < channels; i++) {

	if ((stream[channel + i].buffer = (Sint16 *)
	     malloc(sizeof(Sint16) * BUFFER_LEN)) == 0)
	    return -1;
    }

    stream[channel].param = param;
    stream[channel].callback = callback;
    channel += channels;
    return channel - channels;
}
