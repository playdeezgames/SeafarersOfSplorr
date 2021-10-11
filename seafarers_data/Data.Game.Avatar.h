#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game//20211011
{
	struct Avatar 
	{
		int state;
		std::string name;

		static void Write(int, const Avatar&);
		static std::optional<Avatar> Read(int);

		static int NextId();
	};
}
