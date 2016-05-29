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
		// Listado con las diferentes constantes que representan
		// los diversos sonidos del juego.
		char * SONIDO_PATEANDO = "./Sounds/patear.wav";
		char * SONIDO_TRUCO = "./Sounds/truco.wav";
		char * SONIDO_APLAUSO = "./Sounds/aplausos.wav";
		char * SONIDO_PLUS = "./Sounds/plus.wav";
		char * SONIDO_IMPACTO = "./Sounds/impacto.wav";
		char * SONIDO_DESCONECTADO = "./Sounds/desconectado.wav";
		char * SONIDO_CONECTADO = "./Sounds/conectado.wav";
		// Emite el sonido que obtenemos del path.
		void play(char* path, int vol);
};

#endif
