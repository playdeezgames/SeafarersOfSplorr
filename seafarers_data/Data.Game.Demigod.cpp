#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Demigods]([Name] TEXT NOT NULL UNIQUE, [PatronWeight] INT NOT NULL,[BlessingThreshold] REAL NOT NULL,[BlessingMultiplier] REAL NOT NULL,[BlessingPlightId] INT NOT NULL,[CurseThreshold] REAL NOT NULL,[CurseMultiplier] REAL NOT NULL,[CursePlightId] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Demigods]([Name],[PatronWeight],[BlessingThreshold],[BlessingMultiplier],[BlessingPlightId],[CurseThreshold],[CurseMultiplier],[CursePlightId]) VALUES({},{},{},{},{},{},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [Demigods];";
	static const std::string QUERY_ALL = "SELECT [Name],[PatronWeight],[BlessingThreshold],[BlessingMultiplier],[BlessingPlightId],[CurseThreshold],[CurseMultiplier],[CursePlightId] FROM [Demigods];";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_WEIGHT = "PatronWeight";
	static const std::string FIELD_BLESSING_THRESHOLD = "BlessingThreshold";
	static const std::string FIELD_BLESSING_MULTIPLIER = "BlessingMultiplier";
	static const std::string FIELD_BLESSING_PLIGHT_ID = "BlessingPlightId";
	static const std::string FIELD_CURSE_TRESHOLD = "CurseThreshold";
	static const std::string FIELD_CURSE_MULTIPLIER = "CurseMultiplier";
	static const std::string FIELD_CURSE_PLIGHT_ID = "CursePlightId";

	static const auto AutoCreateDemigodTable = data::game::Common::Run(CREATE_TABLE);

	void Demigod::Clear()
	{
		AutoCreateDemigodTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Demigod::Add(const Demigod& demigod)
	{
		AutoCreateDemigodTable();
		data::game::Common::Execute(
			std::format(REPLACE_ITEM, 
				common::Data::QuoteString(demigod.name), 
				demigod.patronWeight,
				demigod.blessingThreshold,
				demigod.blessingMultiplier,
				demigod.blessingPlightId,
				demigod.curseThreshold,
				demigod.curseMultiplier,
				demigod.cursePlightId));
	}

	std::list<Demigod> Demigod::All()
	{
		AutoCreateDemigodTable();
		std::list<Demigod> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back({
				record[FIELD_NAME],
				(size_t)common::Data::ToInt(record[FIELD_PATRON_WEIGHT]),
				common::Data::ToDouble(record[FIELD_BLESSING_THRESHOLD]),
				common::Data::ToDouble(record[FIELD_BLESSING_MULTIPLIER]),
				common::Data::ToInt(record[FIELD_BLESSING_PLIGHT_ID]),
				common::Data::ToDouble(record[FIELD_CURSE_TRESHOLD]),
				common::Data::ToDouble(record[FIELD_CURSE_MULTIPLIER]),
				common::Data::ToInt(record[FIELD_CURSE_PLIGHT_ID])
				});
		}
		return result;
	}

}