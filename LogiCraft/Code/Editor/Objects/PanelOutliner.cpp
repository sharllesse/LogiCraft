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
#include "PanelOutliner.h"

#include "EditorComponent.h"
#include "EditorObjectManager.h"
#include "Widgets/Menu.h"
#include "Widgets/MenuItem.h"

#include <Engine/Core/Action.h>
#include <Engine/Core/ActionManager.h>
#include <Engine/Core/SmartPtr.h>

#include <imgui/imgui.h>

using namespace Logicraft;

Logicraft::PanelOutliner::PanelOutliner()
{
	MenuPtr pMenuNew = make_shared(Menu, "New Object");
	m_menuBar.AddChild(pMenuNew);
	pMenuNew->SetAction(EditorObjectManager::Get().GetActionCreateObject());
}

void Logicraft::PanelOutliner::Draw()
{
	auto& objects = EditorObjectManager::Get().GetObjects();
	for (auto& pObject : objects)
	{
		ImGui::Text(pObject->GetName().c_str());
	}
}
