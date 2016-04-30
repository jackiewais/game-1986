
#include "LTexture.h"
#include <list>

using namespace std;

class PelotaHelper
{
    public:

		void initTexture(SDL_Renderer* gRend);

		bool loadMedia();

		~PelotaHelper();

		LTexture gPelotaTexture;
    private:
};
