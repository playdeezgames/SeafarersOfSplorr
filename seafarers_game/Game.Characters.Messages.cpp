#include <Common.Utility.Table.h>
#include "Game.Colors.h"
#include "Game.Characters.Messages.h"
#include "Game.Player.h"
#include <map>
namespace game::characters
{
	typedef std::map<int, MessageList> MessageListTable;

	static MessageListTable messageListTable;

	void Messages::Add(int characterId, const std::string& color, const std::string& text)
	{
		if (characterId == Player::GetCharacterId())
		{
			messageListTable[characterId].push_back({color, text});
		}
	}

	void Messages::Add(int characterId, const std::string& text)
	{
		Add(characterId, game::Colors::GRAY, text);
	}

	void Messages::Clear()
	{
		messageListTable.clear();
	}

	void Messages::Clear(int characterId)
	{
		messageListTable.erase(characterId);
	}

	static const MessageList emptyList;

	MessageList Messages::Read(int characterId)
	{
		return common::utility::Table::TryGetKey(messageListTable, characterId).value_or(emptyList);
	}
}