#pragma once
#include <optional>
namespace state
{
	struct ConfirmAbandon
	{
		ConfirmAbandon() = delete;
		static void Start();
		static int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}