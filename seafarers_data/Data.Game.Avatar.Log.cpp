#include <Common.Data.h>
#include "Data.Game.Avatar.Log.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
namespace data::game::avatar//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarLogs]([LogId] INTEGER PRIMARY KEY AUTOINCREMENT, [AvatarId] INT NOT NULL, [LogColor] TEXT NOT NULL, [LogText] TEXT NOT NULL);";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarLogs] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL_LOGS = "DELETE FROM [AvatarLogs];";
	static const std::string QUERY_ALL = "SELECT [LogColor], [LogText] FROM [AvatarLogs] WHERE [AvatarId]={} ORDER BY [LogId] DESC LIMIT {};";
	static const std::string INSERT_ITEM = "INSERT INTO [AvatarLogs]([AvatarId],[LogColor],[LogText]) VALUES({},{},{});";

	static const std::string FIELD_LOG_COLOR = "LogColor";
	static const std::string FIELD_LOG_TEXT = "LogText";

	static const auto AutoCreateAvatarLogTable = data::game::Common::Run(CREATE_TABLE);

	void Log::Clear(int avatarId)
	{
		AutoCreateAvatarLogTable();
		Common::Execute(
			DELETE_ALL, 
			avatarId);
	}

	void Log::Write(int avatarId,const std::string& color, const std::string& text)
	{
		AutoCreateAvatarLogTable();
		Common::Execute(
			INSERT_ITEM, 
			avatarId, 
			common::Data::QuoteString(color), 
			common::Data::QuoteString(text));
	}

	std::list<std::tuple<std::string, std::string>> Log::Read(int avatarId,size_t entryCount)
	{
		AutoCreateAvatarLogTable();
		std::list<std::tuple<std::string, std::string>> results;
		auto records = Common::Execute(QUERY_ALL, avatarId, entryCount);
		for (auto record : records)
		{
			results.push_back(
				std::make_tuple(
					record[FIELD_LOG_COLOR],
					record[FIELD_LOG_TEXT]));
		}
		return results;
	}
	void Log::ClearAll()
	{
		AutoCreateAvatarLogTable();
		Common::Execute(DELETE_ALL_LOGS);
	}
}
