#pragma once
#include "Game.Difficulty.h"
#include <list>
#include <string>
namespace game::avatar::Log
{
	struct LogEntry
	{
		std::string color;
		std::string text;
	};
	void Reset(const Difficulty&);
	std::list<LogEntry> Read(size_t);
	void Write(const LogEntry&);
}
