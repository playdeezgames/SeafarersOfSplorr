#include <Common.RNG.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.DemigodItem.h>
#include "Game.Items.h"
#include "Game.Demigods.h"
#include <map>
#include <set>
#include <string>
namespace game
{
	static const std::map<std::string, size_t> consonants =
	{
		{"ch",1},
		{"th",1},
		{"zh",1},
		{"sh",1},
		{"l",1},
		{"r",1},
		{"z",1}
	};

	static const std::map<std::string, size_t> vowels =
	{
		{"'",1},
		{"a",1},
		{"e",1},
		{"i",1},
		{"o",1},
		{"u",1},
		{"y",1},
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
				(size_t)common::RNG::FromRange(1, 11)//TODO: hardcoded
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