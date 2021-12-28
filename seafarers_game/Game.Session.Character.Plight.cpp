#include "Game.Session.Character.Plight.h"
#include <optional>
#include <map>
namespace game::session::character
{
	struct PlightDescriptor
	{
		std::string name;
		game::characters::PlightType type;
		std::optional<size_t> durationMinimum;
		std::optional<size_t> durationMaximum;
		size_t generationWeight;
	};

	static const std::map<game::characters::Plight, PlightDescriptor> descriptors =
	{
		{
			game::characters::Plight::AGING_IMMUNITY,
			{
				"Youth",
				game::characters::PlightType::BLESSING,
				25,
				50,
				1
			}
		},
		{
			game::characters::Plight::DOUBLE_AGING,
			{
				"Progeria",
				game::characters::PlightType::CURSE,
				25,
				50,
				1
			}
		},
		{
			game::characters::Plight::DOUBLE_HUNGER,
			{
				"Tapeworm",
				game::characters::PlightType::CURSE,
				25,
				50,
				1
			}
		},
		{
			game::characters::Plight::HUNGER_IMMUNITY,
			{
				"Satiety",
				game::characters::PlightType::BLESSING,
				25,
				50,
				1
			}
		}
	};

	void Plight::ApplyTurnEffects() const
	{
		throw "THIS DOESNT WORK";
	}

	std::string Plight::GetName() const
	{
		throw "THIS DOESNT WORK";
	}

	game::characters::PlightType Plight::GetType() const
	{
		throw "THIS DOESNT WORK";
	}
}