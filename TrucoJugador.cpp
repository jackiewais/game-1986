#import "TrucoJugador.h"

void TrucoJugador::init(SDL_Renderer* gRend)
{
	gTrucoTexture.gRenderer = gRend;

    frame = 0;
    rep = 0;
	//Load media
	if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}

}

bool TrucoJugador::loadMedia()
{
	bool success = true;

	if( !gTrucoTexture.loadFromFile  ( "sprites/jugadorColision.png" ) )
	{
		printf( "Failed to load move texture!\n" );
		success = false;
	}else
	{
		for (unsigned int i=0;i < TRUCO_ANIMATION_FRAMES ;i++){
			gSpriteClips[ i ].x =   T_WIDTH * i;
			gSpriteClips[ i ].y =   0;
			gSpriteClips[ i ].w =  T_WIDTH;
			gSpriteClips[ i ].h =  T_HEIGHT;
		}
	}

	return success;
}

void TrucoJugador::hacerTruco(int posX, int posY){

    mPosX = posX;
    mPosY = posY;
	active = true;
}
bool TrucoJugador::render(bool pausa)
{
	SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
	gTrucoTexture.render( mPosX, mPosY, currentClip );

	if (!pausa){
		++frame;
		if( frame / 4 >= TRUCO_ANIMATION_FRAMES ){
			frame = 0;
			rep ++;
		}

		if (rep == 5){
			active = false;
			rep = 0;
		}
	}
	return active;
}


void TrucoJugador::setAlpha(int alpha){
	gTrucoTexture.setAlpha(alpha);
}

TrucoJugador::~TrucoJugador()
{
	gTrucoTexture.free();
}
