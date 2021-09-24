#include <Common.Data.h>
#include "Data.Game.Avatar.Equipment.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarEquipment]([AvatarId] INT NOT NULL,[EquipSlotId] INT NOT NULL,[ItemId] INT NOT NULL,UNIQUE([AvatarId],[EquipSlotId]));";
	static const std::string QUERY_ITEM = "SELECT [ItemId] FROM [AvatarEquipment] WHERE [AvatarId]={} AND [EquipSlotId]={};";
	static const std::string QUERY_ALL = "SELECT [EquipSlotId],[ItemId] FROM [AvatarEquipment] WHERE [AvatarId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarEquipment] WHERE [AvatarId]={} AND [EquipSlotId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarEquipment] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarEquipment] VALUES([AvatarId],[EquipSlotId],[ItemId]) VALUES({},{},{});";
	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string FIELD_EQUIP_SLOT_ID = "EquipSlotId";

	static auto AutoCreateAvatarEquipmentTable = Common::Run(CREATE_TABLE);

	std::optional<int> Equipment::Read(int equipSlotId)
	{
		AutoCreateAvatarEquipmentTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, equipSlotId));
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_ID]);
		}
		return std::nullopt;
	}

	void Equipment::Write(int equipSlotId, const std::optional<int>& itemId)
	{
		AutoCreateAvatarEquipmentTable();
		if (itemId)
		{
			Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, equipSlotId, itemId.value()));
			return;
		}
		Common::Execute(std::format(DELETE_ITEM, Common::AVATAR_ID, equipSlotId));
	}

	void Equipment::Clear()
	{
		AutoCreateAvatarEquipmentTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}

	std::map<int, int> Equipment::All()
	{
		std::map<int, int> result;
		AutoCreateAvatarEquipmentTable();
		auto records = Common::Execute(std::format(QUERY_ALL, Common::AVATAR_ID));
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_EQUIP_SLOT_ID])] = common::Data::ToInt(record[FIELD_ITEM_ID]);
		}
		return result;
	}
}