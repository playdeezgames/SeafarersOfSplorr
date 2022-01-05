#pragma once
namespace data::game
{
	struct SkillCharacteristicCoefficient
	{
		SkillCharacteristicCoefficient() = delete;
		static void Initialize();
		static void Write(int skillId, int characteristic, int coefficient);
	};
}
