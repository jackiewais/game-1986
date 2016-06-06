/*
 * Escenario.cpp
 *
 *  Created on: 22/04/2016
 *      Author: nahue
 */
#include <SDL2/SDL.h>
#include "Escenario.h"
#include "connectionManager/ConnectionManager.h"
#include <stdio.h>
#include <map>
#include <fstream>

#include <sys/socket.h>
#include "Utils/messages.h"
#include "Utils/Util.h"
#include "Sound.h"

#define BUFLEN 1000

using namespace std;
Sound esound;
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
Util myUtil;

type_Elemento miVentana;
type_Elemento miEscenario;
list<type_Elemento> obstaculos;
list<type_Elemento> spriteJugadores;

// En el MAIN se debería llamar a este constructor inicialmente.
// Luego se le pide al servidor los datos del Escenario.
// Para luego invocar al método: generarEscenario() que se encarga
// de completar todos los datos relacionados al escenario.-
Escenario::Escenario(ConnectionManager* connectionManager, int scroll) {
 	this->scroll=scroll;
	conManager = connectionManager;
	clientId = connectionManager -> getId();
	escenarioHeight = 0;
	started = false;
	pausa = true;
	
}

Escenario::Escenario(ConnectionManager* connectionManager, int scroll, int offset, bool playing) {
	pausa = !playing;
	started = playing;
 	this->scroll=scroll;
 	escenarioHeight = offset;
	conManager = connectionManager;
	clientId = connectionManager -> getId();
	
}


void Escenario::setCantJugadores(int cantJugador){
	cantJugadores=cantJugador;
}
void Escenario::deleteBackgroundObjetcs(){
//elimino las texturas en los backgroundTemplate
for (list<BackgroundTemplate *>::iterator it=backgroundObjetcs.begin(); it != backgroundObjetcs.end(); ++it){
		(*it)->~BackgroundTemplate();
		delete(*it);
	}
//vacio la lista. 	
this->backgroundObjetcs.clear();
}
//Deprecado.
Escenario::Escenario(int height, int width, ConnectionManager* connectionManager,int scroll) {
 	this->scroll=scroll;
	this->setSize(width,height);
	conManager = connectionManager;
	clientId = connectionManager -> getId();
	cantJugadores = 2;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		this -> gBGTexture.gRenderer = gRenderer;

		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
	}

}

void Escenario::setEscenarioSize( int height){
	this->escenarioSize.height=height;
}

void Escenario::setVelocidades(int velocidadDesplazamiento, int velocidadDisparo){
	this->velocidadDesplazamiento = velocidadDesplazamiento;
	this->velocidadDisparo = velocidadDisparo;
}

void Escenario::getSpritePath(char *&spritePath, Parser::spriteType id) {
	list<Parser::type_Sprite*>::iterator iter;
	for (iter = sprites.begin(); iter != sprites.end(); iter ++) {
		if ((*iter)->id == id){
			spritePath = (*iter)->path;
			break;
		}
	}
}

void Escenario::crearJugador(int jugId, string nombre, int posXIni){
	char *spritePathPelota, *spritePathJugador1, *spritePathJugador2,
		*spritePathJugador3, *spritePathTruco;
	getSpritePath(spritePathPelota, Parser::PE);
	getSpritePath(spritePathJugador1, Parser::J1);
	getSpritePath(spritePathJugador2, Parser::J2);
	getSpritePath(spritePathJugador3, Parser::J3);
	getSpritePath(spritePathTruco, Parser::VU);
	//cout << "sprite pelota " << spritePathPelota << endl;
	//cout << "sprite jugador " << spritePathJugador1 << endl;
	//cout << "sprite truco " << spritePathTruco << endl;
	cout << "DEBUG crearJugador posXIni = " << posXIni << endl;
	Jugador* otroJugador = new Jugador(gRenderer,screen.width,screen.height, jugId, cantJugadores, nombre,
			velocidadDesplazamiento, velocidadDisparo, spritePathPelota, spritePathJugador1,
			spritePathJugador2, spritePathJugador3, spritePathTruco);
	Elemento* elemento = new Elemento(jugId,posXIni,screen.height-68);
	otroJugador->setElemento(elemento);
	if (started)
		otroJugador->managePausa(pausa);
	jugadores[jugId] = otroJugador;
}

bool Escenario::lunchScreen(struct gst* position, bool forcePos){

	bool quit = false;
	int scrollingOffset = escenarioHeight % gBGTexture.getHeight();
	if (reset){
		scrollingOffset = 0;
		escenarioHeight = 0;
	}

	cout << "DEBUG lunchScreen: gBGTexture.getHeight() = " << gBGTexture.getHeight() << endl;
	cout << "DEBUG lunchScreen: scrollingOffset = " << scrollingOffset << endl;
	cout << "DEBUG lunchScreen: escenarioHeight = " << escenarioHeight << endl;

	SDL_Event e;
	

	for(int i = 1; i <= cantJugadores; i++){
		//if (i != clientId)
			crearJugador(i,"Pepe",screen.width*i/(cantJugadores+1));

	}
	Jugador* jugador = jugadores[clientId];
	if (forcePos){
		int posX = atoi(position->posx);
		int posY = atoi(position->posy);
		jugador->forzarPosicion(posX,posY);

	}

	Label lpausa;
	lpausa.setData(gRenderer, string("Pause"),screen.width/2,screen.height/2,72);
	Label lesperando;
	lesperando.setData(gRenderer, string("Esperando Jugadores"),screen.width/2,screen.height/2+36,24);
	Label ldesconectado;
	ldesconectado.setData(gRenderer, string("Servidor desconectado, presione una tecla para salir"),screen.width/2,screen.height/2+60,20);

	reset = false;


	while( !quit && !reset)
	{

		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT ){
				quit = true;
				jugador->elemento->updateStatus(DESCONECTADO);
			}

			if ( e.type == SDL_KEYUP && e.key.repeat == 0){
				switch(e.key.keysym.sym){
					case SDLK_p:
						if (started){
							jugador->elemento->updateStatus((!pausa)?PAUSA:NO_PAUSA);
							
						}
						break;

					case SDLK_r:
						reset = true;
						jugador->elemento->updateStatus(status::RESET);
						break;

					case SDLK_a:
						if (!started){
							jugador->elemento->updateStatus(status::START);
						}
						break;
				}
				if (desconectado)
					quit = true;

			}

			if (!pausa){
				jugador->handleEvent(e);

			}

		}

		if (escenarioHeight == escenarioSize.height)
		{
			reset = true;
			jugador->elemento->updateStatus(status::RESET);
		}

		sendStatus();

		if (!pausa){
			jugador->move();
			for(auto const &it : jugadores) {
				it.second ->moverPelotas();
			}
			//Scroll background
			scrollingOffset+=scroll;
			escenarioHeight+=scroll;
			if( scrollingOffset >gBGTexture.getHeight() )
			{
				scrollingOffset = 0;
			}
		}


		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		gBGTexture.render( 0, scrollingOffset);
		gBGTexture.render(0,  scrollingOffset - gBGTexture.getHeight() );

		renderBackgroundObjects(scrollingOffset);

		//Renderizo el resto de los jugadores
		for(auto const &it : jugadores) {
			if (it.first != clientId){
				it.second ->render();
			}
		}

		//Renderizo mi jugador para que esté adelante
		jugador->render();
		if (pausa)
			lpausa.render();
		if (!started)
			lesperando.render();
		if (desconectado)
			ldesconectado.render();

		SDL_RenderPresent( gRenderer );

		receiveStatus();
		updateJugadores();
		jugador->cleanStatus();

	}
			
	if (quit)
		close();
	else{
		deleteBackgroundObjetcs();
		lpausa.close();
		lesperando.close();
		ldesconectado.close();
		delete jugador;
		close();
	}

	return quit;
}

void Escenario::insertBackgroundObject(string path , int x , int y, int height, int scrH ){
	std::cout << "inserto objeto de fondo \n";
	BackgroundTemplate* object = new BackgroundTemplate(gRenderer,path, x,y,height, scrH);
	
	backgroundObjetcs.push_back(object);
}

void Escenario::renderBackgroundObjects(int scrollingOffset)
{
	for (list<BackgroundTemplate *>::iterator it=backgroundObjetcs.begin(); it != backgroundObjetcs.end(); ++it){
		(*it)->render(scrollingOffset);
	}
}

void Escenario::setSize(int width, int height) {
	std::cout << "setting scenario size \n";
	this->screen.height=height;
	this->screen.width=width;
}

void Escenario::mostrarVariables()
{
	cout << this->screen.height << endl;
	cout << this->screen.width << endl;

}

bool Escenario::init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "1986", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screen.width, this->screen.height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
			}
		}
	}

	return success;
}

bool Escenario::loadMedia()
{
	bool success = true;
	std::cout << "mi escenario spritePAth: " << miEscenario.spritePath << "\n";
	// CARGAMOS LA IMAGEN DE FONDO.
	//if( !gBGTexture.loadFromFile(miEscenario.spritePath) )
	if( !gBGTexture.loadFromFile( miEscenario.spritePath) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	// RECORREMOS TODOS LOS ELEMENTOS QUE SE OBTUVIERON RELACIONADOS CON EL ESCENARIO.-
	/*
	for(std::list<type_Elemento>::iterator it = obstaculos.begin(); it != obstaculos.end(); it++) {
		insertBackgroundObject((it)->spritePath,(it)->posicionX,(it)->posicionY,(it)->alto,(it)->alto);
	}
	*/
	return success;
}
void Escenario::close()
{
	gBGTexture.free();

	SDL_DestroyWindow( gWindow );
	SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;
	backgroundObjetcs.clear();
	sprites.clear();

	
	SDL_Quit();

}


void Escenario::sendStatus(){

	char *bufferSnd;
	struct gst* sndMsg;
	int bufferSndLen;

	sndMsg = genUpdateGstFromElemento(jugadores[clientId] -> elemento);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);
	bufferSnd[bufferSndLen] = '\0';
	conManager->sendMsg(bufferSnd,bufferSndLen);
	delete bufferSnd;
}

void Escenario::receiveStatus(){

	char bufferRcv[BUFLEN];
	struct gst** rcvMsgs;
	int rcvMsgsQty;
	memset(bufferRcv,0,BUFLEN);
	if (conManager->receive(bufferRcv) == 0){
		rcvMsgsQty = decodeMessages(&rcvMsgs, bufferRcv);

		if (rcvMsgsQty != -1){
			processMessages(rcvMsgs, rcvMsgsQty);
		}
	}else{
		desconectado = true;
		pausa = true;
		for(auto const &j : jugadores) {
			j.second->managePausa(pausa);
		}
	}

}

type_Elemento Escenario::parseMsg(struct gst* msg)
{
	type_Elemento miElemento = type_Elemento();
	char numero[posl+1], id[idl+1], path[pathl+1];

	memcpy(id, msg->id, idl);
	id[idl] = '\0';
	miElemento.elementoId = id;

	// Resguardamos la información obtenida referente a los tamaños.
	if(myUtil.sonDigitos(msg->ancho) && myUtil.sonDigitos(msg->alto)) {
		memcpy(numero, msg->ancho, posl);
		numero[posl] = '\0';
		miElemento.ancho = std::atoi( numero );
		memcpy(numero, msg->alto, posl);
		numero[posl] = '\0';
		miElemento.alto = std::atoi( numero );
	} else {
		miElemento.ancho = 10; miElemento.alto = 10;
		if (miElemento.elementoId == "FO") {miElemento.ancho = 100; miElemento.alto = 1000;}
		else if (miElemento.elementoId == "VE") {miElemento.ancho = 800; miElemento.alto = 600;}
		else if (miElemento.elementoId == "EL") {miElemento.ancho = 10; miElemento.alto = 10;}
	}

	// Resguardamos la información obtenida referente a las posiciones.
	if(myUtil.sonDigitos(msg->posx) && myUtil.sonDigitos(msg->posy)) {
		memcpy(numero, msg->posx, posl);
		numero[posl] = '\0';
		miElemento.posicionX = std::atoi( numero );
		memcpy(numero, msg->posy, posl);
		numero[posl] = '\0';
		miElemento.posicionY = std::atoi( numero );
	} else {
		miElemento.ancho = 10; miElemento.alto = 10;
		if (miElemento.elementoId == "FO") {miElemento.ancho = 100; miElemento.alto = 1000;}
		else if (miElemento.elementoId == "VE") {miElemento.ancho = 800; miElemento.alto = 600;}
		else if (miElemento.elementoId == "EL") {miElemento.ancho = 10; miElemento.alto = 10;}
	}

	if (miElemento.elementoId == "VE") miElemento.spritePath = "background.bmp";
	else {
	// Chequeamos la existencia del archivo imagen en cuestión.
		memset(path, '\0', pathl);
		memcpy(path, msg->path, pathl);
		ifstream fondoSprite (path);
		if (!fondoSprite.good()) {
			// El archivo imagen que queremos usar no existe, usamos el default.
			if (miElemento.elementoId == "FO") miElemento.spritePath = "vacioBackground.bmp";
			else if (miElemento.elementoId == "EL") miElemento.spritePath = "vacio.bmp";
		}
		else{
			// El path de la imagen es correcto y la podemos recuperar.
			miElemento.spritePath = path;
		}
	}


	// ==============================================
	// Lógica para setear las variables dentro de
	// la propia instancia del mapa.
	/*
		type_Elemento miEscenario;
		list<type_Elemento> obstaculos;
		list<type_Elemento> jugadores;
	*/
	// ==============================================
	if (miElemento.elementoId == "FO") miEscenario = miElemento;
	else if (miElemento.elementoId == "VE") miVentana = miElemento;
	else if (miElemento.elementoId == "EL") obstaculos.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR1") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR2") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR3") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR4") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR5") spriteJugadores.push_back(miElemento);
	// ==============================================
	return miElemento;
}

void Escenario::processMessages(struct gst** msgs, int msgQty){
	int tempId;
	Jugador* tempJu;

	for (int i = 0; i < msgQty; i++){

		if (msgs[i] -> type[0] == '2'){
			tempId = atoi(msgs[i]-> id);
			tempJu = jugadores[tempId];
			if (tempJu == NULL){
				//agregar Jugador
				crearJugador(atoi(msgs[i] -> id),"Juan",atoi(msgs[i] -> posx));
			}
			if (tempId != clientId)
				jugadores[tempId]-> elemento -> update(msgs[i]);
			else{
				if (msgs[i]->info[0] == (char) status::START) {
					//cout << "DEBUG processMessages recibi start" << endl;
					pausa = false;
					started = true;

					esound.play(esound.SONIDO_APLAUSO,20);

					for(auto const &it : jugadores) {
						it.second->managePausa(pausa);
						it.second ->hacerTruco();
					}
				}
				else if (msgs[i]->info[0] == (char) status::RESET) {
					reset = true;
				}
				else if ((msgs[i]->info[0] == (char) status::PAUSA) ||
						(msgs[i]->info[0] == (char) status::NO_PAUSA)){
					//pausa = true;
					pausa = (msgs[i]->info[0] == (char) status::PAUSA);
					for(auto const &it : jugadores) {
						it.second->managePausa(pausa);
					}
				}

			}
		}
		else if (msgs[i] -> type[0] == '8'){
			if (msgs[i] -> info[0] == (char) command::DISCONNECT){
				conManager -> disconnect();
			}
		}

		delete msgs[i];
	}
}

void Escenario::generarEscenario()
{
	this->setSize(miVentana.ancho,miVentana.alto);
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		this->gBGTexture.gRenderer=gRenderer;

		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
	}
}

void Escenario::addSprites(list<Parser::type_Sprite*> sprites) {
	this->sprites = sprites;
}

void Escenario::updateJugadores(){
	for(auto const &it : jugadores) {
		if (it.first != clientId){
			it.second->updateFromElemento();
			switch (it.second->elemento->getEstado()){
				case PAUSA:
					if (!pausa){
						for(auto const &j : jugadores) {
							j.second->managePausa(pausa);
						}
					}
					break;
				case NO_PAUSA:
					if (pausa){
						for(auto const &j : jugadores) {
							j.second->managePausa(pausa);
						}
					}
					break;
				case RESET:
					reset = true;
					break;
				default: break;
			}
		}
	}
}





