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

#include "ContentBrowser.h"
#include "Widgets/Menu.h"
#include "Widgets/MenuItem.h"

#include <Engine/Core/Action.h>
#include <Engine/Core/ActionManager.h>
#include <Engine/Core/TaskManager.h>
#include <Engine/Objects/GameObject.h>
#include <Engine/ResourceSystem/ResourceManager.h>
#include <imgui/imgui.h>

using namespace Logicraft;

ContentBrowser::ContentBrowser(const char* name)
  : Panel(name)
{
	MenuPtr pMenuNew = std::make_shared<Menu>("New");
	m_menuBar.AddChild(pMenuNew);

	// TODO for all resource types
	MenuItemPtr pNewGameObject = std::make_shared<MenuItem>("GameObject");
	pMenuNew->AddChild(pNewGameObject);
	ActionPtr pAction = ActionManager::Get().AddAction("new_gameobject");
	pAction->SetCallback([] { ResourceManager::Get().CreateResource<GameObject>(); });
	pNewGameObject->SetAction(pAction);
}

void ContentBrowser::Draw()
{
	if (m_visible)
	{
		if (ImGui::Begin(m_name.c_str(), &m_visible, ImGuiWindowFlags_MenuBar))
		{
			ImGui::SetWindowSize(ImVec2(1920.f, 500.f), ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(0.f, 580.f), ImGuiCond_FirstUseEver);

			m_menuBar.Draw();
		}
		ImGui::End();
	}
}