#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island::dark_alley::GamblingHand
{
	struct Data
	{
		int firstCard;
		int secondCard;
		int thirdCard;
	};

	void Write(const common::XY<double>&, const Data&);
	std::optional<Data> Read(const common::XY<double>&);
}
