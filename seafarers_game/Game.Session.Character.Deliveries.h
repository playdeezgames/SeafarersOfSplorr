#pragma once
namespace game::session::character
{
	struct Deliveries
	{
		constexpr explicit Deliveries(int characterId) : characterId(characterId) {};
		bool HasAny() const;
	private:
		int characterId;
	};
}
