#ifndef LOGIN_H_
#define LOGIN_H_

#include <iostream>
#include <string>
#include <list>
#include <SDL2/SDL.h>

using namespace std;

class Login
{
	public:
		// Procesamos la creación de una ventana
		// a modo de login para obtener los datos.
		void ventanaLogin(char* ipChar, int& portNumber, string& name);
};

#endif
