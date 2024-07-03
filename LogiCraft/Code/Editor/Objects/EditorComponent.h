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
#include <Engine/Objects/GameComponent.h>

#include <memory>
#include <string>

namespace Logicraft
{
#define LOGI_DECLARE_EDITOR_COMPONENT(editorComponentType, gameComponentType)                                              \
	inline static EditorComponentTypeRegisterer<editorComponentType> s_registerer{#editorComponentType, #gameComponentType}; \
	EditorComponentRegisterer&                                       GetTypeClass() const override                           \
	{                                                                                                                        \
		return s_registerer;                                                                                                   \
	}
// How to declare a component class :
//	class MyComponentClass : public EditorComponent
//	{
//		LOGI_DECLARE_EDITOR_COMPONENT(MyComponentClass)
// 	public:
// 		...
//	};

class EditorComponentRegisterer;
class EditorComponent
{
public:
	const GameComponentPtr GetGameComponent() const { return m_pGameComponent; }
	void                   SetGameComponent(GameComponentPtr pGameComponent) { m_pGameComponent = pGameComponent; }

	virtual void DrawUI() const = 0;

	virtual EditorComponentRegisterer& GetTypeClass() const = 0;

protected:
	GameComponentPtr m_pGameComponent;
};
using EditorComponentPtr = std::shared_ptr<EditorComponent>;

class EditorComponentRegisterer
{
public:
	inline static std::vector<EditorComponentRegisterer*> s_registerers;

	EditorComponentRegisterer(const char* editorComponentType, const char* gameComponentType)
	  : m_editorComponentType(editorComponentType)
	  , m_gameComponentType(gameComponentType)
	{
		s_registerers.emplace_back(this);
	}

	const std::string& GetEditorTypeName() const { return m_editorComponentType; }
	const std::string& GetGameTypeName() const { return m_gameComponentType; }

protected:
	friend class EditorObjectManager;
	virtual EditorComponentPtr Create() = 0;

private:
	std::string m_editorComponentType;
	std::string m_gameComponentType;
};

template<typename T>
class EditorComponentTypeRegisterer : public EditorComponentRegisterer
{
public:
	EditorComponentTypeRegisterer(const char* editorComponentType, const char* gameComponentType)
	  : EditorComponentRegisterer(editorComponentType, gameComponentType)
	{
	}

protected:
	EditorComponentPtr Create() override { return std::make_shared<T>(); }
};
} // namespace Logicraft