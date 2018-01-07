/*************************************************************************************/
/*************************************************************************************/
/**                                                                                 **/
/**  Tinyplayer - TibV2 example                                                     **/
/**  written by Tammo 'kb' Hinrichs 2000-2008                                       **/
/**  This file is in the public domain                                              **/
/**  "Patient Zero" is (C) Melwyn+LB 2005, do not redistribute                      **/
/**                                                                                 **/
/*************************************************************************************/
/*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "v2mplayer.h"
#include "libv2.h"
#include "v2mconv.h"
#include "sounddef.h"

static V2MPlayer player;
static SDL_AudioDeviceID dev;

////////////////////////////////////////////////////////////////////////////////////////////////////
static void sdl_callback(void *userdata, Uint8 * stream, int len) {
    player.Render((float*) stream, len / 8);
}

static bool init_sdl() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s\n",SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_AudioSpec desired, actual;
    desired.channels = 2;
    desired.freq = 44100;
    desired.samples = 4096;
    desired.format = AUDIO_F32;
    desired.callback = sdl_callback;

    dev = SDL_OpenAudioDevice(NULL, 0, &desired, &actual, 0);
    if (! dev) {
        SDL_Log("Failed to open audio, %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static unsigned char* check_and_convert(unsigned char* tune, int length) {
    sdInit();

    int version = CheckV2MVersion(tune, length);
    if (version < 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_INPUT, "Failed to Check Version on input file");
        return NULL;
    }

    uint8_t* converted;
    int converted_length;
    ConvertV2M(tune, length, &converted, &converted_length);
    free(tune);

    return converted;
}

int main(int argc, const char** argv) {
    int fn;
    int err=0;
    printf("Farbrausch Tiny Music Player v0.dontcare TWO\n");
    printf("Code and Synthesizer (C) 2000-2008 kb/Farbrausch\n");
    printf("SDL Port by github.com/jgilje\n\n");

    if (argc <= 1) {
        printf("usage: %s V2M_file\n", argv[0]);
        return 1;
    }

  for(fn=1; fn<argc; fn++)
  {
    FILE* file = fopen(argv[fn], "r");
    if (file == NULL) {
        printf("Failed to open %s\n", argv[fn]);
        err|=1;
        continue;
    }

    fseek(file, 0, SEEK_END);
    int64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char* theTune = (unsigned char*) calloc(1, size);

    size_t read = fread(theTune, 1, size, file);
    fclose(file);
    if (size != read) {
        fprintf(stderr, "Invalid read size\n");
        err|=1;
        continue;
    }

    theTune = check_and_convert(theTune, size);

    player.Init();
    player.Open(theTune);

    printf("Now Playing: %s\n", argv[fn]);

    if (! init_sdl()) {
        err|=1;
        break;
    }

    player.Play();
    SDL_PauseAudioDevice(dev, 0);

#if 0
//    printf("\n\npress Enter to quit\n");
//    getc(stdin);
#else
     while(player.IsPlaying()) { sleep(1); }
#endif

    SDL_PauseAudioDevice(dev, 1);
    player.Close();
    free(theTune);
  }
    SDL_Quit();
    return err;
}
