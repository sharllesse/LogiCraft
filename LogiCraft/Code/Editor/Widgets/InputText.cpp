#include "InputText.h"
#include "Console/Console.h"

using namespace Logicraft;

Logicraft::InputText::InputText(const std::string& name)
  : Widget(name)
{
	m_buffer.reserve(256);
}

InputText::~InputText() {}

void Logicraft::InputText::Draw() 
{
	ImGui::SetItemDefaultFocus();
	if (ImGui::InputText(m_name.c_str(), m_buffer.data(), m_buffer.capacity(), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::string command_line = Console::Strtrim(m_buffer);
		if (command_line.empty())
		{
			Console::Get().GetScrollZone().AddChild(UnformatedText)
			Console::ExecCommand(command_line);
		}
		m_buffer.clear();
	}
	// if (ImGui::InputText(
	//       m_name.c_str(),
	//       buffer.data(),
	//       buffer.capacity(),
	//       ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory,
	//       [&](ImGuiInputTextCallbackData* data) { return static_cast<Console*>(data->UserData)->TextEditCallback(data); },
	//       this)/* && m_timer > 0.5f*/)
	//{
	//	 std::string command_line = Strtrim(buffer);
	//	if (command_line.empty())
	//		ExecCommand(command_line);
	//	buffer.clear();
	//	m_timer = 0.f;
	// }
	ImGui::Separator();
}
