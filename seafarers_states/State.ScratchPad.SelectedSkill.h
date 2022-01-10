#pragma once
namespace state::scratch_pad
{
	struct SelectedSkill
	{
		SelectedSkill() = delete;
		static void SetSkillId(int skillId);
		static int GetSkillId();
	private:
		static int selectedSkillId;
	};
}
