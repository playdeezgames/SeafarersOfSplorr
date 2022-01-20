#pragma once
#include <optional>
namespace state::in_play::crew_detail
{
	struct ConfirmAbandonDelivery
	{
		ConfirmAbandonDelivery() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
