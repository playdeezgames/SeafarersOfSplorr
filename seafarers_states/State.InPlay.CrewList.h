#pragma once
namespace state::in_play
{
	struct CrewList
	{
		CrewList() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}