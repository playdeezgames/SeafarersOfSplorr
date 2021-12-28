#include <Common.RNG.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.QuestLegacy.h>
#include <format>
#include "Game.Islands.h"
#include "Game.Islands.Quests.h"
#include "Game.Session.h"
namespace game::islands
{
	static const double MAXIMUM_REWARD = 10.0;
	static const double DEFAULT_MINIMUM_REWARD = 1.0;
	static const double DEFAULT_MAXIMUM_REWARD = 1.0;
	static const double NEGATIVE_REWARD_RADIX = 2.0;
	static double GenerateReward(int characterId)
	{
		double reputation = 
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId)
			.GetCounters()
			.GetCounter(game::characters::Counter::REPUTATION)
			.GetValue();
		double minimum = DEFAULT_MINIMUM_REWARD;
		double maximum = DEFAULT_MAXIMUM_REWARD;
		if (reputation <= 0.0)
		{
			minimum = std::pow(NEGATIVE_REWARD_RADIX, reputation);
		}
		else
		{
			maximum = std::min(MAXIMUM_REWARD, std::sqrt(reputation));
		}
		return common::RNG::FromRange(minimum, maximum);
	}

	static const std::map<std::string, size_t> itemAdverbs = 
	{
		{"woefully",1},
		{"awfully",1},
		{"generally",1},
		{"executively",1},
		{"painfully",1},
		{"wickedly",1},
		{"nastily",1},
		{"evilly",1},
		{"gracefully",1},
		{"mercifully",1}
	};

	static const std::map<std::string, size_t> itemAdjectives =
	{
		{"turgid",1},
		{"odorous",1},
		{"crusty",1},
		{"rusty",1},
		{"musty",1},
		{"dingy",1},
		{"dirty",1},
		{"purple",1},
		{"tatty",1},
		{"Belgian",1},
		{"throbbing",1},
		{"pulsating",1},
		{"writhing",1},
		{"bulging",1},
		{"chewy",1},
		{"swollen",1}
	};

	static const std::map<std::string, size_t> itemNames =
	{
		{"marital aid",1},
		{"bed pan",1},
		{"skeet shooter",1},
		{"poo bucket",1},
		{"chastity belt",1},
		{"nipple clamp",1},
		{"beer bottle",1},
		{"wine glass",1},
		{"speculum",1},
		{"waffle maker",1},
		{"fleshlight",1},
		{"biscuit",1},
		{"marmite",1},
		{"vegemite",1},
		{"transfunctioner",1}
	};

	static std::string GenerateItemName()
	{
		return std::format("{} {} {}",
			common::RNG::FromGenerator(itemAdverbs),
			common::RNG::FromGenerator(itemAdjectives),
			common::RNG::FromGenerator(itemNames));
	}

	static const std::map<std::string, size_t> peopleNames =
	{
		{"Samuel",1},
		{"Roberta",1},
		{"Davin",1},
		{"Shaniqua",1},
		{"Yermom",1},
		{"David",1},
		{"Sander",1},
		{"Marcus",1},
		{"Barnaby",1},
		{"James",1},
		{"Ravi",1},
		{"Michael",1},
		{"Guy",1},
		{"Tim",1}
	};

	static std::string GeneratePersonName()
	{
		return common::RNG::FromGenerator(peopleNames);
	}

	static const std::map<std::string, size_t> professionAdjectives =
	{
		{"lousy",1},
		{"smelly",1},
		{"randy",1},
		{"flatulant",1},
		{"clumsy",1},
		{"grumpy",1},
		{"ugly",1},
		{"miserly",1},
		{"large-breasted",1},
		{"well-endowed",1},
		{"poop eating",1},
		{"stupid",1},
		{"charming",1},
		{"rabid",1}
	};

	static const std::map<std::string, size_t> professionNames =
	{
		{"harlot",1},
		{"innkeeper",1},
		{"peasant",1},
		{"leper",1},
		{"merkinsmith",1},
		{"plague doctor",1},
		{"neer-do-well",1},
		{"malcontent",1},
		{"freebooter",1},
		{"pirate",1},
		{"scoundrel",1},
		{"gambler",1},
		{"gravedigger",1},
		{"bogan",1},
		{"dog",1},
		{"knacker",1},
		{"mudlark",1},
		{"gong farmer",1},
		{"ox-wanker",1}
	};

	static const std::map<std::string, size_t> receiptAdverbs =
	{
		{"horribly",1},
		{"incredibly",1},
		{"barely",1},
		{"extremely",1},
		{"fanatically",1},
		{"visibly",1},
		{"uncannily",1},
		{"unnervingly",1},
		{"creepily",1}
	};

	static const std::map<std::string, size_t> receiptAdjectives =
	{
		{"surprised",1},
		{"aroused",1},
		{"angry",1},
		{"elated",1},
		{"nonchalant",1},
		{"apathetic",1},
		{"happy",1},
		{"grumpy",1},
		{"sad",1},
		{"disappointed",1},
		{"surly",1},
		{"indifferent",1},
		{"miffed",1}
	};

	static std::string GenerateProfessionName()
	{
		return std::format("{} {}",
			common::RNG::FromGenerator(professionAdjectives),
			common::RNG::FromGenerator(professionNames));
	}

	static std::string GenerateReceiptEmotion()
	{
		return std::format("{} {}",
			common::RNG::FromGenerator(receiptAdverbs),
			common::RNG::FromGenerator(receiptAdjectives));
	}

	static int GenerateDestination(int islandId)
	{
		auto islandIds = data::game::Island::All();
		islandIds.remove_if([islandId](int candidateId) { return candidateId == islandId; });
		return common::RNG::FromList(islandIds).value();
	}

	void Quests::Update(int characterId, int fromIslandId)
	{
		if (!data::game::island::QuestLegacy::Read(fromIslandId).has_value())
		{
			data::game::island::QuestLegacy::Write(
				{
					fromIslandId,
					GenerateDestination(fromIslandId),
					GenerateReward(characterId),
					GenerateItemName(),
					GeneratePersonName(),
					GenerateProfessionName(),
					GenerateReceiptEmotion()
				});
		}
	}

	static Quest ToQuest(const data::game::island::QuestLegacy& quest)
	{
		auto location = data::game::Island::ReadLocation(quest.toIslandId).value();
		return
			{
				quest.toIslandId,
				location,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName,
				quest.receiptEmotion
			};
	}

	std::optional<game::Quest> Quests::Read(int fromIslandId)
	{
		return 
			common::utility::Optional::Map<data::game::island::QuestLegacy, Quest>(
				data::game::island::QuestLegacy::Read(fromIslandId),
				ToQuest);
	}
}