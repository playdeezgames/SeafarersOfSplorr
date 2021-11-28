#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishGame.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishGames]([CharacterId] INT NOT NULL UNIQUE,[Guesses] INT NOT NULL,[GivenUp] INT NOT NULL,[FisheryId] INT NULL);";
	static const std::string DELETE_ITEM = "DELETE FROM [FishGames] WHERE [CharacterId]={};";
	static const std::string QUERY_ITEM = "SELECT [Guesses],[GivenUp],[FisheryId] FROM [FishGames] WHERE [CharacterId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishGames]([CharacterId],[Guesses],[GivenUp],[FisheryId]) VALUES({},{},{},{});";
	static const std::string UPDATE_GUESSES = "UPDATE [FishGames] SET [Guesses]={} WHERE [CharacterId]={};";
	static const std::string UPDATE_GIVEN_UP = "UPDATE [FishGames] SET [GivenUp]={} WHERE [CharacterId]={};";
	static const std::string UPDATE_FISHERY_ID = "UPDATE [FishGames] SET [FisheryId]={} WHERE [CharacterId]={};";

	static const std::string FIELD_GUESSES = "Guesses";
	static const std::string FIELD_GIVEN_UP = "GivenUp";
	static const std::string FIELD_FISHERY_ID = "FisheryId";

	static const auto AutoCreateFishGameTable = Common::Run(CREATE_TABLE);

	int FishGame::ReadGuesses(int characterId)
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_GUESSES]);
		}
		return 0;
	}

	void FishGame::WriteGuesses(int characterId, int guesses)
	{
		AutoCreateFishGameTable();
		Common::Execute(UPDATE_GUESSES, guesses, characterId);
	}

	void FishGame::Clear(int characterId)
	{
		AutoCreateFishGameTable();
		Common::Execute(DELETE_ITEM, characterId);
	}

	void FishGame::Start(int characterId, int guesses)
	{
		AutoCreateFishGameTable();
		Common::Execute(REPLACE_ITEM, characterId, guesses, 0, "NULL");
	}

	bool FishGame::ReadGivenUp(int characterId)
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_GIVEN_UP])!=0;
		}
		return false;
	}

	void FishGame::WriteGivenUp(int characterId, bool flag)
	{
		AutoCreateFishGameTable();
		Common::Execute(UPDATE_GIVEN_UP, (flag) ? (1) : (0), characterId);
	}

	void FishGame::WriteFisheryId(int characterId, std::optional<int> fisheryId)
	{
		AutoCreateFishGameTable();
		Common::Execute(UPDATE_FISHERY_ID, common::Data::OfOptional(fisheryId), characterId);
	}

	std::optional<int> FishGame::ReadFisheryId(int characterId)
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_FISHERY_ID]);
		}
		return std::nullopt;
	}
}