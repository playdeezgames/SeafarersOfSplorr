#include <Data.Game.Avatar.Plight.h>
#include "Game.Avatar.Plights.h"
#include <map>
namespace game::avatar
{
	static const std::map<Plight, PlightDescriptor> descriptors =
	{
		{
			Plight::AGING_IMMUNITY,
			{
				"Youth",
				PlightType::BLESSING,
				25,
				50,
				1
			}
		},
		{
			Plight::DOUBLE_AGING,
			{
				"Progeria",
				PlightType::CURSE,
				25,
				50,
				1
			}
		},
		{
			Plight::DOUBLE_HUNGER,
			{
				"Tapeworm",
				PlightType::CURSE,
				25,
				50,
				1
			}
		},
		{
			Plight::HUNGER_IMMUNITY,
			{
				"Satiety",
				PlightType::BLESSING,
				25,
				50,
				1
			}
		}
	};

	const PlightDescriptor& Plights::Read(const Plight& plight)
	{
		return descriptors.find(plight)->second;
	}


	bool Plights::Has(const Plight& plightId)
	{
		return data::game::avatar::Plight::Read((int)plightId).has_value();
	}

	void Plights::ApplyTurnEffects()
	{
		auto plights = data::game::avatar::Plight::All();
		for (auto& plight : plights)
		{
			if (plight.duration.has_value())
			{
				plight.duration = plight.duration.value() - 1;
				if (plight.duration.value() <= 0)
				{
					data::game::avatar::Plight::Clear(plight.plightId);
					continue;
				}
				data::game::avatar::Plight::Write(plight);
			}
		}
	}
}