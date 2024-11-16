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

#include "GameObjectManager.h"
#include "Engine/Core/Action.h"
#include "Engine/Core/Engine.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/SmartPtr.h"

#include <assert.h>
#include <string>

using namespace Logicraft;

GameObjectManager* s_pGameObjectManager = nullptr;

GameObjectManager& GameObjectManager::Get()
{
	assert(s_pGameObjectManager);
	return *s_pGameObjectManager;
}

GameObjectManager::GameObjectManager()
{
	assert(!s_pGameObjectManager);
	s_pGameObjectManager = this;
}

GameObjectManager::~GameObjectManager()
{
	s_pGameObjectManager = nullptr;
}

void Logicraft::GameObjectManager::Init()
{
	ActionPtr pAction = ActionManager::Get().AddAction("GameObjectManager_enable_log");
	pAction->SetCallback([this] { m_infoLogEnabled = true; });

	pAction = ActionManager::Get().AddAction("GameObjectManager_disable_log");
	pAction->SetCallback([this] { m_infoLogEnabled = false; });

	pAction = ActionManager::Get().AddAction("create_object");
	pAction->SetCallback([this] { CreateObject(); });
}

GameObjectPtr GameObjectManager::CreateObject()
{
	GameObjectPtr pNewObject = make_shared(GameObject);
	{
		std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
		m_objects.push_back(pNewObject);
	}
	if (m_infoLogEnabled)
	{
		std::string message = "Object created. " + std::to_string(m_objects.size()) + " in total.";
		Logger::Get().Log(Logger::eInfo, message);
	}
	return pNewObject;
}

void GameObjectManager::RemoveObject(REFGUID objectGUID)
{
	std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGUID)); it != m_objects.end())
	{
		GameObjectPtr                 pObject       = (*it);
		std::vector<GameComponentPtr> tmpComponents = pObject->GetComponents();
		for (GameComponentPtr pComponent : tmpComponents)
		{
			GameObjectManager::Get().RemoveComponent(pComponent->GetGUID());
		}
		pObject->Release();
		m_objects.erase(it);
		if (m_infoLogEnabled)
		{
			std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " removed.";
			Logger::Get().Log(Logger::eInfo, message);
		}
	}
	else
	{
		std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " asked to be removed but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
}

GameObjectPtr GameObjectManager::GetObject(REFGUID objectGUID) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGUID)); it != m_objects.end())
	{
		return *it;
	}
	else
	{
		std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " asked but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
	return GameObjectPtr();
}

GameComponentPtr GameObjectManager::CreateComponent(const char* componentType)
{
	for (auto& pComponentType : GameComponent::GetRegisteredTypes())
	{
		if (pComponentType->GetName().compare(componentType) == 0)
		{
			GameComponentPtr pComponent = pComponentType->Create();

			{
				std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
				m_components.push_back(pComponent);
			}
			if (m_infoLogEnabled)
			{
				std::string message = "Component type " + std::string(componentType) + " created. " + std::to_string(m_components.size()) + " in total.";
				Logger::Get().Log(Logger::eInfo, message);
			}
			return pComponent;
		}
	}

	std::string message = "Component type " + std::string(componentType) + " does not exist!";
	Logger::Get().Log(Logger::eWarning, message);
	return nullptr;
}

void Logicraft::GameObjectManager::RemoveComponent(REFGUID componentGUID)
{
	std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
	if (auto it = std::find_if(m_components.begin(), m_components.end(), GameComponent::GUIDCompare(componentGUID)); it != m_components.end())
	{
		GameComponentPtr pComponent = (*it);
		if (GameObject* pObject = pComponent->GetObject())
		{
			pObject->RemoveComponent(pComponent);
		}
		pComponent->Release();
		m_components.erase(it);

		if (m_infoLogEnabled)
		{
			std::string message = "Component " + GuidUtils::GuidToString(componentGUID) + " removed.";
			Logger::Get().Log(Logger::eInfo, message);
		}
	}
	else
	{
		std::string message = "Component " + GuidUtils::GuidToString(componentGUID) + " asked to be removed but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
}

GameComponentPtr Logicraft::GameObjectManager::GetComponent(REFGUID componentGUID) const
{
	std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
	if (auto it = std::find_if(m_components.begin(), m_components.end(), GameComponent::GUIDCompare(componentGUID)); it != m_components.end())
	{
		return *it;
	}
	else
	{
		std::string message = "Component " + GuidUtils::GuidToString(componentGUID) + " asked but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
	return GameComponentPtr();
}

void Logicraft::GameObjectManager::ForEachObject(const std::function<void(GameObjectPtr)>& func) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);
	for (GameObjectPtr pObject : m_objects)
	{
		func(pObject);
	}
}

void Logicraft::GameObjectManager::ForEachComponent(const std::function<void(GameComponentPtr)>& func) const
{
	std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
	for (GameComponentPtr pComponent : m_components)
	{
		func(pComponent);
	}
}
