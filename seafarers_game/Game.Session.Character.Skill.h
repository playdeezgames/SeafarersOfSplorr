#pragma once
namespace game::session::character
{
	struct Skill
	{
		constexpr explicit Skill(int characterId, int skillId) : characterId(characterId), skillId(skillId) {}
		void AllocatePoints(size_t points) const;
	private:
		int characterId;
		int skillId;
	};
}
