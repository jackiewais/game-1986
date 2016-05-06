#include "LTexture.h"

#include <iostream>

using namespace std;


class Label {
	public:
		Label(SDL_Renderer* gRend, string text, int posX, int posY);
		bool loadMedia();
		void close();
		void render();
		void setText(string text);

	private:
		string ltext;
		LTexture gTextTexture;
		TTF_Font *gFont = NULL;
		int mPosX, mPosY;
};
