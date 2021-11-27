#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island
{
	struct DarkAlley
	{
		double infamyRequirement;
		double ruffianBrawlingStrength;
		double minimumWager;

		static void Clear();
		static void Write(int, const DarkAlley&);
		static std::optional<DarkAlley> Read(int);
	};

}