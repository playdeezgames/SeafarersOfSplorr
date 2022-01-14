#pragma once
namespace state::scratch_pad
{
	struct SelectedDelivery
	{
		SelectedDelivery() = delete;
		static void SetDeliveryId(int deliveryId) { selectedDeliveryId = deliveryId; }
		static int GetDeliveryId() { return selectedDeliveryId; }
	private:
		static int selectedDeliveryId;
	};
}
