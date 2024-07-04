#include "PanelGraphEditor.h"
#include "imgui/imgui.h"

Logicraft::PanelGraphEditor::PanelGraphEditor() {}

void Logicraft::PanelGraphEditor::Draw()
{
	ImGui::BeginChild("Graph Editor");
	m_graphEditor.Draw();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Editor Panel");
	if (m_editorPropertyPanel)
		m_editorPropertyPanel(&m_graphEditor);
	ImGui::EndChild();
}
