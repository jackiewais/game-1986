
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Escenario.h"



using namespace std;
int main( int argc, char* args[] )
{
int width= 500;
int height = 596;
//crea una ventana y muestra una imagen
Escenario* mapa = new Escenario(width,height);
int x = 500;
int y = 500;
mapa->insertBackgroundObject("sprites/pelota.png", x,y);
mapa->lunchScreen();





	return 0;
}
