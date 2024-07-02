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
#include "Engine/Core/Engine.h"
#include "Engine/Utils/SmartPtr.h"

#include <assert.h>

using namespace Logicraft;

struct ObjectGUIDCompare
{
	ObjectGUIDCompare(REFGUID guid)
	  : m_guid(guid)
	{
	}

	bool operator()(const GameObjectPtr& object) const { return object->GetGUID() == m_guid; }

	REFGUID m_guid;
};

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

GameObjectPtr GameObjectManager::CreateObject()
{
	GameObjectPtr pNewObject = make_shared(GameObject);
	m_objects.push_back(pNewObject);
	return pNewObject;
}

void GameObjectManager::RemoveObject(REFGUID objectGUID)
{
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), ObjectGUIDCompare(objectGUID)); it != m_objects.end())
	{
		m_objects.erase(it);
	}
}

GameObjectPtr GameObjectManager::GetObject(REFGUID objectGUID)
{
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), ObjectGUIDCompare(objectGUID)); it != m_objects.end())
	{
		return *it;
	}
	return GameObjectPtr();
}
