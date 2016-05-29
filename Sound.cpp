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
	if (sound == NULL) { std::cout << Mix_GetError() << std::endl; }
	Mix_AllocateChannels(16);
	Mix_VolumeChunk(sound,vol);
	int channel = Mix_PlayChannel(-1, sound, 0);
	if (channel == -1) { printf("%s",Mix_GetError()); }
	while (Mix_Playing(channel)) {SDL_Delay(100);}
}

void Sound::play(char* path, int vol)
{
	//Mix_PlayChannel(16,sound,0);
	thread t(processSound,path,vol);
	// No esperamos a que el hilo termine.
	t.detach();
}
