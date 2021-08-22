#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island::DarkAlley
{
	struct DarkAlleyData
	{
		double infamyRequirement;
		double ruffianBrawlingStrength;
		double minimumWager;
	};

	void Clear();
	void Write(const common::XY<double>&, const DarkAlleyData&);
	std::optional<DarkAlleyData> Read(const common::XY<double>&);
}