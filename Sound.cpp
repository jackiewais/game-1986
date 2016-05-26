#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Sound.h"

using namespace std;

void processSound(char* path, int vol)
{
	// "./Sounds/aplausos.wav"
	Mix_Chunk* sound = Mix_LoadWAV(path);
	//Mix_AllocateChannels(16);
	Mix_VolumeChunk(sound,vol);
	int channel = Mix_PlayChannel(-1, sound, 0);
	if (channel == -1) { printf("%s",Mix_GetError()); }
	while (Mix_Playing(channel)) {SDL_Delay(100);}
	Mix_CloseAudio();
	SDL_Quit();
}

void Sound::play(char* path, int vol)
{
	if( SDL_Init(SDL_INIT_AUDIO) < 0 ) exit(1);
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024) == -1)
	{ printf("%s",Mix_GetError()); }
	//Mix_PlayChannel(16,sound,0);
	std::thread t(processSound,path,vol);
	// No esperamos a que el hilo termine.
	t.detach();
}
