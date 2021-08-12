#include "Game.Messages.h"
#include <queue>
namespace game::Messages
{
	static std::queue<Message> messages;

	bool IsEmpty()
	{
		return messages.empty();
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
		while (!IsEmpty())
		{
			Read();
		}
	}
}