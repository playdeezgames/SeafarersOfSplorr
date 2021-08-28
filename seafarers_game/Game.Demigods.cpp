#include <Common.RNG.h>
#include <Data.Game.Demigod.h>
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
		size_t nameLength = (size_t)(2 * (common::RNG::FromRange(1, 3) + common::RNG::FromRange(1, 3)) + 1);
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
		auto demigodCount =
			common::RNG::FromRange(1, 3) +
			common::RNG::FromRange(1, 3) +
			common::RNG::FromRange(1, 3) +
			common::RNG::FromRange(1, 3);
		auto names = GenerateNames(demigodCount);
		for (auto name : names)
		{
			data::game::Demigod::Add({
				name,
				(size_t)common::RNG::FromRange(1, 11)
				});
		}
	}
}