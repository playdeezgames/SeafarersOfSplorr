#include <Application.Platform.h>
#include <memory>
#include <SDL.h>
namespace application
{
	void Platform::Initialize()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);
	}
}