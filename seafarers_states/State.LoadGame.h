#pragma once
#include <optional>
namespace state
{
	struct LoadGame
	{
		LoadGame() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}