#include "audio.h"
#include <SDL2/SDL.h>

static CHIP8 *g_system_for_audio = NULL; // audio callback

void set_audio_system(CHIP8 *C8) {
    g_system_for_audio = C8;
}

// Simple audio callback
void audio_callback(void *userdata, Uint8 *stream, int len) {
    (void)userdata;
    if (!g_system_for_audio || g_system_for_audio->SOUND_TIMER == 0) {
        
        SDL_memset(stream, 0, len);
        return;
    }

    // 16-bit
    int samples = len / 2;
    static int sample_pos = 0;
    const int sample_rate = 48000;
    const int freq = 880; // beep frequency
    const int period = sample_rate / freq;
    int16_t *samples_out = (int16_t *)stream;
    for (int i = 0; i < samples; i++) {
        int16_t val = (sample_pos % period) < (period / 2) ? 3000 : -3000;
        samples_out[i] = val;
        sample_pos++;
    }
}