#include <Common.RNG.h>
#include <Data.Game.Character.PlightLegacy.h>
#include "Game.Characters.Plights.h"
#include <map>
namespace game::characters
{
	struct PlightDescriptor
	{
		std::string name;
		PlightType type;
		std::optional<size_t> durationMinimum;
		std::optional<size_t> durationMaximum;
		size_t generationWeight;
	};

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

	static const PlightDescriptor& Read(const Plight& plight)
	{
		return descriptors.find(plight)->second;
	}


	bool Plights::Has(int characterId, const Plight& plight)
	{
		return data::game::character::PlightLegacy::ReadDuration(characterId, (int)plight).has_value();
	}

	void Plights::ApplyTurnEffects(int characterId)
	{
		auto plights = data::game::character::PlightLegacy::All(characterId);
		for (auto& plight : plights)
		{
			auto duration = data::game::character::PlightLegacy::ReadDuration(characterId, plight);
			if (duration)
			{
				duration.value()--;
				if (duration.value() <= 0)
				{
					data::game::character::PlightLegacy::ClearPlight(characterId, plight);
					continue;
				}
				data::game::character::PlightLegacy::Write(characterId, plight, duration.value());
			}
		}
	}

	Plight Plights::Generate(const PlightType& type)
	{
		std::map<Plight, size_t> generator;
		for (auto& descriptor : descriptors)
		{
			if (descriptor.second.type == type)
			{
				generator[descriptor.first] = descriptor.second.generationWeight;
			}
		}
		return common::RNG::FromGenerator(generator);
	}

	void Plights::Inflict(int characterId, const Plight& plight)
	{
		auto& descriptor = Read(plight);
		std::optional<size_t> duration = std::nullopt;
		if (descriptor.durationMinimum)
		{
			duration = descriptor.durationMinimum;
			if (descriptor.durationMaximum)
			{
				duration = common::RNG::FromRange(descriptor.durationMinimum.value(), descriptor.durationMaximum.value() + 1);
			}
		}

		if (duration)
		{
			data::game::character::PlightLegacy::Write(
				characterId, 
				(int)plight, 
				(int)duration.value());
		}
	}

	std::set<Plight> Plights::InflictedWith(int characterId)
	{
		std::set<Plight> result;
		auto plights = data::game::character::PlightLegacy::All(characterId);
		for (auto& plight : plights)
		{
			result.insert((Plight)plight);
		}
		return result;
	}

	void Plights::Reset()
	{
		data::game::character::PlightLegacy::ClearAll();
	}

	const std::string& Plights::GetName(const Plight& plight)
	{
		return Read(plight).name;
	}

	PlightType Plights::GetType(const Plight& plight)
	{
		return Read(plight).type;
	}
}