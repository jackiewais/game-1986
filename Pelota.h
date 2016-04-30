#include "LTexture.h"

class Pelota
{
    public:

		static const int PEL_WIDTH = 20;
		static const int PEL_HEIGHT = 20;

		static const int PEL_VEL = 7;

		Pelota(LTexture* gText, int posIX, int posIY);
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
