#pragma once
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Tribe.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Tribes]
		(
			[TribeId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Name] TEXT NOT NULL
		);)"s;
	static const std::string CREATE_ITEM =
		R"(INSERT INTO [Tribes]
		(
			[Name]
		) 
		VALUES({});)";
	static const std::string QUERY_ALL =
		R"(SELECT 
			[TribeId] 
		FROM [Tribes];)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Tribes] 
		WHERE 
			[TribeId]={};)";
	static const std::string DELETE_ALL =
		R"(DELETE FROM [Tribes];)"s;

	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_TRIBE_ID = "TribeId";

	void Tribe::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Tribe::Create(const std::string& name)
	{
		Initialize();
		Common::Execute(CREATE_ITEM, common::Data::QuoteString(name));
		return Common::LastInsertedIndex();
	}

	std::list<int> Tribe::All()
	{
		Initialize();
		std::list<int> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto record : records)
		{
			result.push_back(Common::ToInt(record, FIELD_TRIBE_ID));
		}
		return result;
	}

	std::optional<std::string> Tribe::ReadName(int tribeId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, tribeId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	void Tribe::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}
