#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Demigods]([DemigodId] INTEGER PRIMARY KEY AUTOINCREMENT,[Name] TEXT NOT NULL UNIQUE, [PatronWeight] INT NOT NULL,[BlessingThreshold] REAL NOT NULL,[BlessingMultiplier] REAL NOT NULL,[BlessingPlightId] INT NOT NULL,[CurseThreshold] REAL NOT NULL,[CurseMultiplier] REAL NOT NULL,[CursePlightId] INT NOT NULL,[OfferingCooldown] INT NOT NULL);";
	static const std::string INSERT_ITEM = "INSERT INTO [Demigods]([Name],[PatronWeight],[BlessingThreshold],[BlessingMultiplier],[BlessingPlightId],[CurseThreshold],[CurseMultiplier],[CursePlightId],[OfferingCooldown]) VALUES({},{},{},{},{},{},{},{},{});";
	static const std::string UPDATE_ITEM = "UPDATE [Demigods] SET [Name]={}, [PatronWeight]={}, [BlessingThreshold]={}, [BlessingMultiplier]={}, [BlessingPlightId]={}, [CurseThreshold]={}, [CurseMultiplier]={}, [CursePlightId]={}, [OfferingCooldown]={} WHERE [DemigodId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [Demigods];";
	static const std::string QUERY_ALL = "SELECT [DemigodId],[Name],[PatronWeight],[BlessingThreshold],[BlessingMultiplier],[BlessingPlightId],[CurseThreshold],[CurseMultiplier],[CursePlightId],[OfferingCooldown] FROM [Demigods];";
	static const std::string QUERY_ITEM = "SELECT [DemigodId],[Name],[PatronWeight],[BlessingThreshold],[BlessingMultiplier],[BlessingPlightId],[CurseThreshold],[CurseMultiplier],[CursePlightId],[OfferingCooldown] FROM [Demigods] WHERE [Name]={};";

	static const std::string FIELD_ID = "DemigodId";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_WEIGHT = "PatronWeight";
	static const std::string FIELD_BLESSING_THRESHOLD = "BlessingThreshold";
	static const std::string FIELD_BLESSING_MULTIPLIER = "BlessingMultiplier";
	static const std::string FIELD_BLESSING_PLIGHT_ID = "BlessingPlightId";
	static const std::string FIELD_CURSE_TRESHOLD = "CurseThreshold";
	static const std::string FIELD_CURSE_MULTIPLIER = "CurseMultiplier";
	static const std::string FIELD_CURSE_PLIGHT_ID = "CursePlightId";
	static const std::string FIELD_OFFERING_COOLDOWN = "OfferingCooldown";

	static const auto AutoCreateDemigodTable = data::game::Common::Run(CREATE_TABLE);

	void Demigod::Clear()
	{
		AutoCreateDemigodTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Demigod::Write(const Demigod& demigod)
	{
		AutoCreateDemigodTable();
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
				demigod.cursePlightId,
				demigod.offeringCooldown);
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
				demigod.offeringCooldown,
				demigod.id);
		}
	}

	static Demigod ToDemigod(const std::map<std::string, std::string>& record)
	{
		return {
				common::Data::ToInt(record.find(FIELD_ID)->second),
				record.find(FIELD_NAME)->second,
				(size_t)common::Data::ToInt(record.find(FIELD_PATRON_WEIGHT)->second),
				common::Data::ToDouble(record.find(FIELD_BLESSING_THRESHOLD)->second),
				common::Data::ToDouble(record.find(FIELD_BLESSING_MULTIPLIER)->second),
				common::Data::ToInt(record.find(FIELD_BLESSING_PLIGHT_ID)->second),
				common::Data::ToDouble(record.find(FIELD_CURSE_TRESHOLD)->second),
				common::Data::ToDouble(record.find(FIELD_CURSE_MULTIPLIER)->second),
				common::Data::ToInt(record.find(FIELD_CURSE_PLIGHT_ID)->second),
				(size_t)common::Data::ToInt(record.find(FIELD_OFFERING_COOLDOWN)->second)
		};
	}

	std::list<Demigod> Demigod::All()
	{
		AutoCreateDemigodTable();
		std::list<Demigod> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(ToDemigod(record));
		}
		return result;
	}

	std::optional<Demigod> Demigod::Read(const std::string& name)
	{
		AutoCreateDemigodTable();
		auto records = Common::Execute(QUERY_ITEM, common::Data::QuoteString(name));
		if (!records.empty())
		{
			return ToDemigod(records.front());
		}
		return std::nullopt;
	}
}