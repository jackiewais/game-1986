#include "LTexture.h"

#include <iostream>

using namespace std;


class Label {
	public:
		Label();
		void setData(SDL_Renderer* gRend, string text, int posX, int posY, int size);
		bool loadMedia();
		void close();
		void render();

	private:
		string ltext;
		int lsize;
		LTexture gTextTexture;
		TTF_Font *gFont = NULL;
		int mPosX, mPosY;


};
