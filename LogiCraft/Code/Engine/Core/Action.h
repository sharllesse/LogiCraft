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

#pragma once
#include "DLLExport.h"
#include "EventSystem.h"
#include "Serializable.h"

#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>
#include <string>

namespace Logicraft
{
class LOGI_ENGINE_API Action : public Serializable
{
public:
	explicit Action(const char* name);

	void Execute();
	void ExecuteLater();

	void               SetCallback(std::function<void()>&& callback);
	void               SetShortcut(const std::string& shortcut);
	std::string        GetShortcutString() const;
	const std::string& GetName() const { return m_name; }

	void Serialize(bool load, JsonObjectPtr pJsonObject) override;

	static void SetActionsToExecute(std::vector<std::string>& actionsToExecute);

protected:
	void Load() override;

private:
	std::string           m_name;
	std::function<void()> m_callback;
	sf::Event::KeyEvent   m_shortcut;
	std::string           m_shortcutStr;
	std::string           m_description;
	EventSystem           m_eventSystem;

	inline static bool                     s_logEnabled{false};
	inline static std::vector<std::string> s_actionsToExecute;
};
using ActionPtr = std::shared_ptr<Action>;
} // namespace Logicraft