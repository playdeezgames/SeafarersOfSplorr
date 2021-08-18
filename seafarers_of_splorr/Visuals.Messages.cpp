#include "Application.UIState.h"
#include "Visuals.Messages.h"
#include <queue>
#include "UIState.h"
namespace visuals::Messages
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

	void Reset()
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