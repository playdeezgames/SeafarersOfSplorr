#include <algorithm>
#include <Common.RNG.h>
#include <Common.NameGenerator.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Character.DemigodFavor.h>
#include <Data.Game.Character.h>
#include "Game.Session.Demigods.h"
#include <iterator>
#include <map>
#include <set>
namespace game::session
{
	using CharacterData = data::game::Character;
	using DemigodData = data::game::Demigod;
	using DemigodFavorData = data::game::character::DemigodFavor;

	void Demigods::Reset() const
	{
		DemigodData::Clear();
		DemigodFavorData::ClearAll();
	}

	static const std::map<std::string, size_t> consonants =
	{
		{"CH",1},
		{"TH",1},
		{"ZH",1},
		{"SH",1},
		{"PH",1},
		{"L",1},
		{"R",1},
		{"X",1},
		{"Q",1},
		{"Z",1}
	};

	static const std::map<std::string, size_t> vowels =
	{
		{"'",10},
		{"A",10},
		{"AA",1},
		{"E",10},
		{"I",10},
		{"II",1},
		{"O",10},
		{"U",10},
		{"UU",1},
		{"Y",10},
		{"YY",1},
	};

	static const std::map<size_t, size_t> nameLengths =
	{
		{5,1},
		{7,2},
		{9,1}
	};

	static const common::NameGenerator nameGenerator =
	{
		nameLengths,
		{ {false, 1} },
		vowels,
		consonants
	};

	static std::string GenerateName()
	{
		return nameGenerator.Generate();
	}

	static std::set<std::string> GenerateNames(size_t nameCount)
	{
		std::set<std::string> result;
		while (result.size() < nameCount)
		{
			result.insert(GenerateName());
		}
		return result;
	}

	static const std::map<int, size_t> demigodCounts =
	{
		{4,1},
		{5,4},
		{6,10},
		{7,16},
		{8,19},
		{9,16},
		{10,10},
		{11,4},
		{12,1}
	};

	static const std::map<size_t, size_t> patronWeights =
	{
		{1,1},
		{2,1},
		{3,1},
		{4,1},
		{5,1},
		{6,1},
		{7,1},
		{8,1},
		{9,1},
		{10,1}
	};

	void Demigods::Populate(const Difficulty& difficulty) const
	{
		auto demigodCount = common::RNG::FromGenerator(demigodCounts);
		auto names = GenerateNames(demigodCount);
		std::for_each(
			names.begin(),
			names.end(),
			[](const std::string& name) 
			{
				DemigodData::Create(name,
					common::RNG::FromGenerator(patronWeights));
			});
	}

	std::list<Demigod> Demigods::GetAll() const
	{
		auto demigods = DemigodData::All();
		std::list<Demigod> result;
		std::transform(
			demigods.begin(),
			demigods.end(),
			std::back_inserter(result),
			[](int demigodId) 
			{
				return Demigod(demigodId);
			});
		return result;
	}
}