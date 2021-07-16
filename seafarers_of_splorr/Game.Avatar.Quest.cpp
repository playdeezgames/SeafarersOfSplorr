#include "Game.Avatar.Quest.h"
#include "Data.Game.Avatar.Quest.h"
#include "Data.Game.Island.Quests.h"
#include "Game.Avatar.Statistics.h"
#include "Data.Game.Island.Known.h"
#include "Game.Islands.h"
namespace game::avatar::Quest
{
	AcceptQuestResult AcceptQuest(const common::XY<double>& location)
	{
		if (data::game::avatar::Quest::Read())
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::Quests::Read(location);
		if (quest)
		{
			data::game::avatar::Quest::Write(
				std::optional<data::game::avatar::Quest::QuestData>({
					quest.value().destination,
					quest.value().reward,
					quest.value().itemName,
					quest.value().personName,
					quest.value().professionName}));
			data::game::island::Quests::Clear(location);
			game::Islands::SetKnown(quest.value().destination,(int)game::avatar::Statistics::GetCurrent(game::avatar::Statistic::TURNS_REMAINING));
			data::game::island::Known::Write(quest.value().destination);
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	bool CompleteQuest(const common::XY<double>& location)
	{
		auto quest = data::game::avatar::Quest::Read();
		if (quest.has_value() && quest.value().destination == location)
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::MONEY, quest.value().reward);
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::REPUTATION, 1.0);
			data::game::avatar::Quest::Write(std::nullopt);
			return true;
		}
		return false;
	}

	bool AbandonQuest()
	{
		if (data::game::avatar::Quest::Read())
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::REPUTATION, -1.0);//TODO: hardcoded value
			data::game::avatar::Quest::Write(std::nullopt);
			return true;
		}
		return false;
	}

	std::optional<game::Quest::QuestModel> Read()
	{
		auto quest = data::game::avatar::Quest::Read();
		if (quest)
		{
			return std::optional<game::Quest::QuestModel>({
				quest.value().destination,
				quest.value().reward,
				quest.value().itemName,
				quest.value().personName,
				quest.value().professionName});
		}
		return std::nullopt;
	}

}