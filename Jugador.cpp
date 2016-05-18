#include "Jugador.h"
#include <iostream>
#include <string>

using namespace std;

Jugador::Jugador(SDL_Renderer* gRend, int scr_width, int scr_height, int intJug, string nombre, int velDesplazamiento, int velDisparo)
{
	gJugadorTexture.gRenderer = gRend;

	id = intJug;
	name = nombre;

	JUG_VEL = velDesplazamiento;
	PEL_VEL = velDisparo;

    //Initialize the offsets
    mPosX = scr_width*intJug/3;
    mPosY = scr_height-JUG_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    frame = 0;

    screen_width = scr_width;
    screen_height = scr_height;

	//Load media
	if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}

	pelotaHelper.initTexture(gJugadorTexture.gRenderer);
	truco.init(gJugadorTexture.gRenderer);

	lDesconectado.setData(gJugadorTexture.gRenderer, "Jugador " + to_string(id) + " desconectado",screen_width/2,(id-1)*24,24);
}

void Jugador::setElemento(Elemento* elem){
	elemento = elem;
}

bool Jugador::loadMedia()
{
	//Loading success flag
	bool success = true;

	if( !gJugadorTexture.loadFromFile  ( "sprites/spriteJugador" + to_string(id) + ".png" ))
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}else
	{
		for (unsigned int i=0;i < JUGADOR_ANIMATION_FRAMES ;i++){
			gSpriteClips[ i ].x =   JUG_WIDTH * i;
			gSpriteClips[ i ].y =   0;
			gSpriteClips[ i ].w =  JUG_WIDTH;
			gSpriteClips[ i ].h =  JUG_HEIGHT;
		}
	}
	return success;
}
void Jugador::handleEvent( SDL_Event& e )
{
	//Si está haciendo el truco, no me puedo mover ni patear

			//If a key was pressed
			if( e.type == SDL_KEYDOWN && e.key.repeat == 0){
				//Adjust the velocity
				switch( e.key.keysym.sym ){
					case SDLK_UP: mVelY -= JUG_VEL; break;
					case SDLK_DOWN: mVelY += JUG_VEL; break;
					case SDLK_LEFT: mVelX -= JUG_VEL; break;
					case SDLK_RIGHT: mVelX += JUG_VEL; break;
					case SDLK_SPACE: if (!truco.active) patear(); break;
					case SDLK_RETURN: if (!truco.active) hacerTruco(); break;
				}
				cout << "PRESS PosY: " + to_string(mPosY) + " - velY: " + to_string(mVelY) << endl;
			}
			//If a key was released
			else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
				//Adjust the velocity
				switch( e.key.keysym.sym ){
					case SDLK_UP: mVelY += JUG_VEL; break;
					case SDLK_DOWN: mVelY -= JUG_VEL; break;
					case SDLK_LEFT: mVelX += JUG_VEL; break;
					case SDLK_RIGHT: mVelX -= JUG_VEL; break;

				}
				cout << "SUELYPosY: " + to_string(mPosY) + " - velY: " + to_string(mVelY) << endl;
			}

}

void Jugador::hacerTruco(){
	truco.hacerTruco(mPosX,mPosY);
	elemento->update(mPosX,mPosY,TRUCO);
}

void Jugador::patear(){
	Pelota* pelota = new Pelota(&pelotaHelper.gPelotaTexture,mPosX+(JUG_WIDTH/2),mPosY, PEL_VEL);
	lista_pelotas.push_back(pelota);
	elemento->update(mPosX,mPosY,DISPARANDO);
}

void Jugador::moverPelotas(){
    Pelota* pelToDelete;
    for (list<Pelota *>::iterator it=lista_pelotas.begin(); it != lista_pelotas.end(); ++it){
    	(*it)->move();
    	if ((*it)->ballIsOut())
    		pelToDelete = (*it);
    }

    lista_pelotas.remove(pelToDelete);
}

void Jugador::move()
{
	if (!truco.active){
		mPosX += mVelX;
		if( ( mPosX < 0 ) || ( mPosX + JUG_WIDTH > screen_width ) ){
			//Move back
			mPosX -= mVelX;
		}

		cout << "PosY: " + to_string(mPosY) + " - velY: " + to_string(mVelY) << endl;
		mPosY += mVelY;
		if( ( mPosY < 0 ) || ( mPosY + JUG_HEIGHT > screen_height ) ){
			mPosY -= mVelY;
		}
	}
	elemento->updatePos(mPosX,mPosY);

}

void Jugador::forzarPosicion(int x, int y){
	mPosX = x;
	mPosY = y;
}


void Jugador::updateFromElemento(){
	forzarPosicion(elemento->getPosX(),elemento->getPosY());

	switch (elemento->getEstado()){
		case DISPARANDO:
			patear();
			cleanStatus();
			break;
		case TRUCO:
			hacerTruco();
			cleanStatus();
			break;
		case DESCONECTADO:
			manageDesconexion(true);
			break;
		case VIVO:
			if (desconectado)
				manageDesconexion(false);
			break;
	}

}

void Jugador::cleanStatus(){
	elemento->updateStatus((desconectado)?DESCONECTADO:VIVO);
}
void Jugador::render()
{
	for (list<Pelota *>::iterator it=lista_pelotas.begin(); it != lista_pelotas.end(); ++it)
		(*it)->render();

	if (truco.active){
		truco.render(pausa);
	}else{
		if (!pausa){
			++frame;
			if( frame / 6 >= JUGADOR_ANIMATION_FRAMES ){
				frame = 0;
			}
		}

		SDL_Rect* currentClip = &gSpriteClips[ frame / 6 ];
		gJugadorTexture.render( mPosX, mPosY, currentClip );
		if (desconectado)
			lDesconectado.render();
	}
}

void Jugador::manageAlpha(){
	int alpha = (pausa || desconectado)?128:255;
	gJugadorTexture.setAlpha(alpha);
	truco.setAlpha(alpha);
}

void Jugador::managePausa(bool p){
	pausa = p;
	manageAlpha();

}

void Jugador::manageDesconexion(bool d){
	desconectado = d;
	manageAlpha();
}

Jugador::~Jugador()
{
	gJugadorTexture.free();
	lDesconectado.close();
}
