#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	class Avatar 
	{
	public:
		struct Data
		{
			common::XY<double> location;
			double heading;
			double speed;
			int state;
		};
		static void Write(const Data&);
		static std::optional<Data> Read();
		static void SetState(int);
		static std::optional<int> GetState();
	};
}
