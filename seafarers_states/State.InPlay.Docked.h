#pragma once
#include <optional>
namespace state::in_play
{
	struct Docked
	{
		Docked() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}