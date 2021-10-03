#include <Common.Data.h>
#include "Data.Game.Avatar.Dock.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <format>
#include <string>
namespace data::game::avatar
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDocks]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [X], [Y] FROM [AvatarDocks] WHERE [AvatarId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDocks]([AvatarId],[X],[Y]) VALUES({},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarDocks] WHERE [AvatarId] = {};";

	static const auto AutoCreateAvatarDockTable = data::game::Common::Run(CREATE_TABLE);

	void Dock::Clear(int avatarId)
	{
		AutoCreateAvatarDockTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, avatarId));
	}


	void Dock::Write(int avatarId, const common::XY<double>& location)
	{
		AutoCreateAvatarDockTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, avatarId, location.GetX(), location.GetY()));
	}

	std::optional<common::XY<double>> Dock::Read(int avatarId)
	{
		AutoCreateAvatarDockTable();
		auto result = data::game::Common::Execute(std::format(QUERY_ITEM, avatarId));
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<common::XY<double>>(
				{
					common::Data::ToDouble(record[FIELD_X]),
					common::Data::ToDouble(record[FIELD_Y])
				});
		}
		return std::nullopt;
	}

	void Dock::Write(const common::XY<double>& location)
	{
		Write(Player::GetAvatarId(), location);
	}

	void Dock::Clear()
	{
		Clear(Player::GetAvatarId());
	}

	std::optional<common::XY<double>> Dock::Read()
	{
		return Read(Player::GetAvatarId());
	}
}