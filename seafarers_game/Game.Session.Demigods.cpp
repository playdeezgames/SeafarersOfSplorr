#include <algorithm>
#include <Common.RNG.h>
#include <Common.NameGenerator.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.DemigodItemLegacy.h>
#include <Data.Game.Character.DemigodFavor.h>
#include <Data.Game.Character.h>
#include "Game.Characters.Plights.h"
#include "Game.Demigods.h"
#include "Game.Items.h"
#include "Game.Session.Demigods.h"
#include <map>
#include <set>
namespace game::session
{
	void Demigods::Reset() const
	{
		data::game::Demigod::Clear();
		data::game::DemigodItemLegacy::Clear();
		data::game::character::DemigodFavor::ClearAll();
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
		const double BLESSING_THRESHOLD = 5.0;
		const double BLESSING_MULTIPLIER = 2.0;
		const double CURSE_THRESHOLD = -5.0;
		const double CURSE_MULTIPLIER = 0.5;
		const double OFFERING_FAVOR_MINIMUM = -1.0;
		const double OFFERING_FAVOR_MAXIMUM = 1.0;
		auto demigodCount = common::RNG::FromGenerator(demigodCounts);
		auto names = GenerateNames(demigodCount);
		auto items = Items::All();
		for (auto name : names)
		{
			data::game::Demigod demigod = {
				0,
				name,
				common::RNG::FromGenerator(patronWeights),
				BLESSING_THRESHOLD,
				BLESSING_MULTIPLIER,
				(int)characters::Plights::Generate(characters::PlightType::BLESSING),
				CURSE_THRESHOLD,
				CURSE_MULTIPLIER,
				(int)characters::Plights::Generate(characters::PlightType::CURSE) };
			auto demigodId = data::game::Demigod::Write(demigod);
			for (auto item : items)
			{
				data::game::DemigodItemLegacy::Write(demigodId, (int)item, common::RNG::FromRange(OFFERING_FAVOR_MINIMUM, OFFERING_FAVOR_MAXIMUM));
			}
		}

	}

	using CharacterData = data::game::Character;
	using DemigodData = data::game::Demigod;
	using DemigodFavorData = data::game::character::DemigodFavor;

	void Demigods::ApplyTurnEffects() const
	{
		auto candidates = DemigodFavorData::ReadOfferingCooldowns();
		std::for_each(
			candidates.begin(), 
			candidates.end(), 
			[](DemigodFavorData::Cooldown& candidate)
			{
				if (candidate.cooldown > 0)
				{
					candidate.cooldown--;
				}
			});
	}
}