#pragma once
#include <list>
#include <string>
namespace game::characters
{
	//leave this here until it is used somewhere else also
	struct Message
	{
		std::string color;
		std::string text;
	};

	typedef std::list<Message> MessageList;

	struct Messages
	{
		static void Add(int, const std::string&, const std::string&);
		static void Add(int, const std::string&);
		static void Clear();
		static void Clear(int);
		static MessageList Read(int);
	};
}