#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Data.JSON.Store.h"
#include "Data.JSON.Stores.h"
#include <map>
#include <optional>
#include <string>
#include "Visuals.Texts.h"
namespace data::json::Stores
{
	static std::map<int, nlohmann::json> stores;
	static bool modded = false;

	nlohmann::json& GetStore(int store)
	{
		return stores[store];
	}

	struct storeFile
	{
		std::string filename;
		std::optional<unsigned char> checkSum;
	};

	static std::map<int, storeFile> storeFiles;

	void SetStoreFile(int store, const std::string& filename, std::optional<unsigned char> checksum)
	{
		storeFiles[store] = { filename, checksum };
	}

	static unsigned char GetFileCheckSum(const std::string& filename)
	{
		unsigned char result = 0;
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		unsigned char data;
		if (file)
		{
			while (!feof(file))
			{
				fread(&data, 1, 1, file);
				result += data;
			}
			fclose(file);
		}
		return result;
	}

	void Start()
	{
		for (auto& entry : storeFiles)
		{
			auto checkSum = GetFileCheckSum(entry.second.filename);
			if (entry.second.checkSum.has_value() && checkSum != entry.second.checkSum.value())
			{
				modded = true;
#ifdef _DEBUG
				throw entry.second.filename;
#endif
			}
			stores[entry.first] = ::data::JSON::Load(entry.second.filename);
		}
	}

	void Save(int store)
	{
		auto iter = storeFiles.find(store);
		if (iter != storeFiles.end())
		{
			data::JSON::Save(iter->second.filename, GetStore(store));
		}
	}

	bool IsModded()
	{
		return modded;
	}
}
