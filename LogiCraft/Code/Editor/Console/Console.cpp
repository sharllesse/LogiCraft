#include "Console/Console.h"

#include <Engine/Core/Action.h>
#include <Engine/Core/ActionManager.h>

using namespace Logicraft;

Console::Console()
  : Panel()
  , m_historyPos(-1)
  , m_autoScroll(true)
  , m_scrollZone(ScrollZone("Output"))
  , m_inputText(InputText("Input"))
{
	sConsole             = this;
	MenuPtr pMenuOptions = std::make_shared<Menu>("Options");
	m_menuBar.AddChild(pMenuOptions);

	MenuItemPtr pOption = std::make_shared<MenuItem>("Clear Console");
	pMenuOptions->AddChild(pOption);
	ActionPtr pAction = ActionManager::Get().AddAction("clear_console");
	pAction->SetCallback([this] { ClearLog(); });
	pOption->SetAction(pAction);

	pOption = std::make_shared<MenuItem>("Search Line");
	pMenuOptions->AddChild(pOption);
	pAction = ActionManager::Get().AddAction("search_line");
	pAction->SetCallback([this] {
		// Add line searching
	});
	pOption->SetAction(pAction);

	/*Ajouter action de filtrage des lignes existantes*/

	pOption = std::make_shared<MenuItem>("Auto Scroll ON");
	pMenuOptions->AddChild(pOption);
	pAction = ActionManager::Get().AddAction("auto_scroll");
	pAction->SetCallback([this, pOption] {
		m_autoScroll = !m_autoScroll;
		pOption->setName(m_autoScroll ? "Auto Scroll ON" : "Auto Scroll OFF");
		AddLog(Logger::eInfo, m_autoScroll ? "Auto Scroll ON" : "Auto Scroll OFF");
	});
	pOption->SetAction(pAction);

	// push back un objet complet (loglevel + message)
	Logger::Get().GetEventSystem().AddListener<EventLog>(this, [this](const EventLog& _event) {
		// améliorer et utiliser AddLog()
		AddLog(_event.m_level, _event.m_message);
	});

	ClearLog();
	m_commands.push_back("HELP");    // imgui
	m_commands.push_back("HISTORY"); // imgui
	m_commands.push_back("CLEAR");   // imgui
}

Console::~Console()
{
	//Logger::Get().GetEventSystem().RemoveListener<EventLog>(this);
	ClearLog();
}

void Console::ClearLog()
{
	m_items.clear();
}

void Console::AddLog(Logger::ELogLevel level, const std::string& message)
{
	// change moi stp (structure Outputrow)
	std::string finalMessage = "";
	if (level == Logger::eCommand)
		finalMessage = "COMMAND: " + message;
	else if (level == Logger::eInfo)
		finalMessage = "INFO: " + message;
	else if (level == Logger::eWarning)
		finalMessage = "WARNING: " + message;
	else if (level == Logger::eError)
		finalMessage = "ERROR: " + message;
	// m_items.push_back(std::make_pair("test", finalMessage));
	m_items.push_back(std::make_pair(level, finalMessage));
}

void Console::ExecCommand(const std::string& command_line)
{
	// Utiliser ActionManager::Get().Execute();
	/*AddLog(command_line);
	m_historyPos = -1;
	std::vector<std::string>::iterator it = std::find(m_history.begin(), m_history.end(), command_line);
	if (it != m_history.end())
	{
	  m_history.erase(it);
	}
	m_history.push_back(command_line);*/

	// if (std::strcmp(command_line.c_str(), "CLEAR") == 0)
	//{
	//	sConsole->ClearLog();
	// }
	// else if (std::strcmp(command_line.c_str(), "HELP") == 0)
	//{
	//	sConsole->AddLog(Logger::eCommand, std::string("Commands:"));
	//	for (const auto& cmd : sConsole->m_commands)
	//	{
	//		sConsole->AddLog(Logger::eCommand, std::string("- " + cmd));
	//	}
	// }
	// else if (std::strcmp(command_line.c_str(), "HISTORY") == 0)
	//{
	//	int first = static_cast<int>(sConsole->m_history.size()) - 10;
	//	for (size_t i = first > 0 ? first : 0; i < sConsole->m_history.size(); ++i)
	//	{
	//		sConsole->AddLog(Logger::eCommand, std::string(std::to_string(i) + ": " + sConsole->m_history[i]));
	//	}
	// }
	// else
	//{
	//	sConsole->AddLog(Logger::eCommand, std::string("Unknown command: '" + command_line + "'"));
	// }
}

const std::vector<std::string>& Console::GetHistory() const
{
	return m_history;
}

const std::vector<std::string>& Console::GetCommands() const
{
	return m_commands;
}

std::string Console::Strtrim(std::string str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), str.end());
	return str;
}

int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
		{
			// TODO: completion code
			break;
		}
	case ImGuiInputTextFlags_CallbackHistory:
		{
			const int prev_history_pos = m_historyPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (m_historyPos == -1)
					m_historyPos = static_cast<int>(m_history.size()) - 1;
				else if (m_historyPos > 0)
					m_historyPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (m_historyPos != -1)
					if (++m_historyPos >= static_cast<int>(m_history.size()))
						m_historyPos = -1;
			}

			if (prev_history_pos != m_historyPos)
			{
				const std::string& history_str = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str.c_str());
			}
		}
	}
	return 0;
}

void Console::Draw()
{
	m_menuBar.Draw();
	for (auto& item : m_items)
	{
		ImGui::Text(item.second.c_str());
	}
	m_scrollZone.Draw();
	m_inputText.Draw();
}
