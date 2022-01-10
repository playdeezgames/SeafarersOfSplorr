#include "State.ScratchPad.SelectedSkill.h"
namespace state::scratch_pad
{
	int SelectedSkill::selectedSkillId = 0;

	void SelectedSkill::SetSkillId(int skillId)
	{
		selectedSkillId = skillId;
	}

	int SelectedSkill::GetSkillId()
	{
		return selectedSkillId;
	}
}