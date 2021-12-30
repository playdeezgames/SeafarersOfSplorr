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
			[PatronWeight] INT NOT NULL,
			[BlessingThreshold] REAL NOT NULL,
			[BlessingMultiplier] REAL NOT NULL,
			[BlessingPlightId] INT NOT NULL,
			[CurseThreshold] REAL NOT NULL,
			[CurseMultiplier] REAL NOT NULL,
			[CursePlightId] INT NOT NULL
		);)"s;
	static const std::string INSERT_ITEM =
		R"(INSERT INTO [Demigods]
		(
			[Name],
			[PatronWeight],
			[BlessingThreshold],
			[BlessingMultiplier],
			[BlessingPlightId],
			[CurseThreshold],
			[CurseMultiplier],
			[CursePlightId]
		) 
		VALUES({},{},{},{},{},{},{},{});)"s;
	static const std::string UPDATE_ITEM =
		R"(UPDATE [Demigods] 
		SET 
			[Name]={}, 
			[PatronWeight]={}, 
			[BlessingThreshold]={}, 
			[BlessingMultiplier]={}, 
			[BlessingPlightId]={}, 
			[CurseThreshold]={}, 
			[CurseMultiplier]={}, 
			[CursePlightId]={} 
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
			[PatronWeight],
			[BlessingThreshold],
			[BlessingMultiplier],
			[BlessingPlightId],
			[CurseThreshold],
			[CurseMultiplier],
			[CursePlightId] 
		FROM [Demigods] 
		WHERE 
			[DemigodId]={};)"s;

	static const std::string FIELD_DEMIGOD_ID = "DemigodId";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_WEIGHT = "PatronWeight";
	static const std::string FIELD_BLESSING_THRESHOLD = "BlessingThreshold";
	static const std::string FIELD_BLESSING_MULTIPLIER = "BlessingMultiplier";
	static const std::string FIELD_BLESSING_PLIGHT_ID = "BlessingPlightId";
	static const std::string FIELD_CURSE_TRESHOLD = "CurseThreshold";
	static const std::string FIELD_CURSE_MULTIPLIER = "CurseMultiplier";
	static const std::string FIELD_CURSE_PLIGHT_ID = "CursePlightId";

	void Demigod::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	void Demigod::Clear()
	{
		Initialize();
		data::game::Common::Execute(DELETE_ALL);
	}

	int Demigod::Write(const Demigod& demigod)
	{
		Initialize();
		if (demigod.id == 0)
		{
			data::game::Common::Execute(
				INSERT_ITEM,
				common::Data::QuoteString(demigod.name),
				demigod.patronWeight,
				demigod.blessingThreshold,
				demigod.blessingMultiplier,
				demigod.blessingPlightId,
				demigod.curseThreshold,
				demigod.curseMultiplier,
				demigod.cursePlightId);
			return Common::LastInsertedIndex();
		}
		else
		{
			data::game::Common::Execute(
				UPDATE_ITEM,
				common::Data::QuoteString(demigod.name),
				demigod.patronWeight,
				demigod.blessingThreshold,
				demigod.blessingMultiplier,
				demigod.blessingPlightId,
				demigod.curseThreshold,
				demigod.curseMultiplier,
				demigod.cursePlightId,
				demigod.id);
			return demigod.id;
		}
	}

	static Demigod ToDemigod(const std::map<std::string, std::string>& record)
	{
		return {
				common::Data::ToInt(record.find(FIELD_DEMIGOD_ID)->second),
				record.find(FIELD_NAME)->second,
				(size_t)common::Data::ToInt(record.find(FIELD_PATRON_WEIGHT)->second),
				common::Data::ToDouble(record.find(FIELD_BLESSING_THRESHOLD)->second),
				common::Data::ToDouble(record.find(FIELD_BLESSING_MULTIPLIER)->second),
				common::Data::ToInt(record.find(FIELD_BLESSING_PLIGHT_ID)->second),
				common::Data::ToDouble(record.find(FIELD_CURSE_TRESHOLD)->second),
				common::Data::ToDouble(record.find(FIELD_CURSE_MULTIPLIER)->second),
				common::Data::ToInt(record.find(FIELD_CURSE_PLIGHT_ID)->second)
		};
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

	std::optional<Demigod> Demigod::Read(int demigodId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, demigodId);
		if (!records.empty())
		{
			return ToDemigod(records.front());
		}
		return std::nullopt;
	}
}