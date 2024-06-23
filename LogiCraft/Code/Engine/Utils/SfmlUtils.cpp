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

#include "SfmlUtils.h"

using namespace Logicraft;

void Logicraft::SfmlUtils::ClearKeyEvent(sf::Event::KeyEvent& event)
{
	event.control  = false;
	event.alt      = false;
	event.shift    = false;
	event.code     = sf::Keyboard::Unknown;
	event.scancode = sf::Keyboard::Scan::Unknown;
}

std::string Logicraft::SfmlUtils::KeyEventToString(sf::Event::KeyEvent& event)
{
	std::string eventString;

	if (event.control)
	{
		eventString += "Ctrl+";
	}
	if (event.alt)
	{
		eventString += "Alt+";
	}
	if (event.shift)
	{
		eventString += "Shift+";
	}
	eventString += event.code;
	return eventString;
}

void Logicraft::SfmlUtils::StringToKeyEvent(const std::string& str, sf::Event::KeyEvent& event)
{
	if (size_t plusPos = str.find_last_of('+'); plusPos != std::string::npos)
	{
		if (str.find("Ctrl") != std::string::npos)
		{
			event.control = true;
		}
		if (str.find("Alt") != std::string::npos)
		{
			event.alt = true;
		}
		if (str.find("Shift") != std::string::npos)
		{
			event.shift = true;
		}
		event.code = static_cast<sf::Keyboard::Key>(str[plusPos + 1]);
	}
	else
	{
		event.code = static_cast<sf::Keyboard::Key>(str[0]);
	}
}
