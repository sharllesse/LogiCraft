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

#include "Engine.h"
#include "Objects/GameObject.h"
#include "Profiler.h"

#include <assert.h>

using namespace Logicraft;

Engine* s_pEngine = nullptr;

Engine& Engine::Get()
{
	assert(s_pEngine);
	return *s_pEngine;
}

Engine::Engine()
{
	assert(!s_pEngine);
	s_pEngine            = this;
	m_pActionManager     = std::make_unique<ActionManager>();
	m_pEventSystem       = std::make_unique<EventSystem>();
	m_pGameObjectManager = std::make_unique<GameObjectManager>();
	m_pLogger            = std::make_unique<Logger>();
	m_pResourceManager   = std::make_unique<ResourceManager>();
	m_pTaskManager       = std::make_unique<TaskManager>();
	m_pUnitTest          = std::make_unique<UnitTest>();
}

Engine::~Engine()
{
	s_pEngine = nullptr;
}

void Engine::Init()
{
	m_pGameObjectManager->Init();
	m_pResourceManager->StartLoading();

	ActionPtr pAction = ActionManager::Get().AddAction("EventSystem_enable_log");
	pAction->SetCallback([] { EventSystem::EnableLog(true); });

	pAction = ActionManager::Get().AddAction("EventSystem_disable_log");
	pAction->SetCallback([] { EventSystem::EnableLog(false); });

	pAction = ActionManager::Get().AddAction("run_tests");
	pAction->SetCallback([this] { m_pUnitTest->Run(); });
}

void Logicraft::Engine::ProcessEvents()
{
	m_pEventSystem->QueueEvent(eProcessedEvents);
	m_pEventSystem->ProcessEvents();
}

void Engine::Update()
{
	// PROFILE_FUNCTION
	m_pGameObjectManager->ForEachObject([](GameObjectPtr pObject) { pObject->Update(); });
}

void Engine::Render(sf::RenderWindow& target)
{
	m_pGameObjectManager->ForEachObject([&target](GameObjectPtr pObject) { pObject->Render(target); });
}

void Engine::Release()
{
	m_pResourceManager->StartSaving();
	m_pActionManager->StartSaving();
}
