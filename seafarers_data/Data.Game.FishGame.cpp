#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishGame.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishGames]([AvatarId] INT NOT NULL UNIQUE,[Guesses] INT NOT NULL,[GivenUp] INT NOT NULL,[FisheryId] INT NULL);";
	static const std::string DELETE_ITEM = "DELETE FROM [FishGames] WHERE [AvatarId]={};";
	static const std::string QUERY_ITEM = "SELECT [Guesses],[GivenUp],[FisheryId] FROM [FishGames] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishGames]([AvatarId],[Guesses],[GivenUp],[FisheryId]) VALUES({},{},{},{});";
	static const std::string UPDATE_GUESSES = "UPDATE [FishGames] SET [Guesses]={} WHERE [AvatarId]={};";
	static const std::string UPDATE_GIVEN_UP = "UPDATE [FishGames] SET [GivenUp]={} WHERE [AvatarId]={};";
	static const std::string UPDATE_FISHERY_ID = "UPDATE [FishGames] SET [FisheryId]={} WHERE [AvatarId]={};";
	static const std::string FIELD_GUESSES = "Guesses";
	static const std::string FIELD_GIVEN_UP = "GivenUp";
	static const std::string FIELD_FISHERY_ID = "FisheryId";

	static const auto AutoCreateFishGameTable = Common::Run(CREATE_TABLE);

	int FishGame::ReadGuesses()
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_GUESSES]);
		}
		return 0;
	}

	void FishGame::WriteGuesses(int guesses)
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(UPDATE_GUESSES, guesses, Common::AVATAR_ID));
	}

	void FishGame::Clear()
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(DELETE_ITEM, Common::AVATAR_ID));
	}

	void FishGame::Start(int guesses)
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, guesses, 0, "NULL"));
	}

	bool FishGame::ReadGivenUp()
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_GIVEN_UP])!=0;
		}
		return false;
	}

	void FishGame::WriteGivenUp(bool flag)
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(UPDATE_GIVEN_UP, (flag) ? (1) : (0), Common::AVATAR_ID));
	}

	void FishGame::WriteFisheryId(std::optional<int> fisheryId)
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(UPDATE_FISHERY_ID, common::Data::OfOptional(fisheryId), Common::AVATAR_ID));
	}

	std::optional<int> FishGame::ReadFisheryId()
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_FISHERY_ID]);
		}
		return std::nullopt;

	}

}