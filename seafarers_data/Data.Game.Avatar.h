#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::Avatar
{
	struct AvatarData
	{
		common::XY<double> location;
		double heading;
		double speed;
		int state;
	};

	void Write(const AvatarData&);
	std::optional<AvatarData> Read();
}
