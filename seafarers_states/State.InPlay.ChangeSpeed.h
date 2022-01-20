#pragma once
#include <optional>
namespace state::in_play
{
	struct ChangeSpeed
	{
		ChangeSpeed() = delete;
		static void Start();
		static int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
