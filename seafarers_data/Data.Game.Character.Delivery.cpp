#include "Data.Game.Common.h"
#include "Data.Game.Character.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Character.Delivery.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterDeliveries]
		(
			[CharacterId] INT NOT NULL,
			[DeliveryId] INT NOT NULL UNIQUE
		);)"sv;

	void Delivery::Initialize()
	{
		Delivery::Initialize();
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}
}