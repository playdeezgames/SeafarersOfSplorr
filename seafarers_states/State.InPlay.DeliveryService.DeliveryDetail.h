#pragma once
#include <optional>
namespace state::in_play::delivery_service
{
	struct DeliveryDetail
	{
		DeliveryDetail() = delete;
		static void Start();
		static constexpr int GetStateId() { return stateId.value(); }
	private:
		static std::optional<int> stateId;
	};
}
