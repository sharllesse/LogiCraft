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

#include <Core/Logger.h>
#include <Utils/SfmlUtils.h>

using namespace Logicraft;

Action::Action(const char* name)
  : m_name(name)
{
	SfmlUtils::ClearKeyEvent(m_shortcut);
}

void Action::Execute()
{
	std::string message = "Action executed: " + m_name;
	Logger::Get().Log(Logger::eInfo, message);
	m_callback();
}

void Action::SetCallback(std::function<void()>&& callback)
{
	m_callback = std::move(callback);
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

void Action::Serialize(bool load, Serializer& serializer)
{
	if (load)
	{
		json        action   = serializer[m_name];
		std::string shortcut = action["shortcut"];
		SetShortcut(shortcut);
		m_description = action["description"];
	}
	else
	{
		serializer[m_name]["shortcut"]    = m_shortcutStr;
		serializer[m_name]["description"] = m_description;
	}
}

void Action::Load()
{
	Serializer serializer;
	if (serializer.Parse("action.json"))
	{
		Serialize(true, serializer);
	}
}
