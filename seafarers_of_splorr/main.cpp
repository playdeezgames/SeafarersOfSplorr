#include <Application.Engine.h>
#include "Common.RNG.h"
#include "Common.Utility.h"
#include "Game.Achievements.h"
#include <SDL.h>
static std::vector<std::string> ParseCommandLine(int argc, char** argv)
{
	std::vector<std::string> arguments;
	arguments.reserve(argc);
	for (int index = 0; index < argc; ++index)
	{
		arguments.push_back(std::string(argv[index]));
	}
	return arguments;
}
int main(int argc, char** argv)
{
	if (game::Achievements::ShouldRestartApp())
	{
		return 0;
	}
	const std::string APPLICATION = "config/ui/application.json";
	common::RNG::Seed();
	auto arguments = ParseCommandLine(argc, argv);
	return application::Engine::Run(APPLICATION, arguments);
}

