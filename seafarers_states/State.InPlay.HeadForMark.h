#pragma once
#include <optional>
namespace state::in_play
{
	struct HeadForMark
	{
		HeadForMark() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
