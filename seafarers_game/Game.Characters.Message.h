#pragma once
#include <list>
#include <string>
namespace game::characters
{
	struct Message
	{
		std::string color;
		std::string text;
	};

	typedef std::list<Message> MessageList;
}