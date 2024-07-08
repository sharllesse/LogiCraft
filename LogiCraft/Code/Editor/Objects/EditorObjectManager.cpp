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

#include "EditorObjectManager.h"

#include <Engine/Core/ActionManager.h>
#include <Engine/Core/Engine.h>
#include <Engine/Objects/GameObjectManager.h>
#include <assert.h>

using namespace Logicraft;

struct ObjectGUIDCompare
{
	ObjectGUIDCompare(REFGUID guid)
	  : m_guid(guid)
	{
	}

	bool operator()(const EditorObjectPtr& object) const
	{
		if (GameObjectPtr pGameObject = object->GetGameObject())
			return pGameObject->GetGUID() == m_guid;

		return false;
	}

	REFGUID m_guid;
};

EditorObjectManager* s_pEditorObjectManager = nullptr;

EditorObjectManager& EditorObjectManager::Get()
{
	assert(s_pEditorObjectManager);
	return *s_pEditorObjectManager;
}

EditorObjectManager::EditorObjectManager()
{
	assert(!s_pEditorObjectManager);
	s_pEditorObjectManager = this;
}

EditorObjectManager::~EditorObjectManager()
{
	s_pEditorObjectManager = nullptr;
}

void EditorObjectManager::Init()
{
	m_pActionCreateObject = ActionManager::Get().AddAction("create_object");
	m_pActionCreateObject->SetCallback([this]() { CreateObject(); });
}

void EditorObjectManager::CreateObject()
{
	EditorObjectPtr pNewObject = make_shared(EditorObject);
	std::string     name       = "Object_" + std::to_string(m_objects.size());
	pNewObject->SetName(name);
	m_objects.push_back(pNewObject);

	GameObjectPtr pGameObject = GameObjectManager::Get().CreateObject();
	pNewObject->SetGameObject(pGameObject);
}

void EditorObjectManager::RemoveObject(REFGUID objectGUID)
{
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), ObjectGUIDCompare(objectGUID)); it != m_objects.end())
	{
		m_objects.erase(it);
	}
}

EditorObjectPtr EditorObjectManager::GetObject(REFGUID objectGUID)
{
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), ObjectGUIDCompare(objectGUID)); it != m_objects.end())
	{
		return *it;
	}
	return EditorObjectPtr();
}

void Logicraft::EditorObjectManager::CreateComponent(EditorObjectPtr pObject, const char* editorComponentType)
{
	for (auto& pComponentType : EditorComponent::GetRegisteredTypes())
	{
		if (pComponentType->GetName().compare(editorComponentType) == 0)
		{
			if (GameComponentPtr pGameComponent = GameObjectManager::Get().CreateComponent(pComponentType->GetName().c_str()))
			{
				EditorComponentPtr pEditorComponent = pComponentType->Create();
				pEditorComponent->SetGameComponent(pGameComponent);
				pObject->AddComponent(pEditorComponent);

				std::string message = "Component type " + std::string(editorComponentType) + " created.";
				Logger::Get().Log(Logger::eInfo, message);
				return;
			}
		}
	}
	std::string message = "Component type " + std::string(editorComponentType) + " does not exist!";
	Logger::Get().Log(Logger::eError, message);
}
