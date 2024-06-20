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

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <imgui/imgui-SFML.h>
#include <imgui/imgui.h>

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
}

Editor::~Editor()
{
	s_pEditor = nullptr;
}

void Editor::Run()
{
	m_pEngine = std::make_unique<Engine>();
	m_pEngine->Init();
	CreatePanels();

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "LogiCraft");
	ImGui::CreateContext();
	ImGui::SFML::Init(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}

		m_pEngine->Update();
		for (auto& pPanel : m_panels)
		{
			pPanel->Update();
		}

		window.clear();
		sf::Time currentTime = sf::milliseconds(16);
		ImGui::SFML::Update(window, currentTime);

		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		for (auto& pPanel : m_panels)
		{
			pPanel->Draw();
		}

		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
}

void Editor::CreatePanels()
{
	for (PanelRegisterer* pRegisterer : PanelRegisterer::s_registerers)
	{
		m_panels.push_back(pRegisterer->Create());
		m_panels.back()->StartLoading();
	}
}
