#pragma once
#include <optional>
namespace state
{
	struct ChooseStartType
	{
		ChooseStartType() = delete;
		static void Start();
		static int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
