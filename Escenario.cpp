/*
 * Escenario.cpp
 *
 *  Created on: 22/04/2016
 *      Author: nahue
 */
#include <SDL2/SDL.h>
#include "Escenario.h"	
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

type_Elemento miEscenario;
list<type_Elemento> obstaculos;
list<type_Elemento> spriteJugadores;

Escenario::Escenario() {
	clientId = 1;
}

//Deprecado.
Escenario::Escenario(int height, int width) {
	this->setSize(width,height);
	clientId = 1;

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

void Escenario::crearJugador(int jugId, string nombre, int posXIni){
	Jugador* otroJugador = new Jugador(gRenderer,screen.width,screen.height, jugId, nombre);
	Elemento* elemento = new Elemento(jugId,posXIni,screen.height-68);
	otroJugador->setElemento(elemento);
	jugadores[jugId] = otroJugador;
}

bool Escenario::lunchScreen(){

bool quit = false;
bool started = false;


SDL_Event e;
int scrollingOffset = 0;

crearJugador(1,"Juan",screen.height/3);
crearJugador(2,"Roman",screen.height*2/3);

Jugador* jugador = jugadores[clientId];

Label lpausa;
lpausa.setData(gRenderer, string("Pause"),screen.width/2,screen.height/2,72);
Label lesperando;
lesperando.setData(gRenderer, string("Esperando Jugadores"),screen.width/2,screen.height/2+36,24);

bool reset = false;

	while( !quit && !reset)
	{
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
							jugador->elemento->update(jugador->elemento->getPosX(),jugador->elemento->getPosY(),PAUSA);
						}
						break;

					case SDLK_r:
						reset = true;
						jugador->elemento->update(jugador->elemento->getPosX(),jugador->elemento->getPosY(),RESET);
						break;
				}
			}

			if (!pausa){
				jugador->handleEvent(e);
			}

			//TODO ESTO SE REEMPLAZA POR LOS MENSAJES DEL SERVIDOR
			if (e.type == SDL_KEYUP && e.key.repeat == 0){
				switch (e.key.keysym.sym){
					case SDLK_0:
						jugadores[2]->elemento->updateStatus(DISPARANDO);
						break;
					case SDLK_9:
						jugadores[2]->elemento->updateStatus(TRUCO);
						break;
					case SDLK_1:
						jugadores[2]->elemento->updatePos(15,70);
						break;
					case SDLK_2:
						jugadores[2]->elemento->updatePos(230,400);
						break;
					case SDLK_q:
						jugadores[2]->elemento->updateStatus(DESCONECTADO);
						break;
					case SDLK_w:
						jugadores[2]->elemento->updateStatus(VIVO);
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
			//-------------------------------------------------------------
		}

		if (!pausa){
			jugador->move();
			for(auto const &it : jugadores) {
				it.second ->moverPelotas();
			}
			//Scroll background
			++scrollingOffset;
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


void Escenario::interchangeStatus(map<int,Elemento*> &elementos){

	char *bufferSnd, bufferRcv[BUFLEN];
	struct gst* sndMsg, ** rcvMsgs;
	int bufferSndLen, rcvMsgsQty;

	sndMsg = genUpdateGstFromElemento(elementos[clientId]);
	bufferSndLen = encodeMessages(&bufferSnd, &sndMsg, 1);

	send(socketCliente,bufferSnd,bufferSndLen,0);
	memset(bufferRcv,0,BUFLEN);
	if (receiveMsg(bufferRcv) == 0){
		rcvMsgsQty = decodeMessages(&rcvMsgs, bufferRcv);

		if (rcvMsgsQty != -1){
			processMessages(elementos, rcvMsgs, rcvMsgsQty);
		}
	}

}

int Escenario::receiveMsg(char* buffer){

	char msgLenChar[3];
	int msgLen, rcvLen;

	rcvLen = recv(socketCliente, buffer, BUFLEN -1 , 0);
	if( rcvLen < 0){
		//glog.writeErrorLine("ERROR al recibir la respuesta. El servidor no responde");
		return 1;
	}


	memcpy(msgLenChar, buffer, 3);
	msgLen = stoi(msgLenChar, nullptr);

	if (rcvLen == msgLen){//full message received.
		return 0;
	}else{//message incomplete.
		int readed = rcvLen;
		while ( readed != msgLen){
			rcvLen = recv(socketCliente, buffer+readed, msgLen-readed, 0);
			readed += rcvLen;
		}

	}

	return 0;
}

type_Elemento Escenario::parseMsg(string s)
{
	//std::string s = "SPRITEID,LONGLONGLONGLONG,PATH";

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
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
	    token = s.substr(0, pos);
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
	    		miElemento.ancho = 10;
	    		miElemento.alto = 10;
	    	}
	    	// Resguardamos la información obtenida referente a las posiciones.
	    	digitosAncho = new char[token.substr(8,4).length()+1];
	    	digitosAlto = new char[token.substr(12,4).length()+1];
	    	if(myUtil.sonDigitos(digitosAncho) && myUtil.sonDigitos(digitosAlto)) {
	    		miElemento.posicionX = std::stoi( token.substr (8,4) );
	    		miElemento.posicionY = std::stoi( token.substr (12,4) );
	    	} else {
	    		// Usamos los valores por defecto.
	    		miElemento.posicionX = 25;
	    		miElemento.posicionY = 25;
	    	}
	    }
	    else if (count == 2)
	    {
			// Chequeamos la existencia del archivo imagen en cuestión.
			ifstream fondoSprite (token.c_str());
			if (!fondoSprite.good()) {
				// El archivo imagen que queremos usar no existe, usamos el default.
				miElemento.spritePath = "background.bmp";
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
	if (miElemento.elementoId == "FONDO") miEscenario = miElemento;
	else if (miElemento.elementoId == "ELEMENTO") obstaculos.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR1") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR2") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR3") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR4") spriteJugadores.push_back(miElemento);
	else if (miElemento.elementoId == "JUGADOR5") spriteJugadores.push_back(miElemento);
	// ==============================================
	return miElemento;
}

void Escenario::processMessages(map<int,Elemento*> &elementos, struct gst** rcvMsgsQty, int msgsQty){
	//TODO
}

void Escenario::generarEscenario()
{
	this->setSize(miEscenario.ancho,miEscenario.alto);
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
			//MANEJAR PAUSA Y RESET
		}
	}
}




