#include "Selectable.h"

#include <imgui.h>

namespace Logicraft
{
WidgetSelectableText::WidgetSelectableText(const char* name)
  : Widget(name)
{
}
void WidgetSelectableText::Update() {}
void WidgetSelectableText::Draw()
{
	if (ImGui::Selectable(m_name.c_str(), m_isSelected))
	{
		m_isSelected = !m_isSelected;
	}
}	
} // namespace Logicraft
