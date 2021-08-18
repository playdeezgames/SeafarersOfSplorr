#include "Visuals.Messages.h"
#include <queue>
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
}