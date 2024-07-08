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

#include "EditorComponentSprite.h"
#include "ResourceSystem/EditorResourceManager.h"

#include <imgui/imgui.h>

using namespace Logicraft;

void Logicraft::EditorComponentSprite::Update() {}

void Logicraft::EditorComponentSprite::DrawUI()
{
	EditorComponent::DrawUI();
	if (ImGui::BeginCombo("Texture", m_pTexture ? m_pTexture->GetName().c_str() : "<Select A Texture>", ImGuiComboFlags_HeightLargest))
	{
		for (auto& pResource : EditorResourceManager::Get().GetResources())
		{
			if (&pResource->GetType() == &EditorTexture::GetTypeStatic())
			{
				const bool isSelected = (m_pTexture == pResource);
				if (ImGui::Selectable(pResource->GetName().c_str(), isSelected))
				{
					SetTexture(std::dynamic_pointer_cast<EditorTexture>(pResource));
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
}

void Logicraft::EditorComponentSprite::SetTexture(EditorTexturePtr pTexture)
{
	m_pTexture = pTexture;
	if (ComponentSpritePtr pGameComponent = std::dynamic_pointer_cast<ComponentSprite>(m_pGameComponent))
	{
		pGameComponent->SetTexture(pTexture->GetTexture());
	}
}
