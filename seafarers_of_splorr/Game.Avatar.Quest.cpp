#include "Game.Avatar.Quest.h"
#include "Data.Game.Avatar.Quest.h"
#include "Data.Game.Island.Quests.h"
#include "Game.Avatar.Statistics.h"
namespace game::avatar::Quest
{
	bool AcceptQuest(const common::XY<double>& location)
	{
		if (data::game::avatar::Quest::Read())
		{
			return false;//already have a quest
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
			return true;
		}
		return false;
	}

	bool CompleteQuest(const common::XY<double>& location)
	{
		auto quest = data::game::avatar::Quest::Read();
		if (quest.has_value() && quest.value().destination == location)
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::MONEY, quest.value().reward);
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