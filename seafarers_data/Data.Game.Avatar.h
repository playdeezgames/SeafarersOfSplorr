#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game
{
	struct Avatar 
	{
		int state;
		std::string name;

		static void Write(int, const Avatar&);
		static std::optional<Avatar> Read(int);
		static void WriteState(int, int);
		static std::optional<int> ReadState(int);

		static int NextId();
	};
}
