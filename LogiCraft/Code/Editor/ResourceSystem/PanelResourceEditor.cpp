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
#include "PanelResourceEditor.h"
#include "Objects/EditorComponent.h"
#include "ResourceSystem/EditorResource.h"
#include "ResourceSystem/EditorResourceManager.h"
#include "Widgets/Menu.h"

#include <Engine/Core/Action.h>
#include <Engine/Core/ActionManager.h>
#include <Engine/Core/TaskManager.h>
#include <imgui/imgui.h>

using namespace Logicraft;

Logicraft::PanelResourceEditor::PanelResourceEditor()
{
	MenuPtr pMenuNew = make_shared(Menu, "New");
	m_menuBar.AddChild(pMenuNew);

	for (auto& pResourceType : EditorResource::GetRegisteredTypes())
	{
		MenuItemPtr pItemNew = make_shared(MenuItem, pResourceType->GetName().c_str());
		pMenuNew->AddChild(pItemNew);
		ActionPtr pAction = ActionManager::Get().AddAction((std::string("new_") + pResourceType->GetName()).c_str());
		pAction->SetCallback([this, pResourceType] { CreateNewResource(pResourceType->GetName()); });
		pItemNew->SetAction(pAction);
	}
}

void Logicraft::PanelResourceEditor::Update() {}

void Logicraft::PanelResourceEditor::CreateNewResource(const std::string& resourceType)
{
	TaskManager::Get().AddTask([this, resourceType] { m_pEditedResource = EditorResourceManager::Get().CreateResource(resourceType.c_str()); });
}

void Logicraft::PanelResourceEditor::Draw()
{
	if (m_pEditedResource)
	{
		m_pEditedResource->DrawUI();
	}
}
