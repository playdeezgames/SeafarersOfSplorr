#include <Common.RNG.h>
#include <Data.Game.Avatar.Plight.h>
#include "Game.Avatar.Plights.h"
#include <Game.Player.h>
#include <map>
#include <string>
namespace game::avatar
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


	bool Plights::Has(const Plight& plightId)
	{
		return data::game::avatar::Plight::Read(Player::GetAvatarId(), (int)plightId).has_value();
	}

	void Plights::ApplyTurnEffects()
	{
		auto plights = data::game::avatar::Plight::All(Player::GetAvatarId());
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
				data::game::avatar::Plight::Write(Player::GetAvatarId(), plight);
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

	void Plights::Inflict(const Plight& plight)
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

		data::game::avatar::Plight::Write(Player::GetAvatarId(), {(int)plight, (duration)?(std::optional<int>((int)duration.value())):(std::nullopt) });
	}

	std::set<Plight> Plights::InflictedWith()
	{
		std::set<Plight> result;
		auto plights = data::game::avatar::Plight::All(Player::GetAvatarId());
		for (auto& plight : plights)
		{
			result.insert((Plight)plight.plightId);
		}
		return result;
	}

	void Plights::Reset(const Difficulty&)
	{
		data::game::avatar::Plight::ClearAll();
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