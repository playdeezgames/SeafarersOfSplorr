#pragma once
#include <vector>
#include <string>
namespace game::characters
{
	struct Message
	{
		std::string color;
		std::string text;
	};

	typedef std::vector<Message> MessageList;
}