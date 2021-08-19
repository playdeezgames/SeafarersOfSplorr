#include <algorithm>
#include "Data.JSON.h"
#include "Data.JSON.Store.h"
#include "Data.JSON.Stores.h"
#include <functional>
#include "Visuals.DrawerFunction.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Data.Types.h"
#include "Visuals.Layouts.h"
#include "Visuals.Menus.h"
#include "Visuals.Areas.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
#include "Visuals.WorldMap.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Sublayout.h"
namespace visuals::Layouts
{
	struct InternalLayout
	{
		std::vector<DrawerFunction> drawers;
	};

	static std::map<std::string, nlohmann::json> layouts;
	static std::map<std::string, InternalLayout> internalLayouts;

	static std::map<std::string, InternalizerFunction> internalizers =
	{
		{"Image", visuals::Image::Internalize},
		{"Text", visuals::Text::Internalize},
		{"Menu", visuals::Menu::Internalize},
		{"Layout", visuals::Sublayout::Internalize},
		{"Area", visuals::Areas::Internalize},
		{"SpriteGrid", visuals::SpriteGrid::Internalize}
	};

	void RegisterType(const std::string& typeName, InternalizerFunction internalizer)
	{
		internalizers[typeName] = internalizer;
	}

	std::function<void()> DoRegisterType(const std::string& typeName, InternalizerFunction internalizer)
	{
		return [typeName, internalizer]() 
		{
			RegisterType(typeName, internalizer);
		};
	}


	static void InternalizeTypedDrawn(const std::string layoutName, const std::string& drawnType, const nlohmann::json& drawn)
	{
		internalLayouts[layoutName].drawers.push_back(internalizers.find(drawnType)->second(layoutName, drawn));
	}

	static void InternalizeTypedDrawnFromString(const std::string layoutName, const nlohmann::json& type, const nlohmann::json& drawn)
	{
		InternalizeTypedDrawn(layoutName, type, drawn);
	}

	static void InternalizeDrawn(const std::string layoutName, const nlohmann::json& drawn)
	{
		auto& types = drawn[visuals::data::Properties::TYPE];
		if (types.is_array())
		{
			for (auto& type : types)
			{
				InternalizeTypedDrawnFromString(layoutName, type, drawn);
			}
			return;
		}
		InternalizeTypedDrawnFromString(layoutName, types, drawn);
	}

	static void Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		internalLayouts[layoutName] = {};
		for (auto& drawn : model)
		{
			InternalizeDrawn(layoutName, drawn);
		}
	}

	static void InitializeLayout(const std::string& layoutName)
	{
		if (!layouts.contains(layoutName))
		{
			layouts[layoutName] = ::data::JSON::Load(::data::json::Stores::GetStore(::data::json::Store::LAYOUTS)[layoutName]);
			Internalize(layoutName, layouts[layoutName]);
		}
	}

	void Start()
	{
		auto& store = ::data::json::Stores::GetStore(::data::json ::Store::LAYOUTS);
		for (auto& entry : store.items())
		{
			InitializeLayout(entry.key());
		}
	}

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		return layouts.find(layoutName)->second;
	}

	void Draw(const std::shared_ptr<application::Engine::Renderer>& renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		auto& internalLayout = internalLayouts[layoutName];
		for (auto& drawer : internalLayout.drawers)
		{
			drawer(renderer);
		}
	}
}