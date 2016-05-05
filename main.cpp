
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Escenario.h"



using namespace std;
int main( int argc, char* args[] )
{

	bool quit = false;

	while (!quit){
		int width= 500;
		int height = 500;
		//crea una ventana y muestra una imagen
		Escenario* mapa = new Escenario(width,height);
		int x = 200;
		int y = 500;
		//cargo varios backround

		mapa->insertBackgroundObject("enfermera", 0,0,84,height);
		mapa->insertBackgroundObject("sprites/pelota.png", x,y,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 200,2000,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 150,700,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 100,100,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 450,657,40,height);
		mapa->insertBackgroundObject("sprites/pelota.png", 300,500,40,height);

		/*mapa->insertBackgroundObject("sprites/pelota.png", 100,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 150,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 200,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 250,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 300,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 350,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 400,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 450,100);
		mapa->insertBackgroundObject("sprites/pelota.png", 500,100);*/

		quit = mapa->lunchScreen();
	}



	return 0;
}
