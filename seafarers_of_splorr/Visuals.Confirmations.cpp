#include "Visuals.Confirmations.h"
#include <stack>
namespace visuals::Confirmations
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

	void Reset()
	{
		while (HasConfirmation())
		{
			Read();
		}
	}
}
