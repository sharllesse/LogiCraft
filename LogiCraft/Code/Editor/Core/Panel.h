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
#include "Widgets/MenuBar.h"

#include <Engine/Core/Serializable.h>
#include <Engine/Core/SmartPtr.h>

#include <string>
#include <vector>

namespace Logicraft
{
#define LOGI_DECLARE_PANEL(type, name)                                 \
	inline static PanelTypeRegisterer<type> s_registerer{name};          \
	const char*                             GetTypeName() const override \
	{                                                                    \
		return name;                                                       \
	}
// How to declare a panel class :
//	class MyPanelClass : public Panel
//	{
//		LOGI_DECLARE_PANEL(MyPanelClass)
// 	public:
// 		...
//	};

class Panel : public Serializable
{
public:
	virtual void Update() {}
	void         BaseDraw();

	virtual const char* GetTypeName() const = 0;

	void SetVisible(bool visible);
	bool IsVisible() const { return m_visible; }

	void Serialize(bool load, JsonObjectPtr pJsonObject) override;

protected:
	virtual void Draw() = 0;

	void Load() override;
	void Save() override;

protected:
	MenuBar m_menuBar;
	bool    m_visible{true};
};
using PanelPtr = std::shared_ptr<Panel>;

class PanelRegisterer
{
public:
	inline static std::vector<PanelRegisterer*> s_registerers;

	PanelRegisterer(const char* _typename)
	  : m_typename(_typename)
	{
		s_registerers.emplace_back(this);
	}
	virtual PanelPtr Create() = 0;

protected:
	std::string m_typename;
};

template<typename T>
class PanelTypeRegisterer : public PanelRegisterer
{
public:
	PanelTypeRegisterer(const char* _typename)
	  : PanelRegisterer(_typename)
	{
	}

	PanelPtr Create() override { return make_shared(C); }
};
} // namespace Logicraft