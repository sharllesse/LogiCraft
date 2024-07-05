#include "PanelGraphEditor.h"
#include "Utils/ImGuiUtils.h"
#include "imgui/imgui.h"

Logicraft::PanelGraphEditor::PanelGraphEditor() {}

void Logicraft::PanelGraphEditor::Draw()
{
	ImGui::BeginChild("Tree information##lc",
	  ImVec2((ImGui::GetWindowSize().x / 2) - 20, ImGui::GetWindowSize().y - 55),
	  true,
	  ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	m_graphEditor.Draw();
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("Node information##lc", ImVec2((ImGui::GetWindowSize().x / 2) - 20, ImGui::GetWindowSize().y - 55), true);
	if (m_editorPropertyPanel)
		m_editorPropertyPanel(&m_graphEditor);
	ImGui::EndChild();
}
