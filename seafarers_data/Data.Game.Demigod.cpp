#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Demigods]
		(
			[DemigodId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Name] TEXT NOT NULL UNIQUE, 
			[PatronWeight] INT NOT NULL
		);)"s;
	static const std::string INSERT_ITEM =
		R"(INSERT INTO [Demigods]
		(
			[Name],
			[PatronWeight]
		) 
		VALUES({},{});)"s;
	static const std::string UPDATE_ITEM =
		R"(UPDATE [Demigods] 
		SET 
			[Name]={}, 
			[PatronWeight]={} 
		WHERE 
			[DemigodId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [Demigods];)"s;
	static const std::string QUERY_ALL =
		R"(SELECT 
			[DemigodId]
		FROM [Demigods];)"s;
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[DemigodId],
			[Name],
			[PatronWeight]
		FROM [Demigods] 
		WHERE 
			[DemigodId]={};)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Demigods] 
		WHERE 
			[DemigodId]={};)"s;

	static const std::string FIELD_DEMIGOD_ID = "DemigodId";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_WEIGHT = "PatronWeight";

	void Demigod::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	void Demigod::Clear()
	{
		Initialize();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::list<Demigod::demigodid_t> Demigod::All()
	{
		Initialize();
		std::list<demigodid_t> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(Common::ToInt(record, FIELD_DEMIGOD_ID));
		}
		return result;
	}

	int Demigod::Create(const name_t& name, patronweight_t patronWeight)
	{
		Initialize();
		data::game::Common::Execute(
			INSERT_ITEM,
			common::Data::QuoteString(name),
			patronWeight);
		return Common::LastInsertedIndex();
	}

	std::optional<Demigod::name_t> Demigod::ReadName(demigodid_t demigodId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, demigodId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::optional<Demigod::patronweight_t> Demigod::ReadPatronWeight(demigodid_t demigodId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_PATRON_WEIGHT, demigodId);
		if (record)
		{
			return (size_t)Common::ToInt(record.value(), FIELD_PATRON_WEIGHT);
		}
		return std::nullopt;
	}
}