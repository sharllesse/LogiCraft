#include "EditorTexture.h"
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
#include "EditorTexture.h"

#include <Engine/Core/ActionManager.h>
#include <Engine/ResourceSystem/Resources/Texture.h>
#include <imgui/imgui.h>

using namespace Logicraft;

Logicraft::EditorTexture::EditorTexture() {}

void Logicraft::EditorTexture::DrawUI()
{
	EditorResource::DrawUI();

	if (TexturePtr pTexture = std::dynamic_pointer_cast<Texture>(m_pResource))
	{
		if (ImGui::Button(pTexture->GetFilePath().empty() ? "<Select A File>" : pTexture->GetFilePath().c_str()))
		{
			pTexture->SetFilePath("C:/Users/rciron/Documents/GitHub/LogiCraft/Sample_Resources/200x200D.png");
			pTexture->GetTexture().loadFromFile("C:/Users/rciron/Documents/GitHub/LogiCraft/Sample_Resources/200x200D.png");
		}
	}
}

TexturePtr Logicraft::EditorTexture::GetTexture()
{
	return std::dynamic_pointer_cast<Texture>(m_pResource);
}
