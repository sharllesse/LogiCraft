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

#include "EditorResourceManager.h"

#include <Engine/Core/Logger.h>
#include <Engine/ResourceSystem/ResourceManager.h>
#include <assert.h>
#include <format>

using namespace Logicraft;

EditorResourceManager* s_pEditorResourceManager = nullptr;

EditorResourceManager& EditorResourceManager::Get()
{
	assert(s_pEditorResourceManager);
	return *s_pEditorResourceManager;
}

EditorResourceManager::EditorResourceManager()
{
	assert(!s_pEditorResourceManager);
	s_pEditorResourceManager = this;
}

EditorResourceManager::~EditorResourceManager()
{
	s_pEditorResourceManager = nullptr;
}

EditorResourcePtr EditorResourceManager::CreateResource(const char* resourceType)
{
	for (auto& pResourceType : EditorResource::GetRegisteredTypes())
	{
		if (pResourceType->GetName().compare(resourceType) == 0)
		{
			EditorResourcePtr pEditorResource = pResourceType->Create();
			std::string       name            = std::format("{}_{}", resourceType, m_resources.size());
			pEditorResource->SetName(name);
			m_resources.push_back(pEditorResource);

			ResourcePtr pGameResource = ResourceManager::Get().CreateResource(resourceType);
			pEditorResource->SetResource(pGameResource);
			return pEditorResource;
		}
	}
	std::string message = "Resource type " + std::string(resourceType) + " does not exist!";
	Logger::Get().Log(Logger::eError, message);
	return nullptr;
}

void EditorResourceManager::Serialize(bool load, JsonObjectPtr pJsonObject)

{
	if (load) {}
	else // Save
	{
	}
}

void EditorResourceManager::Load() {}
