#include <Application.Platform.h>

#include <memory>
#include <SDL.h>
#include <SDL_image.h>
namespace application
{
	static std::shared_ptr<SDL_Window> window = nullptr;
	static std::shared_ptr<Engine::Renderer> renderer = nullptr;

	void Platform::Initialize()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);
	}

	void Platform::SetFullscreen(bool flag)
	{
		SDL_SetWindowFullscreen(window.get(), (flag) ? (SDL_WINDOW_FULLSCREEN_DESKTOP) : (0));
	}

	bool Platform::IsFullscreen()
	{
		return (SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN) > 0;
	}

	void Platform::StartWindow(int windowWidth, int windowHeight, int logicalWidth, int logicalHeight, const std::string& windowTitle, const std::string& iconFileName)
	{
		SDL_Window* pw = nullptr;//TODO: to application platform
		SDL_Renderer* pr = nullptr;
		SDL_CreateWindowAndRenderer(
			windowWidth,
			windowHeight,
			SDL_WINDOW_RESIZABLE,
			&pw,
			&pr);
		window = std::shared_ptr<SDL_Window>(pw, SDL_DestroyWindow);
		renderer = std::make_shared<Engine::Renderer>(pr);
		SDL_RenderSetLogicalSize(renderer.get()->renderer.get(), logicalWidth, logicalHeight);
		SDL_SetWindowTitle(window.get(), windowTitle.c_str());
		auto iconSurface = IMG_Load(iconFileName.c_str());
		SDL_SetWindowIcon(window.get(), iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	const std::shared_ptr<Engine::Renderer>& Platform::GetRenderer()
	{
		return renderer;
	}

	void Platform::Present()
	{
		SDL_RenderPresent(renderer.get()->renderer.get());
	}
}