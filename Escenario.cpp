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

#define BUFLEN 1000

using namespace std;
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
Escenario::Escenario(ConnectionManager* connectionManager,int scroll) {
 	this->scroll=scroll;
	conManager = connectionManager;
	clientId = connectionManager -> getId();
	cantJugadores = 2;
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
		this->gBGTexture.gRenderer=gRenderer;

		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
	}

}

void Escenario::setEscenarioSize( int height){
this->escenarioSize.height=height;
}

void Escenario::crearJugador(int jugId, string nombre, int posXIni){
	Jugador* otroJugador = new Jugador(gRenderer,screen.width,screen.height, jugId, nombre);
	Elemento* elemento = new Elemento(jugId,posXIni,screen.height-68);
	otroJugador->setElemento(elemento);
	jugadores[jugId] = otroJugador;
}

bool Escenario::lunchScreen(struct gst* position){

	bool quit = false;
	bool started = false;
	int escenarioHeight=0;

	SDL_Event e;
	int scrollingOffset = 0;

	//crearJugador(atoi(position -> id),"Juan",atoi(position -> posx));
	//crearJugador(2,"Roman",screen.height*2/3);
	for(int i = 1; i <= cantJugadores; i++){
		//if (i != clientId)
			crearJugador(i,"Pepe",screen.height*i/(cantJugadores+1));

	}
	Jugador* jugador = jugadores[clientId];

	Label lpausa;
	lpausa.setData(gRenderer, string("Pause"),screen.width/2,screen.height/2,72);
	Label lesperando;
	lesperando.setData(gRenderer, string("Esperando Jugadores"),screen.width/2,screen.height/2+36,24);

	reset = false;

	while( !quit && !reset)
	{

		sendStatus();
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT ){
				quit = true;
			}

			if ( e.type == SDL_KEYUP && e.key.repeat == 0){
				switch(e.key.keysym.sym){
					case SDLK_p:
						if (started){
							pausa = !pausa;
							for(auto const &it : jugadores) {
								it.second->managePausa(pausa);
							}
							jugador->elemento->updateStatus((pausa)?PAUSA:NO_PAUSA);
						}
						break;

					case SDLK_r:
						reset = true;
						jugador->elemento->update(jugador->elemento->getPosX(),jugador->elemento->getPosY(),RESET);
						break;

					case SDLK_a:
						if (!started){
							pausa = false;
							started = true;

							for(auto const &it : jugadores) {
								it.second->managePausa(pausa);
								it.second ->hacerTruco();
							}
							break;
						}
				}
			}

			if (!pausa){
				jugador->handleEvent(e);
			}

		}

		if (!pausa){
			jugador->move();
			for(auto const &it : jugadores) {
				it.second ->moverPelotas();
			}
			//Scroll background
			scrollingOffset+=scroll;
			escenarioHeight+=scroll;
			cout << escenarioHeight << endl;
			if (escenarioHeight>escenarioSize.height)
			{ 
			reset = true;
			jugador->elemento->update(jugador->elemento->getPosX(),jugador->elemento->getPosY(),RESET);
			}
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

		SDL_RenderPresent( gRenderer );

		receiveStatus();
		//interchangeStatus(elementos);
		updateJugadores();
		jugador->cleanStatus();

	}
			
	close();

	return quit;
}

void Escenario::insertBackgroundObject(string path , int x , int y, int height, int scrH ){

	BackgroundTemplate* object = new BackgroundTemplate(gRenderer,path, x,y,height, scrH);
	
	backgroundObjetcs.push_back(object);
}

void Escenario::renderBackgroundObjects(int scrollingOffset)
{
	for (list<BackgroundTemplate *>::iterator it=backgroundObjetcs.begin(); it != backgroundObjetcs.end(); ++it)
		(*it)->render(scrollingOffset);
}

void Escenario::setSize(int width, int height) {
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

	// CARGAMOS LA IMAGEN DE FONDO.
	//if( !gBGTexture.loadFromFile(miEscenario.spritePath) )
	if( !gBGTexture.loadFromFile( "background.bmp") )
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
	
	SDL_Quit();

}


void Escenario::sendStatus(){

	char *bufferSnd;
	struct gst* sndMsg;
	int bufferSndLen;

	sndMsg = genUpdateGstFromElemento(jugadores[clientId] -> elemento);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);
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
	}

}

type_Elemento Escenario::parseMsg(string s)
{
	//std::string s = "SPRITEID,LONGLONGLONGLONG,PATH";
	string cleanS = trim(s);
	std::string delimiter = ",";
	// COUNT = nos indica la cantidad de iteraciones sobre el string en cuestion.
	// 0 = representa el ID del elemento.
	// 1 = representa el ancho, el alto y las posiciones.
	// 2 = representa el path específico del sprite.
	int count = 0;
	int longToken = 0;
	char* digitosAlto;
	char* digitosAncho;

	// Los elementos que componen el escenario.
	type_Elemento miElemento;

	size_t pos = 0;
	std::string token;
	while ((pos = cleanS.find(delimiter)) != std::string::npos)
	{
	    token = cleanS.substr(0, pos);
	    if(count == 0)
	    {
	    	// 0 = representa el ID del elemento con el que estamos trabajando.
	    	miElemento.elementoId = token;
	    }
	    else if (count == 1)
	    {
	    	// 1 = representa el ancho, el alto y las posiciones del elemento en cuestión.
	    	// Resguardamos la información obtenida referente a los tamaños.
	    	digitosAncho = new char[token.substr(0,4).length()+1];
	    	digitosAlto = new char[token.substr(4,4).length()+1];
	    	if(myUtil.sonDigitos(digitosAncho) && myUtil.sonDigitos(digitosAlto)) {
	    		miElemento.ancho = std::stoi( token.substr (0,4) );
	    		miElemento.alto = std::stoi( token.substr (4,4) );
	    	} else {
	    		miElemento.ancho = 10; miElemento.alto = 10;
				if (miElemento.elementoId == "FO") {miElemento.ancho = 100; miElemento.alto = 1000;}
				else if (miElemento.elementoId == "VE") {miElemento.ancho = 800; miElemento.alto = 600;}
				else if (miElemento.elementoId == "EL") {miElemento.ancho = 10; miElemento.alto = 10;}
	    	}
	    	// Resguardamos la información obtenida referente a las posiciones.
	    	digitosAncho = new char[token.substr(8,4).length()+1];
	    	digitosAlto = new char[token.substr(12,4).length()+1];
	    	if(myUtil.sonDigitos(digitosAncho) && myUtil.sonDigitos(digitosAlto)) {
	    		miElemento.posicionX = std::stoi( token.substr (8,4) );
	    		miElemento.posicionY = std::stoi( token.substr (12,4) );
	    	} else {
	    		// Usamos los valores por defecto.
	    		miElemento.posicionX = 10;
	    		miElemento.posicionY = 15;
	    	}
	    }
	    else if (count == 2)
	    {
			// Chequeamos la existencia del archivo imagen en cuestión.
			ifstream fondoSprite (token.c_str());
			if (!fondoSprite.good()) {
				// El archivo imagen que queremos usar no existe, usamos el default.
				if (miElemento.elementoId == "FO") miElemento.spritePath = "background.bmp";
				else if (miElemento.elementoId == "VE") miElemento.spritePath = "background.bmp";
				else if (miElemento.elementoId == "EL") miElemento.spritePath = "sprites/pelota.png";
			}
			else{
				// El path de la imagen es correcto y la podemos recuperar.
				miElemento.spritePath = token;
			}
	    }
	    std::cout << token << std::endl;
	    s.erase(0, pos + delimiter.length());

	    // Incrementamos COUNT siempre.-
	    count++;
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
	else if (miElemento.elementoId == "J1") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "J2") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "J3") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "J4") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "J5") spriteJugadores.push_back(miElemento);
	// ==============================================
	return miElemento;
}

string Escenario::trim(string& str)
{
    size_t first = str.find_first_not_of('-');
    size_t last = str.find_last_not_of('-');
    return str.substr(first, (last-first+1));
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


void Escenario::updateJugadores(){
	for(auto const &it : jugadores) {
		if (it.first != clientId){
			it.second->updateFromElemento();
			switch (it.second->elemento->getEstado()){
				case PAUSA:
					if (!pausa){
						pausa = true;
						for(auto const &j : jugadores) {
							j.second->managePausa(pausa);
						}
					}
					break;
				case NO_PAUSA:
					if (pausa){
						pausa = false;
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




