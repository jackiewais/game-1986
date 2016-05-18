#include "LTexture.h"

class Pelota
{
    public:

		static const int PEL_WIDTH = 20;
		static const int PEL_HEIGHT = 20;

		int PEL_VEL;

		Pelota(LTexture* gText, int posIX, int posIY, int velDisparo);
		~Pelota();

		void setDataCopy(LTexture* gText);
		void setData(SDL_Renderer* gRend);

		bool loadMedia();

		void patear( int posIX, int posIY);
		void move();

		void render();

		bool ballIsOut();

		LTexture gPelotaTexture;
    private:
		int mPosX, mPosY;
		bool isOut;
};
