#include "Command.h"
#include "Common.Data.h"
#include "Data.Stores.h"
#pragma warning (disable: 26812)
namespace application::Keyboard
{
	static std::map<int, ::Command> keyboardCommands;

	static bool initialized = false;

	static void Initialize()
	{
		if (!initialized)
		{
			auto& config = data::Stores::GetStore(data::Store::KEYS);
			for (auto& item : config.items())
			{
				int code = common::Data::StringToInt(item.key());
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