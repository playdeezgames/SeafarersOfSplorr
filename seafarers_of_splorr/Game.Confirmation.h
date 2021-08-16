#pragma once
#include <functional>
#include <string>
namespace game
{
	struct Confirmation
	{
		std::string prompt;
		std::function<void()> onYes;
		std::function<void()> onNo;
	};
}