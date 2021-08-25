#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Merchant.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Merchants]([MerchantId] INTEGER PRIMARY KEY AUTOINCREMENT,[X] REAL NOT NULL,[Y] REAL NOT NULL,[DestinationX] REAL NOT NULL,[DestinationY] REAL NOT NULL);";
	static const std::string INSERT_ITEM = "INSERT INTO [Merchants]([X],[Y],[DestinationX],[DestinationY]) VALUES({:.4f},{:.4f},{:.4f},{:.4f});";
	static const std::string UPDATE_ITEM = "UPDATE [Merchants] SET [X]={}, [Y]={}, [DestinationX]={}, [DestinationY]={} WHERE [MerchantId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [Merchants] WHERE [MerchantId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [Merchants];";
	static const std::string QUERY_ALL = "SELECT [MerchantId],[X],[Y],[DestinationX],[DestinationY] FROM [Merchants];";
	static const std::string FIELD_MERCHANT_ID = "MerchantId";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_DESTINATION_X = "DestinationX";
	static const std::string FIELD_DESTINATION_Y = "DestinationY";

	static const auto AutoCreateMerchantsTable = Common::Run(CREATE_TABLE);

	void Merchant::Add(const Merchant& merchant)
	{
		AutoCreateMerchantsTable();
		Common::Execute(std::format(INSERT_ITEM, 
			merchant.location.GetX(), 
			merchant.location.GetY(),
			merchant.destination.GetX(),
			merchant.destination.GetY()));
	}

	static Merchant ToMerchant(const std::map<std::string, std::string> record)
	{
		return {
			common::Data::ToInt(record.find(FIELD_MERCHANT_ID)->second),
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			{
				common::Data::ToDouble(record.find(FIELD_DESTINATION_X)->second),
				common::Data::ToDouble(record.find(FIELD_DESTINATION_Y)->second)
			}
		};
	}

	std::list<Merchant> Merchant::All()
	{
		AutoCreateMerchantsTable();
		std::list<Merchant> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(ToMerchant(record));
		}
		return result;
	}

	void Merchant::Update(const Merchant& merchant)
	{
		AutoCreateMerchantsTable();
		Common::Execute(
			std::format(
				UPDATE_ITEM,
				merchant.location.GetX(),
				merchant.location.GetY(),
				merchant.destination.GetX(),
				merchant.destination.GetY(),
				merchant.merchantId));
	}

	void Merchant::Remove(int merchantId)
	{
		AutoCreateMerchantsTable();
		Common::Execute(
			std::format(
				DELETE_ITEM,
				merchantId));
	}

	void Merchant::Clear()
	{
		AutoCreateMerchantsTable();
		Common::Execute(DELETE_ALL);
	}

}