#pragma once
#include "Visuals.Message.h"
namespace visuals::Messages
{
	bool HasMessage();
	Message Read();
	void Write(const Message&);
	void Reset();
}
