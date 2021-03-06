#include "Game.Session.Commodities.h"
namespace game::session
{
	static const std::vector<Commodity> allCommodities =
	{
		Commodity(game::Commodity::WHEAT),
		Commodity(game::Commodity::WOOD),
		Commodity(game::Commodity::MEAT),
		Commodity(game::Commodity::WOOL),
		Commodity(game::Commodity::JOOLS),
		Commodity(game::Commodity::LABOR),
		Commodity(game::Commodity::SILK),
		Commodity(game::Commodity::SEAWEED),
		Commodity(game::Commodity::SALT),
		Commodity(game::Commodity::FOOD)
	};

	const std::vector<Commodity>& Commodities::GetAll() const
	{
		return allCommodities;
	}
}