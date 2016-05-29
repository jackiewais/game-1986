
#include "LTexture.h"
#include <list>

using namespace std;

class PelotaHelper
{
    public:

		void initTexture(SDL_Renderer* gRend, char spritePath[60]);

		bool loadMedia();

		~PelotaHelper();

		LTexture gPelotaTexture;

		char *spritePath;
    private:
};
