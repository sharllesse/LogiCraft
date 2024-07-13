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

#include "Action.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Serializer.h"
#include "Utils/SfmlUtils.h"

#include <algorithm>
#include <cctype>

using namespace Logicraft;

Action::Action(const char* name)
  : m_name(name)
{
	SfmlUtils::ClearKeyEvent(m_shortcut);
	Engine::Get().GetEventSystem().AddQueuedEventCallback(this, Engine::eProcessedEvents, [this] { m_eventSystem.ProcessEvents(); });
	m_eventSystem.AddQueuedEventCallback(this, Engine::eActionExecuteRequested, [this] { Execute(); });
}

void Action::Execute()
{
	if (m_callback)
	{
		if (s_logEnabled)
			Logger::Get().Log(Logger::eInfo, "Action will be executed: " + m_name);

		m_callback();
	}
	else if (s_logEnabled)
	{
		Logger::Get().Log(Logger::eError, "Action has no callback: " + m_name);
	}
}

void Logicraft::Action::ExecuteLater()
{
	m_eventSystem.QueueEvent(Engine::eActionExecuteRequested);
}

void Action::SetCallback(std::function<void()>&& callback)
{
	m_callback = std::move(callback);

	for (auto it = s_actionsToExecute.begin(); it != s_actionsToExecute.end(); ++it)
	{
		if (m_name == (*it))
		{
			Execute();
			s_actionsToExecute.erase(it);
			return;
		}
	}
}

void Action::SetShortcut(const std::string& shortcut)
{
	m_shortcutStr = shortcut;
	SfmlUtils::ClearKeyEvent(m_shortcut);
	SfmlUtils::StringToKeyEvent(shortcut, m_shortcut);
}

std::string Action::GetShortcutString() const
{
	return m_shortcutStr;
}

void Action::Serialize(bool load, JsonObjectPtr pJsonObject)
{
	if (load)
	{
		if (JsonObjectPtr pNameObject = pJsonObject->GetObject(m_name.c_str()))
		{
			if (StringPtr pShortcut = pNameObject->GetString("shortcut"))
			{
				SetShortcut(*pShortcut);
			}
			pNameObject->GetString("description", m_description);
		}
	}
	else
	{
		JsonObjectPtr pNameObject = pJsonObject->AddObject(m_name.c_str());
		pNameObject->AddString("shortcut", m_shortcutStr);
		pNameObject->AddString("description", m_description);
	}
}

void Action::Load()
{
	Serializer serializer;
	if (serializer.Parse("action.json"))
	{
		JsonObjectPtr pRoot = serializer.GetRoot();
		Serialize(true, pRoot);
	}
}

void Logicraft::Action::SetActionsToExecute(std::vector<std::string>& actionsToExecute)
{
	s_actionsToExecute.swap(actionsToExecute);
}