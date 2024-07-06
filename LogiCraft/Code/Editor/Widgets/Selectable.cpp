#include "Selectable.h"

#include <imgui.h>

namespace Logicraft
{
Selectable::Selectable(const char* name)
  : Widget(name)
{
}
void Selectable::Update() {}
void Selectable::Draw()
{
	if (ImGui::Selectable(m_name.c_str(), m_isSelected))
	{
		m_isSelected = !m_isSelected;
	}
}	
} // namespace Logicraft
