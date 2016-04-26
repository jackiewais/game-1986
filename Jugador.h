#include "LTexture.h"

class Jugador
{
    public:
		static const int JUG_WIDTH = 20;
		static const int JUG_HEIGHT = 20;

		static const int JUG_VEL = 10;

		Jugador(SDL_Renderer* gRend);

		void handleEvent( SDL_Event& e );

		void move();

		void render();

		bool loadMedia();

    private:
		LTexture gJugadorTexture;

		int mPosX, mPosY;

		int mVelX, mVelY;

		int SCREEN_WIDTH, SCREEN_HEIGHT;
};
