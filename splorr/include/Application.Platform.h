#pragma once
#include "Application.Engine.h"
namespace application
{
	struct Platform
	{
		static void Initialize();
		static void SetFullscreen(bool);
		static bool IsFullscreen();
		static void StartWindow(int, int, int, int, const std::string&, const std::string&);
		static const std::shared_ptr<Engine::Renderer>& GetRenderer();
		static void Present();
	};
}
