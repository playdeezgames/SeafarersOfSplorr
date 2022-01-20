#pragma once
#include <optional>
namespace state
{
	struct ChoosePersonalSkill
	{
		ChoosePersonalSkill() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
