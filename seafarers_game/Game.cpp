#include <Data.Game.World.h>
#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.h"
#include "Game.Characters.h"
#include "Game.Characters.Equipment.h"
#include "Game.Characters.Items.h"
#include "Game.Characters.Plights.h"
#include "Game.Characters.Ships.h"
#include "Game.Characters.Statistics.h"
#include "Game.Demigods.h"
#include "Game.Fisheries.h"
#include "Game.Islands.h"
#include "Game.Islands.Features.h"
#include "Game.Player.h"
#include "Game.Ships.h"
#include "Game.Ship.Docked.h"
#include "Game.World.h"
#include <list>
#include <map>
namespace game
{
	static std::list<std::function<void()>> resetters;
	static void AddResetter(std::function<void()> resetter)
	{
		resetters.push_back(resetter);
	}

	std::function<void()> DoAddResetter(std::function<void()> resetter)
	{
		return [resetter]()
		{
			AddResetter(resetter);
		};
	}

	void ResetLegacy(const Difficulty& difficulty)
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);

		World::Reset(difficulty);//must be done first to establish world size
		Ships::Reset(difficulty);
		Characters::Reset(difficulty);
		characters::Equipment::Reset(difficulty);
		characters::Items::Reset(Player::GetCharacterId(), difficulty);
		characters::Plights::Reset(difficulty);
		characters::Statistics::Reset(Player::GetCharacterId(), difficulty);
		Demigods::Reset(game::Player::GetCharacterId(), difficulty);
		Fisheries::Reset(difficulty);
		Islands::Reset(difficulty);
		islands::Features::Reset(difficulty);
		for (auto& resetter : resetters)
		{
			resetter();
		}
	}

	void AutoSave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, data::sqlite::Store::AUTOSAVE);
	}

	void LoadFromAutosave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::AUTOSAVE, data::sqlite::Store::IN_MEMORY);
	}

	static bool DoesWorldExistInStore(const data::sqlite::Store& store)
	{
		return !data::sqlite::Stores::Execute(store, "SELECT name FROM sqlite_master WHERE type='table' AND name='Worlds';").empty();
	}

	bool DoesAutosaveExist()
	{
		return DoesWorldExistInStore(data::sqlite::Store::AUTOSAVE);
	}

	const std::map<int, data::sqlite::Store> slotTable =
	{
		{1, data::sqlite::Store::SLOT_1},
		{2, data::sqlite::Store::SLOT_2},
		{3, data::sqlite::Store::SLOT_3},
		{4, data::sqlite::Store::SLOT_4},
		{5, data::sqlite::Store::SLOT_5}
	};

	bool DoesSlotExist(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			return DoesWorldExistInStore(iter->second);
		}
		return false;
	}

	void LoadFromSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(iter->second, data::sqlite::Store::IN_MEMORY);
		}
	}

	void SaveToSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, iter->second);
		}
	}

	static const std::list<std::function<void()>> turnEffects = 
	{
		Ships::ApplyTurnEffects,
		Characters::ApplyTurnEffects,
		Islands::ApplyTurnEffects,
		[]() { characters::Plights::ApplyTurnEffects(game::Player::GetCharacterId()); },
		[]() { Demigods::ApplyTurnEffects(game::Player::GetCharacterId()); },
		Fisheries::ApplyTurnEffects,
		World::ApplyTurnEffects,
	};

	void ApplyTurnEffects()
	{
		for (auto turnEffect : turnEffects)
		{
			turnEffect();
		}
	}
}