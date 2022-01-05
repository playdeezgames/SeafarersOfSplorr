#pragma once
namespace data::game
{
	struct SkillCharacteristicCoefficient
	{
		static void Initialize();
		static void Write(int skillId, int characteristic, int coefficient);
	};
}
