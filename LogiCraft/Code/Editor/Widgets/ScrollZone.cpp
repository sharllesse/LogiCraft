#include "ScrollZone.h"

using namespace Logicraft;

Logicraft::ScrollZone::ScrollZone(const std::string& name) 
	: Widget(name)
{
}

ScrollZone::~ScrollZone() {}

void Logicraft::ScrollZone::Draw()
{
	ImGui::Separator();
	ImGui::BeginChild(m_name.c_str(), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (WidgetPtr& child : m_children)
	{
		child->Draw();
	}
	// for (int i = 0; i < pairs.size(); i++)
	//{
	//	// liste de widgets enfants.
	//	ImVec4 color(1.f,1.f,1.f,1.f);
	//	if (typeid(pairs[i].first).name() == "enum Logicraft::Logger::ELogLevel")
	//	{
	//		if (pairs[i].first == Logger::eError)
	//		{
	//			color = ImVec4(1.0f, 0.f, 0.f, 1.0f);
	//		}
	//		else if (pairs[i].first == Logger::eWarning)
	//		{
	//			color = ImVec4(1.0f, 1.f, 0.f, 1.0f);
	//		}
	//		else if (pairs[i].first == Logger::eCommand)
	//		{
	//			color = ImVec4(0.f, 1.f, 0.f, 1.0f);
	//		}
	//	}
	//	ImGui::PushStyleColor(ImGuiCol_Text, color);
	//	ImGui::TextUnformatted(std::string(std::to_string(i) + ":" + pairs[i].second.c_str()).c_str());
	//	ImGui::PopStyleColor();
	// }
	/*if (autoScroll)
		ImGui::SetScrollHereY(1.0f);*/
	ImGui::EndChild();
	ImGui::Separator();
}

