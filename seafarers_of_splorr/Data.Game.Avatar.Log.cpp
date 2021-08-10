#include "Data.Game.Avatar.Log.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Log
{
	const std::string FIELD_LOG_COLOR = "LogColor";
	const std::string FIELD_LOG_TEXT = "LogText";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarLogs]([LogId] INTEGER PRIMARY KEY AUTOINCREMENT, [AvatarId] INT NOT NULL, [LogColor] TEXT NOT NULL, [LogText] TEXT NOT NULL);";
	const std::string DELETE_ALL = "DELETE FROM [AvatarLogs] WHERE [AvatarId]={};";
	const std::string QUERY_ALL = "SELECT [LogColor], [LogText] FROM [AvatarLogs] WHERE [AvatarId]={} ORDER BY [LogId] DESC LIMIT {};";
	const std::string INSERT_ITEM = "INSERT INTO [AvatarLogs]([AvatarId],[LogColor],[LogText]) VALUES({},{},{});";

	static auto AutoCreateAvatarLogTable = data::game::Common::Run(CREATE_TABLE);

	void Clear()
	{
		AutoCreateAvatarLogTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}

	void Write(const std::string& color, const std::string& text)
	{
		AutoCreateAvatarLogTable();
		Common::Execute(std::format(INSERT_ITEM, Common::AVATAR_ID, Common::QuoteString(color), Common::QuoteString(text)));
	}

	std::list<std::tuple<std::string, std::string>> Read(size_t entryCount)
	{
		AutoCreateAvatarLogTable();
		std::list<std::tuple<std::string, std::string>> results;
		auto records = Common::Execute(std::format(QUERY_ALL, Common::AVATAR_ID, entryCount));
		for (auto record : records)
		{
			results.push_back(
				std::make_tuple(
					record[FIELD_LOG_COLOR],
					record[FIELD_LOG_TEXT]));
		}
		return results;
	}
}
