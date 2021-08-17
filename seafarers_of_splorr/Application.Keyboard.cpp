#include "Command.h"
#include "Common.Data.h"
#include "Data.JSON.Store.h"
#include "Data.JSON.Stores.h"
#pragma warning (disable: 26812)
namespace application::Keyboard
{
	static std::map<int, ::Command> keyboardCommands;

	static bool initialized = false;

	static void Initialize()
	{
		if (!initialized)
		{
			auto& config = data::json::Stores::GetStore(data::json::Store::KEYS);
			for (auto& item : config.items())
			{
				int code = common::Data::ToInt(item.key());
				::Command command = (::Command)(int)item.value();
				keyboardCommands[code] = command;
			}
			initialized = true;
		}
	}

	std::optional<::Command> ToCommand(int keycode)
	{
		Initialize();
		auto iter = keyboardCommands.find(keycode);
		if (iter != keyboardCommands.end())
		{
			return iter->second;
		}
		return std::nullopt;
	}
}