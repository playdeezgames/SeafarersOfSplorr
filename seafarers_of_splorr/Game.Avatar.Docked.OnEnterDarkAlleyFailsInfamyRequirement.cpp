#pragma once
#include "Visuals.Data.Colors.h"
#include "Visuals.Messages.h"
namespace game::avatar::Docked
{
	void OnEnterDarkAlleyFailsInfamyRequirement()
	{
		visuals::Messages::Write(
			{
				"==FIGHT!==",
				{
					{
						{19,9},
						"You have been waylaid by a ruffian!",
						visuals::data::Colors::RED,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						"Prepare to fight!",
						visuals::data::Colors::RED,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
	}
}