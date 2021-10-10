#include "Visuals.Message.h"
namespace visuals
{
	Message::Message(const std::string& caption)
		: caption(caption)
		, details()
	{

	}
	Message& Message::Add(
		const common::XY<int>& location,
		const std::string& text,
		const std::string& color,
		const visuals::HorizontalAlignment& alignment)
	{
		details.push_back({location, text, color, alignment});
	}

}