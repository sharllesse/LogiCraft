#include "Selectable.h"

#include "Core/Editor.h"

#include <imgui.h>

namespace Logicraft
{
WidgetSelectableText::WidgetSelectableText(const char* name)
  : Widget(name)
{

}
void WidgetSelectableText::Draw()
{
	m_eventSystem.ProcessEvents();
	if (ImGui::Selectable(m_name.c_str(), m_isSelected))
	{
		m_isSelected = !m_isSelected;
		m_eventSystem.QueueEvent(Editor::eSelectable);
	}
}

} // namespace Logicraft
