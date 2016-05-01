
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Escenario.h"



using namespace std;
int main( int argc, char* args[] )
{
int width= 600;
int height = 596;
//crea una ventana y muestra una imagen
Escenario* mapa = new Escenario(width,height);
int x = 200;
int y = 500;
//cargo varios backround 
mapa->insertBackgroundObject("sprites/spriteJugador.png", x,y);
mapa->insertBackgroundObject("enfermera", 100,500);
mapa->insertBackgroundObject("sprites/pelota.png", 200,2000);
mapa->insertBackgroundObject("sprites/pelota.png", 150,700);
mapa->insertBackgroundObject("sprites/pelota.png", 100,100);
mapa->insertBackgroundObject("sprites/pelota.png", 450,657);
mapa->insertBackgroundObject("sprites/pelota.png", 300,500);

mapa->insertBackgroundObject("sprites/pelota.png", 100,100);
mapa->insertBackgroundObject("sprites/pelota.png", 150,100);
mapa->insertBackgroundObject("sprites/pelota.png", 200,100);
mapa->insertBackgroundObject("sprites/pelota.png", 250,100);
mapa->insertBackgroundObject("sprites/pelota.png", 300,100);
mapa->insertBackgroundObject("sprites/pelota.png", 350,100);
mapa->insertBackgroundObject("sprites/pelota.png", 400,100);
mapa->insertBackgroundObject("sprites/pelota.png", 450,100);
mapa->insertBackgroundObject("sprites/pelota.png", 500,100);

mapa->lunchScreen();





	return 0;
}
