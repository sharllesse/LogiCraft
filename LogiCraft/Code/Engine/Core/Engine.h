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
#include "ActionManager.h"
#include "DLLExport.h"
#include "EventSystem.h"
#include "Logger.h"
#include "Objects/GameObjectManager.h"
#include "ResourceSystem/ResourceManager.h"
#include "TaskManager.h"
#include "UnitTest/UnitTest.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace Logicraft
{
class LOGI_ENGINE_API Engine
{
public:
	enum EEvent
	{
		eObjectCreated = 0,
		eObjectLoaded,
		eProcessedEvents,
		eActionExecuteRequested,
		eEventCount
	};

	static Engine& Get();

	Engine();
	~Engine();

	void Init();
	void ProcessEvents();
	void Update();
	void Render(sf::RenderWindow& target);
	void Release();

	EventSystem& GetEventSystem() { return *m_pEventSystem; }

private:
	std::unique_ptr<ActionManager>     m_pActionManager;
	std::unique_ptr<EventSystem>       m_pEventSystem;
	std::unique_ptr<GameObjectManager> m_pGameObjectManager;
	std::unique_ptr<Logger>            m_pLogger;
	std::unique_ptr<ResourceManager>   m_pResourceManager;
	std::unique_ptr<TaskManager>       m_pTaskManager;
	std::unique_ptr<UnitTest>          m_pUnitTest;
};
} // namespace Logicraft