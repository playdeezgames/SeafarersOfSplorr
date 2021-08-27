#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Demigod.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Demigods]([Name] TEXT NOT NULL UNIQUE, [PatronWeight] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Demigods]([Name],[PatronWeight]) VALUES({},{});";
	static const std::string DELETE_ALL = "DELETE FROM [Demigods];";
	static const std::string QUERY_ALL = "SELECT [Name],[PatronWeight] FROM [Demigods];";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_PATRON_WEIGHT = "PatronWeight";

	static const auto AutoCreateDemigodTable = data::game::Common::Run(CREATE_TABLE);

	void Demigod::Clear()
	{
		AutoCreateDemigodTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Demigod::Add(const Demigod& demigod)
	{
		AutoCreateDemigodTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, common::Data::QuoteString(demigod.name), demigod.patronWeight));
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
				(size_t)common::Data::ToInt(record[FIELD_PATRON_WEIGHT])
				});
		}
		return result;
	}

}