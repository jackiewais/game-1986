#include "LTexture.h"
#include "Pelota.h"
#include "PelotaHelper.h"
#include "TrucoJugador.h"
#include "Elemento/Elemento.h"
#include "Label.h"
#include <iostream>
#include <list>

using namespace std;

class Jugador
{
    public:
		static const int JUG_WIDTH = 80;
		static const int JUG_HEIGHT = 68;
		static const int JUGADOR_ANIMATION_FRAMES = 6;

		int JUG_VEL;
		int PEL_VEL;

		char *spritePathPelota;
		char *spritePathJugador1;
		char *spritePathJugador2;
		char *spritePathJugador3;
		char * spritePathTruco;

		Jugador(SDL_Renderer* gRend, int scr_width, int scr_height, int intJug, string nombre,
				int velDesplazamiento, int velDisparo, char *spritePathPelota, char *spritePathJugador1,
				char *spritePathJugador2, char *spritePathJugador3, char * spritePathTruco);
		~Jugador();

		void patear();
		void hacerTruco();
		void moverPelotas();
		void forzarPosicion(int x, int y);

		void updateFromElemento();
		void handleEvent( SDL_Event& e);

		void move();

		void render();

		bool loadMedia();

		void setElemento(Elemento* elem);

		void managePausa(bool pausa);
		void manageDesconexion(bool d);
		void cleanStatus();
		Elemento* elemento;
    private:
		LTexture gJugadorTexture;
		SDL_Rect gSpriteClips[ JUGADOR_ANIMATION_FRAMES ];
		list<Pelota *> lista_pelotas;
		PelotaHelper pelotaHelper;
		TrucoJugador truco;
		Label lDesconectado;

		int mPosX, mPosY;
		int mVelX, mVelY;

		int frame;
		int screen_width,screen_height;

		bool pausa = true;
		bool desconectado = false;

		int id;
		string name;

		void manageAlpha();
};
