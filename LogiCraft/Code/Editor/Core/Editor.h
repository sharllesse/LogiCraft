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
#include "Core/Panel.h"
#include "Objects/EditorObjectManager.h"
#include "ResourceSystem/EditorResourceManager.h"
#include "Widgets/MainMenu.h"

#include <Engine/Core/Engine.h>
#include <Engine/Core/EventSystem.h>
#include <Engine/Core/Profiler.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>

namespace Logicraft
{
class Editor
{
public:
	enum EEvent
	{
		ePanelVisible = Engine::EEvent::eEventCount,
		eEventCount
	};
	static Editor& Get();

	Editor();
	~Editor();
	void Run();
	void ProcessWindowEvents();
	void ProcessEventSystem();
	void Update();
	void Render();
	void InitImGui();
	void CreatePanels();

	void      RestartClock() { m_timer = m_clock.restart(); }
	sf::Int32 GetDeltaTime() const { return m_timer.asMilliseconds(); }

	EventSystem& GetEventSystem() { return *m_pEventSystem; }

private:
	sf::RenderWindow m_window;

	std::unique_ptr<EditorObjectManager>   m_pEditorObjectManager;
	std::unique_ptr<EditorResourceManager> m_pEditorResourceManager;
	std::unique_ptr<Engine>                m_pEngine;
	std::unique_ptr<EventSystem>           m_pEventSystem;
	std::unique_ptr<MainMenu>              m_pMainMenu;

	std::vector<PanelPtr> m_panels;

	sf::Time  m_timer;
	sf::Clock m_clock;
};
} // namespace Logicraft