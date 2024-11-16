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

#include "GameObject.h"
#include "GameObjectManager.h"

#include <assert.h>
#include <combaseapi.h>

using namespace Logicraft;

Logicraft::GameObject::GameObject()
{
	HRESULT res = CoCreateGuid(&m_GUID);
	assert(res == S_OK);
}

void Logicraft::GameObject::Release()
{
	std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
	m_components.clear();
}

void GameObject::Update()
{
	std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
	for (GameComponentPtr pComponent : m_components)
	{
		pComponent->Update();
	}
}

void Logicraft::GameObject::Render(sf::RenderWindow& target)
{
	std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
	for (GameComponentPtr pComponent : m_components)
	{
		pComponent->Render(target);
	}
}

void GameObject::Serialize(bool load, JsonObjectPtr pJsonObject)
{
	std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
	for (GameComponentPtr pComponent : m_components)
	{
		// TODO
	}
}

void Logicraft::GameObject::AddComponent(GameComponentPtr pComponent)
{
	std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
	m_components.push_back(pComponent);
	pComponent->m_pGameObject = this;
}

void Logicraft::GameObject::RemoveComponent(GameComponentPtr pComponent)
{
	std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
	if (auto it = std::find_if(m_components.begin(), m_components.end(), GameComponent::GUIDCompare(pComponent->GetGUID())); it != m_components.end())
	{
		(*it)->m_pGameObject = nullptr;
		m_components.erase(it);
	}
}

void GameObject::Load() {}
