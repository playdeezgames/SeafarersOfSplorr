#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Character.DemigodFavor.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.DemigodItemLegacy.h>
#include "Game.Characters.Plights.h"
#include "Game.Items.h"
#include "Game.Demigods.h"
#include <map>
#include <set>
namespace game
{
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

	static bool ApplyBlessing(int characterId, data::game::Demigod& demigod, const Item& item, double favor)
	{
		if (favor >= demigod.blessingThreshold)
		{
			game::characters::Plights::Inflict(characterId, (game::characters::Plight)demigod.blessingPlightId);
			favor -= demigod.blessingThreshold;
			data::game::character::DemigodFavor::Write(characterId, demigod.id, favor, data::game::character::DemigodFavor::ReadOfferingCooldown(characterId, demigod.id).value_or(0));
			demigod.blessingThreshold *= demigod.blessingMultiplier;
			data::game::Demigod::Write(demigod);
			return true;
		}
		return false;
	}

	static bool ApplyCurse(int characterId, data::game::Demigod& demigod, const Item& item, double favor)
	{
		if (favor <= demigod.curseThreshold)
		{
			game::characters::Plights::Inflict(characterId, (game::characters::Plight)demigod.cursePlightId);
			favor -= demigod.curseThreshold;
			data::game::character::DemigodFavor::Write(characterId, demigod.id, favor, data::game::character::DemigodFavor::ReadOfferingCooldown(characterId, demigod.id).value_or(0));
			demigod.curseThreshold *= demigod.curseMultiplier;
			data::game::Demigod::Write(demigod);
			return true;
		}
		return false;
	}

	static OfferingResult ApplyFavor(int characterId, data::game::Demigod& demigod, const Item& item, double delta)
	{
		auto favor = data::game::character::DemigodFavor::ReadFavor(characterId, demigod.id).value_or(0.0);
		favor += delta;
		data::game::character::DemigodFavor::Write(characterId, demigod.id, favor, data::game::character::DemigodFavor::ReadOfferingCooldown(characterId, demigod.id).value_or(0));
		if (ApplyBlessing(characterId, demigod, item, favor))
		{
			return OfferingResult::BLESSING;
		}
		if (ApplyCurse(characterId, demigod, item, favor))
		{
			return OfferingResult::CURSE;
		}
		return OfferingResult::SUCCESS;
	}

	static void UpdateCooldown(int characterId, data::game::Demigod& demigod)
	{
		size_t coolDown = data::game::character::DemigodFavor::ReadOfferingCooldown(characterId, demigod.id).value_or(0);
		coolDown++;
		data::game::character::DemigodFavor::Write(characterId, demigod.id, data::game::character::DemigodFavor::ReadFavor(characterId, demigod.id).value_or(0.0) , (int)coolDown);
	}

	static OfferingResult DoMakeOffering(int characterId, data::game::Demigod& demigod, const Item& item)
	{
		auto delta = data::game::DemigodItemLegacy::Read(demigod.id, (int)item);
		if (delta)
		{
			UpdateCooldown(characterId, demigod);
			return ApplyFavor(characterId, demigod, item, delta.value());
		}
		return OfferingResult::SUCCESS;
	}

	static OfferingResult CheckOfferingCooldown(int characterId, data::game::Demigod& demigod, const Item& item)
	{
		if (data::game::character::DemigodFavor::ReadOfferingCooldown(characterId, demigod.id).value_or(0) > 0)
		{
			return OfferingResult::COOLING_DOWN;
		}
		return DoMakeOffering(characterId, demigod, item);
	}

	OfferingResult Demigods::MakeOffering(int characterId, int demigodId, const Item& item)
	{
		auto demigod = data::game::Demigod::Read(demigodId);
		if(demigod)
		{
			return CheckOfferingCooldown(characterId, demigod.value(), item);
		}
		return OfferingResult::FAILURE;
	}

	std::string Demigods::ReadName(int demigodId)
	{
		auto demigod = data::game::Demigod::Read(demigodId);
		return demigod.value().name;
	}

}