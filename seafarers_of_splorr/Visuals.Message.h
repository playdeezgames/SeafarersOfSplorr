#pragma once
#include "Visuals.MessageDetail.h"
#include <list>
namespace visuals
{
	struct Message
	{
		std::string caption;
		std::list<MessageDetail> details;
	};
}
