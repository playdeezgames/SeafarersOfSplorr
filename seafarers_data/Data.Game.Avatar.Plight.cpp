#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.Plight.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Plights]([AvatarId] INT NOT NULL,[PlightId] INT NOT NULL, [Duration] INT NULL, UNIQUE([AvatarId],[PlightId]));";
	static const std::string DELETE_ALL = "DELETE FROM [Plights] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL_PLIGHTS = "DELETE FROM [Plights];";
	static const std::string DELETE_ITEM = "DELETE FROM [Plights] WHERE [PlightId]={} AND [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Plights]([AvatarId], [PlightId], [Duration]) VALUES({}, {}, {});";
	static const std::string QUERY_ITEM = "SELECT [Duration] FROM [Plights] WHERE [PlightId]={} AND [AvatarId]={};";
	static const std::string QUERY_ALL = "SELECT [PlightId], [Duration] FROM [Plights] WHERE [AvatarId]={};";
	static const std::string FIELD_PLIGHT_ID = "PlightId";
	static const std::string FIELD_DURATION = "Duration";

	static const auto AutoCreatePlightTable = Common::Run(CREATE_TABLE);

	std::optional<Plight> Plight::Read(int avatarId, int plightId)
	{
		AutoCreatePlightTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, plightId, avatarId));
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<Plight>({
				plightId,
				common::Data::ToOptionalInt(record[FIELD_DURATION])
			});
		}
		return std::nullopt;
	}

	void Plight::Write(int avatarId, const Plight& plight)
	{
		AutoCreatePlightTable();
		Common::Execute(std::format(REPLACE_ITEM, avatarId, plight.plightId, common::Data::OfOptional(plight.duration)));
	}

	void Plight::ClearPlight(int avatarId, int plightId)
	{
		AutoCreatePlightTable();
		Common::Execute(std::format(DELETE_ITEM, plightId, avatarId));

	}
	void Plight::Clear(int avatarId)
	{
		AutoCreatePlightTable();
		Common::Execute(std::format(DELETE_ALL,avatarId));
	}

	std::list<Plight> Plight::All(int avatarId)
	{
		AutoCreatePlightTable();
		std::list<Plight> result;
		auto records = Common::Execute(std::format(QUERY_ALL, avatarId));
		for (auto& record : records)
		{
			result.push_back({
				common::Data::ToInt(record[FIELD_PLIGHT_ID]),
				common::Data::ToOptionalInt(record[FIELD_DURATION])
				});
		}
		return result;
	}

	std::optional<Plight> Plight::Read(int plightId)
	{
		return Read(Common::AVATAR_ID, plightId);
	}

	void Plight::Write(const Plight& plight)
	{
		Write(Common::AVATAR_ID, plight);
	}

	void Plight::ClearPlight(int plightId)
	{
		ClearPlight(Common::AVATAR_ID, plightId);
	}

	void Plight::Clear()
	{
		Clear(Common::AVATAR_ID);
	}

	std::list<Plight> Plight::All()
	{
		return All(Common::AVATAR_ID);
	}

	void Plight::ClearAll()
	{
		AutoCreatePlightTable();
		Common::Execute(DELETE_ALL_PLIGHTS);
	}

}