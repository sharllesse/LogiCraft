#include "Selectable.h"
#include "imgui.h"

namespace Logicraft
{
Selectable::Selectable(const char* name)
  : Widget(name)
{
}
void Selectable::Update() 
{

}
void Selectable::Draw() 
{
	m_isSelected = ImGui::Selectable(m_name.c_str());
}
} // namespace Logicraft
