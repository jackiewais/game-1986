#include "LTexture.h"
#include "Pelota.h"
#include "PelotaHelper.h"
#include "TrucoJugador.h"
#include <iostream>
#include <list>

using namespace std;

class Jugador
{
    public:
		static const int JUG_WIDTH = 80;
		static const int JUG_HEIGHT = 68;
		static const int JUGADOR_ANIMATION_FRAMES = 6;

		static const int JUG_VEL = 5;

		Jugador(SDL_Renderer* gRend , int scr_width, int scr_height, double iWidth);
		~Jugador();

		void patear();
		void hacerTruco();
		void moverPelotas();
		void forzarPosicion(int x, int y);

		void handleEvent( SDL_Event& e);

		void move();

		void render();

		bool loadMedia();

		void managePausa(bool pausa);
		void manageDesconexion(bool d);

    private:
		LTexture gJugadorTexture;
		SDL_Rect gSpriteClips[ JUGADOR_ANIMATION_FRAMES ];
		list<Pelota *> lista_pelotas;
		PelotaHelper pelotaHelper;
		TrucoJugador truco;

		int mPosX, mPosY;
		int mVelX, mVelY;

		int frame;
		int screen_width,screen_height;

		bool pausa = true;
		bool desconectado = false;

		void manageAlpha();
};
