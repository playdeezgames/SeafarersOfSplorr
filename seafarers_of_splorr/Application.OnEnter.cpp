#include "Application.Handlers.h"
#include "Application.OnEnter.h"
#include <map>
#include <vector>
namespace application::OnEnter
{
	static std::map<int, std::vector<Handler>> handlers;

	void AddHandler(int state, Handler handler)
	{
		if (handlers.find(state) == handlers.end())
		{
			handlers[state] = std::vector<Handler>();
		}
		handlers[state].push_back(handler);
	}

	void Handle()
	{
		application::Handlers::WithCurrent(handlers, [](const std::vector<Handler>& enterers)
		{
			for (auto& enterer : enterers)
			{
				enterer();
			}
		});
	}
}