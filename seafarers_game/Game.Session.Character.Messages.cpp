#include <Common.Utility.Table.h>
#include <Data.Game.Player.h>
#include "Game.Session.Character.Messages.h"
#include <map>
namespace game::session::character
{
	typedef std::map<int, game::characters::MessageList> MessageListTable;

	static MessageListTable messageListTable;

	void Messages::AddInternal(const std::string_view& color, const std::string& text) const
	{
		if (characterId == data::game::Player::GetCharacterId().value())
		{
			messageListTable[characterId].push_back({ color, text });
		}
	}

	characters::MessageList Messages::GetAll() const
	{
		return 
			common::utility::Table::TryGetKey(messageListTable, characterId)
			.value_or(characters::MessageList());
	}

	void Messages::Clear() const
	{
		messageListTable.erase(characterId);
	}

}