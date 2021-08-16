#include "Game.Confirmations.h"
#include <stack>
namespace game::Confirmations
{
	static std::stack<Confirmation> confirmations;

	bool HasConfirmation()
	{
		return !confirmations.empty();
	}

	Confirmation Read()
	{
		Confirmation result = confirmations.top();
		confirmations.pop();
		return result;
	}

	void Write(const Confirmation& confirmation)
	{
		confirmations.push(confirmation);
	}

	void Reset(const Difficulty&)
	{
		while (HasConfirmation())
		{
			Read();
		}
	}
}
