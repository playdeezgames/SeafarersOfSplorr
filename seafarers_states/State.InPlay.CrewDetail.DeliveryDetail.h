#pragma once

namespace state::in_play::crew_detail
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
