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
#include <Engine/Core/AsyncLoadedObject.h>

#include <memory>
#include <string>
#include <vector>

namespace Logicraft
{
#define LOGI_DECLARE_PANEL(type) inline static TypedRegisterer<type> s_registerer{#type};
// How to declare a panel class :
//	class MyPanelClass : public Panel
//	{
//		LOGI_DECLARE_PANEL(MyPanelClass)
// 	public:
// 		...
//	};

class Panel : public AsyncLoadedObject
{
public:
	Panel(const char* name);

	virtual void Update() {}
	virtual void Draw() = 0;

	const std::string& GetName() const { return m_name; }

	void SetVisible(bool visible) { m_visible = visible; }
	bool IsVisible() const { return m_visible; }

protected:
	void Load() override;

protected:
	std::string m_name;
	bool        m_visible{true};
};
using PanelPtr = std::shared_ptr<Panel>;

class PanelRegisterer
{
public:
	inline static std::vector<PanelRegisterer*> s_registerers;

	PanelRegisterer(const char* panelName)
	  : m_panelName(panelName)
	{
		s_registerers.emplace_back(this);
	}
	virtual std::shared_ptr<Panel> Create() = 0;
	std::string                    m_panelName;
};

template<typename C>
class TypedRegisterer : public PanelRegisterer
{
public:
	TypedRegisterer(const char* panelName)
	  : PanelRegisterer(panelName)
	{
	}
	std::shared_ptr<Panel> Create() override { return std::make_shared<C>(m_panelName.c_str()); }
};
} // namespace Logicraft