#include <Data.Game.Character.Plight.h>
#include "Game.Session.Character.Plight.h"
#include <map>
namespace game::session::character
{
	struct PlightDescriptor
	{
		std::string name;
		game::characters::PlightType type;
	};

	static const std::map<game::characters::Plight, PlightDescriptor> descriptors =
	{
		{
			game::characters::Plight::AGING_IMMUNITY,
			{
				"Youth",
				game::characters::PlightType::BLESSING
			}
		},
		{
			game::characters::Plight::DOUBLE_AGING,
			{
				"Progeria",
				game::characters::PlightType::CURSE
			}
		},
		{
			game::characters::Plight::DOUBLE_HUNGER,
			{
				"Tapeworm",
				game::characters::PlightType::CURSE
			}
		},
		{
			game::characters::Plight::HUNGER_IMMUNITY,
			{
				"Satiety",
				game::characters::PlightType::BLESSING
			}
		},
		{
			game::characters::Plight::FOOD_POISONING,
			{
				"Food Poisoning",
				game::characters::PlightType::CURSE
			}
		}
	};

	using PlightData = data::game::character::Plight;

	void Plight::ApplyTurnEffects() const
	{
		auto duration = PlightData::ReadDuration(characterId, (int)plight);
		if (duration)
		{
			if (duration.value() > 0)
			{
				PlightData::Write(characterId, (int)plight, duration.value() - 1);
			}
			else
			{
				PlightData::ClearPlight(characterId, (int)plight);
			}
		}
	}

	std::string Plight::GetName() const
	{
		return descriptors.find(plight)->second.name;
	}

	game::characters::PlightType Plight::GetType() const
	{
		return descriptors.find(plight)->second.type;
	}
}