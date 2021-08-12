#pragma once
#include "Game.MessageDetail.h"
#include <list>
namespace game
{
	struct Message
	{
		std::string caption;
		std::list<MessageDetail> details;
	};
}
