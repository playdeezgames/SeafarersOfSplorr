#pragma once
#include "Game.Difficulty.h"
#include <format>
#include <list>
#include <string>
namespace game::avatar
{
	struct Log
	{
		std::string color;
		std::string text;
		static void Reset(const Difficulty&);
		static std::list<Log> Read(size_t);
		static void Write(const Log&);
		template<typename ...Ts>
		static void Write(const std::string& color, const std::string& formatString, Ts... args)
		{
			Write({ color, std::format(formatString, args...) });
		}
	};
}
