#include <algorithm>
#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Player.h"
#include "Game.ShipNames.h"
#include <iterator>
#include <numeric>
namespace game::session
{
	Character Player::GetCharacter() const
	{
		return Character(data::game::Player::GetCharacterId().value());
	}

	using ShipData = data::game::Ship;

	static void GenerateCharacterShip(int characterId)
	{
		auto worldSize = game::Session().GetWorld().GetBounds().GetSize();
		auto shipType = game::ShipType::RAFT;
		int shipId = ShipData::Create(
			(int)shipType,
			game::ShipNames::Generate(),
			{
				worldSize.GetX() / 2.0,
				worldSize.GetY() / 2.0
			},
			common::Data::ModuloDouble(
				common::RNG::FromRange(0.0, common::Heading::DEGREES),
				common::Heading::DEGREES).value(),
			1.0);
		data::game::character::Ship::Write(characterId, shipId, (int)BerthType::CAPTAIN);
	}

	void Player::Populate(const Difficulty& difficulty) const
	{
		auto character = game::session::Characters().Create(game::characters::State::AT_SEA);
		auto allocations = game::session::Player::GetProfessionSkillPointAllocations();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[character](const std::pair<int, size_t>& entry)
			{
				auto skill = character.GetSkills().GetSkill(entry.first);
				skill.AllocatePoints(entry.second);
			});
		data::game::Player::Create((int)character);
		GenerateCharacterShip((int)character);
	}

	void Player::Reset() const
	{
	}

	std::optional<Character> Player::TryGetCharacter() const
	{
		auto characterId = data::game::Player::GetCharacterId();
		if (characterId)
		{
			return Character(characterId.value());
		}
		return std::nullopt;
	}
	void Player::SetCharacterCreationDefaults()
	{
		auto skills = game::Session().GetWorld().GetSkills();
		auto skillId = skills.GetSkillsInCategory(game::SkillCategory::CLIMB).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::DODGE).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::NAVIGATE).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::SWIM).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::LISTEN).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::SPOT).begin()->operator int();
		AllocateProfessionalSkillPoints(skillId, 50);
	}

	static std::map<int, size_t> professionalSkillPointAllocations;

	void Player::ClearProfessionSkillPointAllocations()
	{
		professionalSkillPointAllocations.clear();
	}

	const std::map<int, size_t>& Player::GetProfessionSkillPointAllocations()
	{
		return professionalSkillPointAllocations;
	}

	size_t Player::GetProfessionalSkillPointsAllocated()
	{
		return std::accumulate(
			professionalSkillPointAllocations.begin(),
			professionalSkillPointAllocations.end(),
			(size_t)0,
			[](size_t accumulator, const std::pair<int, size_t> entry)
			{
				return accumulator + entry.second;
			});
	}

	size_t Player::GetProfessionalSkillPointsRemaining()
	{
		return PROFESSIONAL_SKILL_POINT_COUNT - GetProfessionalSkillPointsAllocated();
	}

	void Player::AllocateProfessionalSkillPoints(int skillId, size_t points)
	{
		professionalSkillPointAllocations[skillId] += points;
	}


	static std::set<Skill> professionalSkillSet;

	static void AddSkillCategoryToProfessionalSkillSet(const SkillCategory& category)
	{
		auto skills =
			game::Session()
			.GetWorld()
			.GetSkills()
			.GetSkillsInCategory(category.operator game::SkillCategory());
		std::copy(
			skills.begin(),
			skills.end(),
			std::inserter(professionalSkillSet, professionalSkillSet.end()));
	}

	static void AddSkillCategorySetToProfessionalSkillSet(const std::set<SkillCategory>& categorySet)
	{
		std::for_each(
			categorySet.begin(),
			categorySet.end(),
			AddSkillCategoryToProfessionalSkillSet);
	}


	void Player::GenerateProfessionalSkillList(const game::Profession& profession, const std::set<SkillCategory>& categorySet)
	{
		professionalSkillSet.clear();

		AddSkillCategorySetToProfessionalSkillSet(
			game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(profession)
			.GetSkillCategories());

		AddSkillCategorySetToProfessionalSkillSet(
			categorySet);
	}

	const std::set<Skill>& Player::GetProfessionalSkillSet()
	{
		return professionalSkillSet;
	}

	int selectedSkillId;

	void Player::SetSelectedSkillId(int skillId)
	{
		selectedSkillId = skillId;
	}

	int Player::GetSelectedSkillId()
	{
		return selectedSkillId;
	}

	static std::map<int, size_t> personalSkillPointAllocations;

	void Player::ClearPersonalSkillAllocations()
	{
		personalSkillPointAllocations.clear();
	}

	size_t Player::GetPersonalSkillPointsAllocated()
	{
		return std::accumulate(
			personalSkillPointAllocations.begin(),
			personalSkillPointAllocations.end(),
			(size_t)0,
			[](size_t accumulator, const std::pair<int, size_t> entry)
			{
				return accumulator + entry.second;
			});
	}

	size_t Player::GetPersonalSkillPointCount()
	{
		const size_t INT_MULTIPLIER = 10;
		size_t intelligence =
			(size_t)game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetCharacteristics()
			.GetCharacteristic(game::Characteristic::INTELLIGENCE)
			.GetValue();
		return intelligence * INT_MULTIPLIER;
	}

	size_t Player::GetPersonalSkillPointsRemaining()
	{
		return GetPersonalSkillPointCount() - GetPersonalSkillPointsAllocated();
	}

	const std::map<int, size_t>& Player::GetPersonalSkillPointAllocations()
	{
		return personalSkillPointAllocations;
	}

	void Player::AllocatePersonalSkillPoints(int skillId, size_t points)
	{
		personalSkillPointAllocations[skillId] += points;
	}

	void Player::DistributePersonalSkillPointAllocations()
	{
		std::for_each(
			personalSkillPointAllocations.begin(), 
			personalSkillPointAllocations.end(), 
			[](const std::pair<int, size_t>& entry) 
			{
				game::Session()
					.GetPlayer()
					.GetCharacter()
					.GetSkills()
					.GetSkill(entry.first)
					.AllocatePoints(entry.second);
			});
	}

}