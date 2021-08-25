#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Merchant.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Merchants]([MerchantId] INTEGER PRIMARY KEY AUTOINCREMENT,[X] REAL NOT NULL,[Y] REAL NOT NULL,[DestinationX] REAL NOT NULL,[DestinationY] REAL NOT NULL);";
	static const std::string INSERT_ITEM = "INSERT INTO [Merchants]([X],[Y],[DestinationX],[DestinationY]) VALUES({},{},{},{});";
	
	static const auto AutoCreateMerchantsTable = Common::Run(CREATE_TABLE);

	void Merchant::Add(const Merchant& merchant)
	{
		AutoCreateMerchantsTable();
		Common::Execute(std::format(INSERT_ITEM, 
			merchant.location.GetX(), 
			merchant.location.GetY(),
			merchant.destination.GetX(),
			merchant.destination.GetY()
			));
	}
}