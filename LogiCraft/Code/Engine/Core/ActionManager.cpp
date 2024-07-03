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

#include "ActionManager.h"

#include "Logger.h"
#include "Serializer.h"

#include <assert.h>
#include <utility>

using namespace Logicraft;

ActionManager* s_pActionsManager = nullptr;

ActionManager& ActionManager::Get()
{
	assert(s_pActionsManager);
	return *s_pActionsManager;
}

ActionManager::ActionManager()
{
	assert(!s_pActionsManager);
	s_pActionsManager = this;
}

ActionManager::~ActionManager()
{
	s_pActionsManager = nullptr;
}

ActionPtr ActionManager::AddAction(const char* name)
{
	ActionPtr pAction = make_shared(Action, name);
	m_actions.push_back(pAction);

	if (IsLoaded())
	{
		// If the manager is already loaded and a new action is added, load it alone
		// But it should be avoided in general
		std::string message = "Action added after loading: " + pAction->GetName();
		Logger::Get().Log(Logger::eWarning, message);
		pAction->StartLoading();
	}

	return pAction;
}

bool Logicraft::ActionManager::ExecuteAction(const char* name)
{
	for (ActionPtr& action : m_actions)
	{
		if (action->GetName() == name)
		{
			action->Execute();
			return true;
		}
	}
	return false;
}

void ActionManager::Serialize(bool load, JsonObjectPtr pJsonObject)
{
	for (ActionPtr& action : m_actions)
	{
		action->Serialize(load, pJsonObject);
	}
}

void ActionManager::Save()
{
	Serializer    serializer;
	JsonObjectPtr pRoot = serializer.CreateRoot();
	Serialize(false, pRoot);
	serializer.Write("action.json");
}

void ActionManager::Load()
{
	Serializer serializer;
	if (serializer.Parse("action.json"))
	{
		JsonObjectPtr pRoot = serializer.GetRoot();
		Serialize(true, pRoot);
	}
}
