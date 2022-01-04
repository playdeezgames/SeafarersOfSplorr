#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include <iterator>
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

	std::vector<int> Demigod::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::vector<int> result(records.size());
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_DEMIGOD_ID));
		return result;
	}

	int Demigod::Create(const std::string& name, size_t patronWeight)
	{
		Initialize();
		data::game::Common::Execute(
			INSERT_ITEM,
			common::Data::QuoteString(name),
			patronWeight);
		return Common::LastInsertedIndex();
	}

	std::optional<std::string> Demigod::ReadName(int demigodId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, demigodId),
			FIELD_NAME);
	}

	std::optional<size_t> Demigod::ReadPatronWeight(int demigodId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_PATRON_WEIGHT, demigodId),
			FIELD_PATRON_WEIGHT);
	}
}