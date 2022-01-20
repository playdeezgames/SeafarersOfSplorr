#pragma once
#include <optional>
namespace state::in_play::crew_detail
{
	struct Characteristics
	{
		Characteristics() = delete;
		static void Start();
	private:
		static std::optional<int> stateId;
	};
}
