#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Demigods]
		(
			[DemigodId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Name] TEXT NOT NULL UNIQUE, 
			[PatronWeight] INT NOT NULL
		);)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [Demigods]
		(
			[Name],
			[PatronWeight]
		) 
		VALUES({},{});)"sv;
	static constexpr std::string_view UPDATE_ITEM =
		R"(UPDATE [Demigods] 
		SET 
			[Name]={}, 
			[PatronWeight]={} 
		WHERE 
			[DemigodId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [Demigods];)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT 
			[DemigodId]
		FROM [Demigods];)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[DemigodId],
			[Name],
			[PatronWeight]
		FROM [Demigods] 
		WHERE 
			[DemigodId]={};)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Demigods] 
		WHERE 
			[DemigodId]={};)"sv;

	static constexpr std::string_view FIELD_DEMIGOD_ID = "DemigodId";
	static constexpr std::string_view FIELD_NAME = "Name";
	static constexpr std::string_view FIELD_PATRON_WEIGHT = "PatronWeight";

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
		std::vector<int> result;
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