#include <algorithm>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Tribe.h>
#include "Game.Session.Tribes.h"
#include <iterator>
#include <set>
namespace game::session
{
	using TribeData = data::game::Tribe;
	std::vector<Tribe> Tribes::GetAll() const
	{
		auto tribeIds = TribeData::All();
		std::vector<Tribe> result;
		std::transform(
			tribeIds.begin(),
			tribeIds.end(),
			std::back_inserter(result),
			Tribe::FromTribeId);
		return result;
	}

	static const std::map<std::string, size_t> consonants =
	{
		{"g",1},
		{"k",1},
		{"m",1},
		{"n",1},
		{"s",1},
		{"z",1}
	};

	static const std::map<std::string, size_t> vowels =
	{
		{"a",1},
		{"e",1},
		{"i",1},
		{"o",1},
		{"u",1}
	};

	static const std::map<size_t, size_t> nameLengths =
	{
		{5, 1},
		{6, 5},
		{7, 10},
		{8, 10},
		{9, 5},
		{10, 1}
	};

	static const common::NameGenerator nameGenerator =
	{
		nameLengths,
		{ 
			{false, 1},
			{true,1}
		},
		vowels,
		consonants
	};

	static const std::map<Difficulty, std::map<int, size_t>> tribeCountGenerators =
	{
		{ Difficulty::EASY,
		{
			{2, 1},
			{3, 2},
			{4, 1}
		}},
		{ Difficulty::NORMAL,
		{
			{3, 1},
			{4, 3},
			{5, 3},
			{6, 1}
		}},
		{ Difficulty::HARD,
		{
			{4, 1},
			{5, 4},
			{6, 6},
			{7, 4},
			{8, 1}
		}},
		{ Difficulty::HARDCORE,
		{
			{5, 1},
			{6, 5},
			{7, 10},
			{8, 10},
			{9, 5},
			{10, 1}
		}},
	};

	void Tribes::Populate(const Difficulty& difficulty) const
	{
		auto tribeCount = 
			common::RNG::FromGenerator(
				tribeCountGenerators.find(difficulty)->second);
		std::set<std::string> names;
		while (names.size() < tribeCount)
		{
			names.insert(nameGenerator.Generate());
		}
		std::for_each(
			names.begin(), 
			names.end(), 
			TribeData::Create);
	}

	void Tribes::Reset() const
	{
		TribeData::Clear();
	}
}