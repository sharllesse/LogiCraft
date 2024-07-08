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

#include "ResourceManager.h"
#include "Core/Logger.h"

#include <assert.h>

using namespace Logicraft;

ResourceManager* s_pResourceManager = nullptr;

ResourceManager& ResourceManager::Get()
{
	assert(s_pResourceManager);
	return *s_pResourceManager;
}

ResourceManager::ResourceManager()
{
	assert(!s_pResourceManager);
	s_pResourceManager = this;
}

ResourceManager::~ResourceManager()
{
	s_pResourceManager = nullptr;
}

ResourcePtr ResourceManager::CreateResource(const char* resourceType)
{
	for (auto& pResourceType : ResourceRegisterer::s_registerers)
	{
		if (pResourceType->GetName().compare(resourceType) == 0)
		{
			ResourcePtr pResource = pResourceType->Create();
			m_loadedResources.push_back(pResource);
			return pResource;
		}
	}
	std::string message = "Resource type " + std::string(resourceType) + " does not exist!";
	Logger::Get().Log(Logger::eError, message);
	return nullptr;
}

void ResourceManager::Serialize(bool load, JsonObjectPtr pJsonObject)

{
	if (load)
	{
		for (GUID& resourceGUID : m_resourcesToLoad)
		{
			if (auto it = m_resourcesToFiles.find(resourceGUID); it != m_resourcesToFiles.end())
			{
				std::string file = it->second;
			}
			else
			{
				// Error
			}
		}
	}
	else // Save
	{
		for (ResourcePtr& pResource : m_loadedResources)
		{
			pResource->Serialize(load, pJsonObject);
		}
	}
}

void ResourceManager::Load() {}
