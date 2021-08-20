#include "Application.Handlers.h"
#include "Application.TextInput.h"
#include <map>
namespace application::TextInput
{
	static std::map<int, std::vector<Handler>> textInputHandlers;

	void Handle(const std::string& text)
	{
		application::Handlers::WithCurrent(
			textInputHandlers,
			[text](const std::vector<Handler>& handlers)
			{
				for (auto& handler : handlers)
				{
					handler(text);
				}
			});
	}

	void AddHandler(int state, Handler handler)
	{
		textInputHandlers[state].push_back(handler);
	}
}