#include "LTexture.h"

class Jugador
{
    public:
		static const int JUG_WIDTH = 80;
		static const int JUG_HEIGHT = 68;
		static const int JUGADOR_ANIMATION_FRAMES = 6;

		static const int JUG_VEL = 5;

		Jugador(SDL_Renderer* gRend , int scr_width, int scr_height);
		~Jugador();

		void handleEvent( SDL_Event& e);

		void move();

		void render();

		bool loadMedia();

    private:
		LTexture gJugadorTexture;
		SDL_Rect gSpriteClips[ JUGADOR_ANIMATION_FRAMES ];

		int mPosX, mPosY;

		int mVelX, mVelY;

		int frame;
		int screen_width,screen_height;
};