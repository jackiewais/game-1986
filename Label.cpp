#include "Label.h"


Label::Label(SDL_Renderer* gRend, string text, int posX, int posY){
	gTextTexture.gRenderer = gRend;
	ltext = text;
	mPosX = posX;
	mPosY = posY;

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	}else if( !loadMedia())
	{
		printf( "Failed to load media!\n" );
	}

}

bool Label::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "munro_small.ttf", 72 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		gTextTexture.gFont = gFont;
		//Render text
		SDL_Color textColor = { 255, 255, 255 };

		if( !gTextTexture.loadFromRenderedText( ltext, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}


	}

	return success;
}

void Label::setText(string text){
	ltext = text;
}
void Label::close()
{
	//Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
}

void Label::render(){
	gTextTexture.render( mPosX - gTextTexture.getWidth()/2 ,  mPosY - gTextTexture.getHeight()/ 2 );
}
