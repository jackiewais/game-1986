#include "LTexture.h"

class TrucoJugador{

public:
	static const int T_WIDTH = 70;
	static const int T_HEIGHT = 68;
	static const int TRUCO_ANIMATION_FRAMES = 15;

	void init(SDL_Renderer* gRend, char *spritePath);
	char *spritePath;
	~TrucoJugador();

	bool render(bool pausa);

	bool loadMedia();

	void hacerTruco(int posX, int posY);
	bool active = false;

	void setAlpha(int alpha);

private:
	LTexture gTrucoTexture;
	SDL_Rect gSpriteClips[ TRUCO_ANIMATION_FRAMES ];

	int mPosX, mPosY;

	int frame;

	int rep;
	};
