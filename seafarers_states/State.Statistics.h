#pragma once
#include <optional>
namespace state
{
	struct Statistics
	{
		Statistics() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}