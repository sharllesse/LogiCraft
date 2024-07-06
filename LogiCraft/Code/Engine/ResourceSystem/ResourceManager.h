/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) 2024 CIRON Robin
Copyright (c) 2024 GRALLAN Yann
Copyright (c) 2024 LESAGE Charles
Copyright (c) 2024 MENA-BOUR Samy

This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:

- [SFML](https://github.com/SFML)
- [ImGUI](https://github.com/ocornut/imgui)
- [ImNodes](https://github.com/Nelarius/imnodes)
- [SFML-Manager](https://github.com/Xanhos/Cpp-Manager-for-SFML)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
---------------------------------------------------------------------------------*/

#pragma once
#include "Core/Serializable.h"
#include "Core/SmartPtr.h"
#include "DLLExport.h"
#include "Resource.h"
#include "Utils/GuidUtils.h"

#include <map>
#include <string>
#include <vector>

namespace Logicraft
{
class LOGI_ENGINE_API ResourceManager : public Serializable
{
	enum EFileFormat
	{
		eJson,
		eBinary
	};

public:
	static ResourceManager& Get();

	ResourceManager();
	~ResourceManager();

	void SetFileFormat(EFileFormat format) { m_fileFormat = format; }

	template<typename T>
	ResourcePtr CreateResource()
	{
		ResourcePtr pResource = make_shared(T);
		m_loadedResources.push_back(pResource);
		return pResource;
	}
	ResourcePtr CreateResource(const char* resourceType);

	template<typename T>
	std::shared_ptr<T> Find(REFGUID guid)
	{
		for (auto& pResource : m_loadedResources)
		{
			if (pResource->GetGUID() == guid)
			{
				return pResource;
			}
		}
		return nullptr;
	}

	const std::vector<ResourcePtr>& GetLoadedResources() const { return m_loadedResources; }

	void Serialize(bool load, JsonObjectPtr pJsonObject) override;

protected:
	void Load() override;

private:
	std::map<GUID, std::string, GuidUtils::GUIDComparer> m_resourcesToFiles;
	std::vector<ResourcePtr>                             m_loadedResources;
	std::vector<GUID>                                    m_resourcesToLoad;
	EFileFormat                                          m_fileFormat{eBinary};
};
} // namespace Logicraft