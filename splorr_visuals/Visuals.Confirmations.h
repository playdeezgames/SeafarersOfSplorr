#pragma once
#include "Visuals.Confirmation.h"
namespace visuals::Confirmations
{
	bool HasConfirmation();
	Confirmation Read();
	void Write(const Confirmation&);
	void Reset();
}
