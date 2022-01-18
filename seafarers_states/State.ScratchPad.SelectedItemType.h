#pragma once
namespace state::scratch_pad
{
	struct SelectedItemType
	{
		SelectedItemType() = delete;
		static constexpr void SetItemTypeId(int itemTypeId) { selectedItemTypeId = itemTypeId; }
		static constexpr int GetItemTypeId() { return selectedItemTypeId; }
	private:
		static int selectedItemTypeId;
	};
}
