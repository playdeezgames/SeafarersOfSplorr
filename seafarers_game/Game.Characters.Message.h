#pragma once
#include <vector>
#include <string>
namespace game::characters
{
	struct Message
	{
		std::string_view color;
		std::string text;
	};

	typedef std::vector<Message> MessageList;
}