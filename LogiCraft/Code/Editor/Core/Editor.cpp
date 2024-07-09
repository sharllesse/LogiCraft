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

#include "Editor.h"

#include <Engine/Core/Action.h>
#include <Engine/Core/SmartPtr.h>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <assert.h>
#include <imgui-SFML/imgui-SFML.h>
#include <imgui/imgui.h>

using namespace Logicraft;

Editor* s_pEditor = nullptr;

Editor& Editor::Get()
{
	assert(s_pEditor);
	return *s_pEditor;
}

Editor::Editor()
{
	assert(!s_pEditor);
	s_pEditor = this;

	// alphabetical order, no dependencies
	m_pEditorObjectManager   = std::make_unique<EditorObjectManager>();
	m_pEditorResourceManager = std::make_unique<EditorResourceManager>();
	m_pEngine                = std::make_unique<Engine>();
	m_pEventSystem           = std::make_unique<EventSystem>();
	m_pMainMenu              = std::make_unique<MainMenu>();
}

Editor::~Editor()
{
	s_pEditor = nullptr;
}

void Editor::Run()
{
	// Always initialize Engine first
	m_pEngine->Init();

	// Put other initializations here

	m_pEditorObjectManager->Init();

	// Initialize panels late as nothing depends on them and they depend on the other systems
	CreatePanels();

	// Load actions shortcuts after other systems have been initialized and added their actions
	ActionManager::Get().StartLoading();

	m_window.create(sf::VideoMode::getDesktopMode(), "LogiCraft");

	InitImGui();

	while (m_window.isOpen())
	{
		RestartClock();
		ProcessWindowEvents();
		ProcessEventSystem();
		Update();
		Render();
	}

	ImGui::SFML::Shutdown();

	m_pEngine->Release();
}

void Editor::ProcessWindowEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_window.close();
			}
		}
	}
}

void Editor::ProcessEventSystem()
{
	m_pEngine->ProcessEvents();
	GetEventSystem().ProcessEvents();
}

void Editor::Update()
{
	// PROFILE_FUNCTION
	m_pEngine->Update();

	for (PanelPtr& pPanel : m_panels)
	{
		// PROFILE_SCOPE(pPanel->GetName().c_str());
		pPanel->Update();
	}
}

void Editor::Render()
{
	// PROFILE_FUNCTION
	// PROFILE_SCOPE("Window Render");
	ImGui::SFML::Update(m_window, m_timer);
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);

	m_pMainMenu->Draw();

	for (PanelPtr& pPanel : m_panels)
	{
		// PROFILE_SCOPE(pPanel->GetName().c_str());
		pPanel->BaseDraw();
	}

	m_window.clear();
	m_pEngine->Render(m_window);
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Editor::InitImGui()
{
	ImGui::CreateContext();
	ImGui::SFML::Init(m_window);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Editor::CreatePanels()
{
	MenuPtr pPanelsMenu = m_pMainMenu->AddMenu("Panels");
	for (auto pType : Panel::GetRegisteredTypes())
	{
		// Create panel and start loading it
		m_panels.push_back(pType->Create());
		PanelPtr pPanel = m_panels.back();
		pPanel->StartLoading();

		// Add panel to the menu with action to toggle its visibility
		MenuItemPtr pItem = make_shared(MenuItem, pPanel->GetType().GetName().c_str());
		pItem->SetCheckEnabled(true);
		pItem->SetChecked(pPanel->IsVisible());
		pPanelsMenu->AddChild(pItem);

		const std::string actionName = std::string("toggle_") + pPanel->GetType().GetName();
		ActionPtr         pAction    = ActionManager::Get().AddAction(actionName.c_str());
		pAction->SetCallback([pPanel] { pPanel->SetVisible(!pPanel->IsVisible()); });
		pItem->SetAction(pAction);

		GetEventSystem().AddQueuedEventCallback(this, ePanelVisible, [pItem, pPanel] { pItem->SetChecked(pPanel->IsVisible()); });
	}

	std::sort(m_panels.begin(), m_panels.end(), [](const PanelPtr& a, const PanelPtr& b) { return a->GetType().GetName() < b->GetType().GetName(); });
}
