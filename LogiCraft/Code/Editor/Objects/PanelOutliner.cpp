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

#include "Core/Editor.h"
#include "EditorComponent.h"
#include "EditorObjectManager.h"
#include "Widgets/Menu.h"
#include "Widgets/MenuItem.h"
#include "Widgets/SelectionManager.h"

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

	Editor::Get().GetEventSystem().AddAsyncListener(Editor::eObjectChanged, [this]() { m_refreshObjectList = true; });
}

Logicraft::PanelOutliner::~PanelOutliner()
{
	m_objects.clear();
}

void Logicraft::PanelOutliner::Update()
{
	if (m_refreshObjectList)
	{
		RefrectObjectList();
		m_refreshObjectList = false;
	}

	for (auto& selectable : m_objects)
	{
		selectable.first.Update();
		if (selectable.first.IsSelected()) 
		{
			if (m_pSelectedObject)
				if (m_pSelectedObject != &selectable && m_pSelectedObject->first.IsSelected()) 
					m_pSelectedObject->first.Selected(false);

			m_pSelectedObject = &selectable;
			SelectionManager::Get().SelectGameObject(selectable.second);
			Editor::Get().GetEventSystem().QueueEvent(Editor::eObjectSelectedChanged);
		}
	}
}

void Logicraft::PanelOutliner::Draw()
{
	for (auto& selectable : m_objects)
	{
		selectable.first.Draw();
	}
}

void Logicraft::PanelOutliner::RefrectObjectList()
{
	const std::vector<EditorObjectPtr>& managerObjects = EditorObjectManager::Get().GetObjects();
	if (m_objects.size() > managerObjects.size())
	{
		const EditorObjectPtr& newGameObject = SelectionManager::Get().SelectedGameObject();

		if (newGameObject) 
		{
			const auto objectIt = std::remove_if(m_objects.begin(),
			  m_objects.end(),
			  [&newGameObject](const std::pair<Selectable, EditorObjectPtr>& outlinerObject) { return outlinerObject.second == newGameObject; });

			m_objects.erase(objectIt);
		}

		// for (auto outlinerObject = m_objects.begin(); outlinerObject != m_objects.end();)
		//{
		//	const auto objectIt = std::find(managerObjects.begin(), managerObjects.end(), (*outlinerObject).second);

		//	if (objectIt == managerObjects.end())
		//		outlinerObject = m_objects.erase(outlinerObject);
		//	else
		//		outlinerObject++;
		//}
	}
	else
	{
		const EditorObjectPtr& newGameObject = SelectionManager::Get().SelectedGameObject();
		if (newGameObject)
			m_objects.emplace_back(std::make_pair(Selectable(newGameObject->GetName().c_str()), newGameObject));
		/*for (auto& managerObject : managerObjects)
		{
		  const auto objectIt = std::find_if(m_objects.begin(), m_objects.end(), [&managerObject](const std::pair<Selectable, EditorObjectPtr>&
		outlinerObject) { return outlinerObject.second == managerObject;
		  });

		  if (objectIt == m_objects.end())
		    m_objects.emplace_back(std::make_pair(Selectable(managerObject->GetName().c_str()), managerObject));
		}*/
	}
}
