#pragma once
#include <optional>
namespace state::in_play::crew_detail
{
	struct InventoryDetail
	{
		InventoryDetail() = delete;
		static void Start();
		static int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
