#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include <optional>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Characters]([CharacterId] INTEGER PRIMARY KEY AUTOINCREMENT,[State] INT NOT NULL, [Name] TEXT NOT NULL);";
	static const std::string QUERY_ITEM= "SELECT [State],[Name] FROM [Characters] WHERE [CharacterId] = {};";
	static const std::string CREATE_ITEM = "INSERT INTO [Characters]([State],[Name]) VALUES({}, {});";
	static const std::string UPDATE_ITEM = "UPDATE [Characters] SET [State]={}, [Name]={} WHERE CharacterId={};";
	static const std::string QUERY_ALL = "SELECT [CharacterId] FROM [Characters];";

	static const std::string FIELD_STATE = "State";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	static Character ToCharacter(const std::map<std::string, std::string> record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_STATE)->second),
			record.find(FIELD_NAME)->second
		};
	}

	std::optional<Character> Character::Read(int characterId)
	{
		AutoCreateTable();
		auto records = Common::Execute(
				QUERY_ITEM,
				characterId);
		if (!records.empty())
		{
			return ToCharacter(records.front());
		}
		return std::nullopt;
	}

	int Character::Create(const Character& character)
	{
		AutoCreateTable();
		Common::Execute(
			CREATE_ITEM,
			character.state,
			common::Data::QuoteString(character.name));
		return Common::LastInsertedIndex();
	}


	void Character::Write(int characterId, const Character& avatar)
	{
		AutoCreateTable();
		Common::Execute(
			UPDATE_ITEM,
			avatar.state,
			common::Data::QuoteString(avatar.name),
			characterId);
	}

	std::list<int> Character::All()
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ALL);
		std::list<int> result;
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_CHARACTER_ID]));
		}
		return result;
	}

}