#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include <Data.Game.World.Month.h>
#include <format>
#include "Game.Session.World.Calendar.h"
namespace game::session::world
{
	static const std::map<std::string, size_t> consonants =
	{
		{"b",1},
		{"v",1},
		{"p",1},
		{"f",1},
		{"m",1},
		{"n",1},
		{"l",1},
		{"r",1}
	};

	static const std::map<std::string, size_t> vowels =
	{
		{"a",10},
		{"e",10},
		{"i",1},
		{"o",10},
		{"u",10}
	};

	static const std::map<size_t, size_t> nameLengths =
	{
		{3,1},
		{4,2},
		{5,2},
		{6,1},
		{7,2},
		{8,3},
		{9,1}
	};

	static const common::NameGenerator nameGenerator =
	{
		nameLengths,
		{ {false, 1}, {true,1} },
		vowels,
		consonants
	};

	static const std::map<int, size_t> lengthGenerator =
	{
		{25,1},
		{26,2},
		{27,3},
		{28,4},
		{29,5},
		{30,6},
		{31,5},
		{32,4},
		{33,3},
		{34,2},
		{35,1},
	};

	int Calendar::GetYearLength() const
	{
		return data::game::world::Month::YearLength(worldId);
	}

	std::string Calendar::GetDate() const
	{
		auto yearLength = GetYearLength();
		auto dayInWorld = data::game::World::Read(worldId).value().day;
		auto year = dayInWorld / yearLength;
		auto dayOfYear = dayInWorld % yearLength;
		auto months = GetMonths().GetAll();
		for (auto month : months)
		{
			auto daysInMonth = month.GetDays();
			if (dayOfYear < daysInMonth)
			{
				return std::format("Date: {} {}, {}", month.GetName(), dayOfYear + 1, year + 1);
			}
			else
			{
				dayOfYear -= daysInMonth;
			}
		}
		throw "YOU SHOULD NOT GET HERE!";
	}

	void Calendar::Reset(const Difficulty&) const
	{
		data::game::world::Month::Clear(worldId);
		int total = 0;
		int ordinal = 1;
		while (total < 350)
		{
			auto name = nameGenerator.Generate();
			auto length = common::RNG::FromGenerator(lengthGenerator);
			total += length;
			data::game::world::Month::Write(worldId, 
				{
					ordinal++,
					name,
					length
				});
		}
	}
}