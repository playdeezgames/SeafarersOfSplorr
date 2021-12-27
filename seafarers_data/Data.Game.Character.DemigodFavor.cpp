#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.DemigodFavor.h"
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterDemigodFavor]
		(
			[CharacterId] INT NOT NULL,
			[DemigodId] INT NOT NULL,
			[Favor] REAL NOT NULL, 
			[OfferingCooldown] INT NOT NULL, 
			UNIQUE([CharacterId],[DemigodId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([DemigodId]) REFERENCES [Demigods]([DemigodId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterDemigodFavor]
		(
			[CharacterId],
			[DemigodId],
			[Favor],
			[OfferingCooldown]
		) 
		VALUES({},{},{},{});)"s;
	static const std::string QUERY_ITEM_FAVOR = 
		R"(SELECT 
			[Favor] 
		FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"s;
	static const std::string QUERY_ITEM_OFFERING_COOLDOWN = 
		R"(SELECT 
			[OfferingCooldown] 
		FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"s;
	static const std::string DELETE_ALL_FOR_CHARACTER = 
		R"(DELETE FROM [CharacterDemigodFavor] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [CharacterDemigodFavor];)"s;
	static const std::string UPDATE_ITEM_COLUMN =
		R"(UPDATE [CharacterDemigodFavor] 
		SET 
			[{}]={} 
		WHERE 
			[CharacterId]={} 
			AND [DemigodId]={};)"s;
	static const std::string QUERY_OFFERING_COOLDOWNS =
		R"(SELECT 
			[CharacterId],
			[DemigodId],
			[OfferingCooldown] 
		FROM [CharacterDemigodFavor];)"s;

	static const std::string FIELD_CHARACTER_ID = "CharacterId";
	static const std::string FIELD_DEMIGOD_ID = "DemigodId";
	static const std::string FIELD_FAVOR = "Favor";
	static const std::string FIELD_OFFERING_COOLDOWN = "OfferingCooldown";

	void DemigodFavor::Initialize()
	{
		Character::Initialize();
		Demigod::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void DemigodFavor::Write(int characterId, int demigodId, double favor, int offeringCooldown)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			demigodId, 
			favor,
			offeringCooldown);
	}

	std::optional<double> DemigodFavor::ReadFavor(int characterId, int demigodId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_FAVOR, characterId, demigodId);
		if (!records.empty())
		{
			return common::Data::ToOptionalDouble(records.front()[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	std::optional<int> DemigodFavor::ReadOfferingCooldown(int characterId, int demigodId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_OFFERING_COOLDOWN, characterId, demigodId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_OFFERING_COOLDOWN]);
		}
		return std::nullopt;
	}

	void DemigodFavor::WriteOfferingCooldown(int characterId, int demigodId, int cooldown)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_OFFERING_COOLDOWN, cooldown, characterId, demigodId);
	}


	void DemigodFavor::ClearAll()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::list<std::tuple<int, int, int>> DemigodFavor::ReadOfferingCooldowns()
	{
		Initialize();
		auto records = Common::Execute(QUERY_OFFERING_COOLDOWNS);
		std::list<std::tuple<int, int, int>> result;
		std::transform(
			records.begin(), 
			records.end(), 
			std::back_inserter(result), 
			[](Common::Record record) 
			{
				return std::make_tuple(
					Common::ToInt(record, FIELD_CHARACTER_ID),
					Common::ToInt(record, FIELD_DEMIGOD_ID),
					Common::ToInt(record, FIELD_OFFERING_COOLDOWN));
			});
		return result;
	}

	void DemigodFavor::WriteOfferingCooldowns(const std::list<std::tuple<int, int, int>>& updates)
	{
		Initialize();
		std::for_each(
			updates.begin(), 
			updates.end(), 
			[](const std::tuple<int, int, int>& update)
			{
				Common::Execute(
					UPDATE_ITEM_COLUMN, 
					FIELD_OFFERING_COOLDOWN, 
					std::get<2>(update), 
					std::get<0>(update), 
					std::get<1>(update));
			});

	}
}
