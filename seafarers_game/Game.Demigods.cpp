#include <Common.RNG.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.DemigodItem.h>
#include "Game.Avatar.Plights.h"
#include "Game.Items.h"
#include "Game.Demigods.h"
#include <map>
#include <set>
#include <string>
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

	static std::string GenerateName()
	{
		//TODO: hardcoded
		size_t nameLength = 2 * (common::RNG::FromRange((size_t)1, (size_t)3) + common::RNG::FromRange((size_t)1, (size_t)3)) + 1;
		bool vowel = false;
		std::string result = "";
		while (nameLength > 0)
		{
			if (vowel)
			{
				result = result + common::RNG::FromGenerator(vowels, std::string(""));
			}
			else
			{
				result = result + common::RNG::FromGenerator(consonants, std::string(""));
			}
			vowel = !vowel;
			nameLength--;
		}
		return result;
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

	void Demigods::Reset(const Difficulty&)
	{
		data::game::Demigod::Clear();
		data::game::DemigodItem::Clear();
		auto demigodCount =
			common::RNG::FromRange(1, 3) +//TODO: hardcoded
			common::RNG::FromRange(1, 3) +//TODO: hardcoded
			common::RNG::FromRange(1, 3) +//TODO: hardcoded
			common::RNG::FromRange(1, 3);//TODO: hardcoded
		auto names = GenerateNames(demigodCount);
		auto items = Items::All();
		for (auto name : names)
		{
			data::game::Demigod::Add({
				name,
				(size_t)common::RNG::FromRange(1, 11),//TODO: hardcoded
				5.0,
				2.0,
				(int)avatar::Plights::Generate(avatar::PlightType::BLESSING),
				-5.0,
				0.5,
				(int)avatar::Plights::Generate(avatar::PlightType::CURSE)
				});
			for (auto item : items)
			{
				data::game::DemigodItem::Write(name, (int)item, common::RNG::FromRange(-1.0, 1.0));//TODO: hardcoded
			}
		}
	}

	void Demigods::MakeOffering(const std::string& demigod, const Item& item)
	{
		//TODO: get favor/disfavor amount for this item and this demigod
		//TODO: change favor with demigod for the avatar
		//TODO: if hits blessing threshold, give blessing and change next blessing threshold
		//TODO: if hits curse threshold, give curse and change next curse threshold
	}
}