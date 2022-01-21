#include <Application.Engine.h>
namespace application
{
	bool Engine::ShouldStart()
	{
		return true;
	}
	const std::string Engine::CONFIG_FILE = "config/ui/application.json";
}
