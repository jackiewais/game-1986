
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
mapa->lunchScreen();




	return 0;
}
