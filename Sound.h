#ifndef SOUND_H_
#define SOUND_H_

#include <iostream>
#include <string>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;

class Sound
{
	public:

		// Emite el sonido que obtenemos del path.
		void play(char* path, int vol);
};

#endif
