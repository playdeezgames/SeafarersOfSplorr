#include "Common.NameGenerator.h"
#include "Common.RNG.h"
#include <sstream>
namespace common//20211015
{
	std::string NameGenerator::Generate() const
	{
		auto length = common::RNG::FromGenerator(lengthGenerator);
		auto isVowel = common::RNG::FromGenerator(vowelStartGenerator);
		std::stringstream ss;
		while (length > 0)
		{
			if (isVowel)
			{
				ss << common::RNG::FromGenerator(vowelGenerator);
			}
			else
			{
				ss << common::RNG::FromGenerator(consonantGenerator);
			}
			length--;
			isVowel = !isVowel;
		}
		return ss.str();;

	}
}