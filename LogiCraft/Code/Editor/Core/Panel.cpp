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

#include "Panel.h"

#include "Editor.h"

#include <imgui/imgui.h>

using namespace Logicraft;

void Logicraft::Panel::BaseDraw()
{
	if (m_visible)
	{
		if (ImGui::Begin(GetType().GetName().c_str(), &m_visible, ImGuiWindowFlags_MenuBar))
		{
			ImGui::SetWindowSize(ImVec2(1920.f, 500.f), ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(0.f, 580.f), ImGuiCond_FirstUseEver);
			m_menuBar.Draw();
			Draw();
		}
		if (!m_visible)
		{
			SetVisible(false);
		}
		ImGui::End();
	}
}

void Logicraft::Panel::SetVisible(bool visible)
{
	m_visible = visible;
	Editor::Get().GetEventSystem().QueueEvent(Editor::ePanelVisible);
}

void Logicraft::Panel::Serialize(bool load, JsonObjectPtr pJsonObject) {}

void Logicraft::Panel::Load()
{
	// TODO load m_visible
	// Serialize(true, );
}

void Logicraft::Panel::Save()
{
	// Serialize(false, );
}
