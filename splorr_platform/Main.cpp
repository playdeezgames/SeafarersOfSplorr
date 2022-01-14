#include <algorithm>
#include <Application.Engine.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <iterator>
#include <SDL.h>
static std::vector<std::string_view> ParseCommandLine(int argc, char** argv)
{
	std::vector<std::string_view> arguments;
	arguments.reserve(argc);
	std::transform(
		argv, 
		argv + argc, 
		std::back_inserter(arguments), 
		[](const char* arg) 
		{
			return std::string_view(arg);
		});
	return arguments;
}
int main(int argc, char** argv)
{
	if (!application::Engine::ShouldStart())
	{
		return 0;
	}
	common::RNG::Seed();
	auto arguments = ParseCommandLine(argc, argv);
	return application::Engine::Run(arguments);
}

