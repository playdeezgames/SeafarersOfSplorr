#include "Application.UIState.h"
#include "Game.Messages.h"
#include <queue>
#include "UIState.h"
namespace game::Messages
{
	static std::queue<Message> messages;

	bool HasMessage()
	{
		return !messages.empty();
	}

	Message Read()
	{
		Message result = messages.front();
		messages.pop();
		return result;
	}

	void Write(const Message& message)
	{
		messages.push(message);
	}

	void Reset(const Difficulty&)
	{
		while (HasMessage())
		{
			Read();
		}
	}

	void Send(const Message& message)
	{
		Write(message);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

}