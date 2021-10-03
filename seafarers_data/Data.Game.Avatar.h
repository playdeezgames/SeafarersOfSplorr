#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game
{
	struct Avatar 
	{
		common::XY<double> location;
		double heading;
		double speed;
		int state;
		std::string name;

		static void Write(int, const Avatar&);
		static std::optional<Avatar> Read(int);
		static void WriteState(int, int);
		static std::optional<int> ReadState(int);

		static int NextId();

		static void Write(const Avatar&);
		static std::optional<Avatar> Read();
		static void WriteState(int);
		static std::optional<int> ReadState();
	};
}
