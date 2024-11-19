#pragma once

#include "Editor/Core/Panel.h"
#include "Editor/Widgets/InputText.h"
#include "Editor/Widgets/Menu.h"
#include "Editor/Widgets/MenuBar.h"
#include "Editor/Widgets/MenuItem.h"
#include "Editor/Widgets/ScrollZone.h"
#include <imgui/imgui.h>

namespace Logicraft
{
class Console;
static Console* sConsole = nullptr;
class Console : public Panel
{
	LOGI_TYPEDEF_DERIVED_TYPE(Panel, Console, "Console")

	struct OutputRow
	{
		Logger::ELogLevel level;
		std::string       message;
	};

public:
	Console();
	~Console();

	void            ClearLog();
	static Console& Get() { return *sConsole; }
	void            AddLog(Logger::ELogLevel level, const std::string& message);
	static void     ExecCommand(const std::string& command_line);

	static std::string Strtrim(std::string str);
	int                TextEditCallback(ImGuiInputTextCallbackData* data);

	const std::vector<std::string>& GetHistory() const;
	const std::vector<std::string>& GetCommands() const;
	ScrollZone&                     GetScrollZone() { return m_scrollZone; }

	void Draw() override;

protected:
	MenuBar    m_menuBar;
	ScrollZone m_scrollZone;
	InputText  m_inputText;

private:

private:
	std::vector<std::pair<Logger::ELogLevel, std::string>> m_items;
	// std::vector<std::pair<std::string, std::string>> m_items;
	std::vector<std::string> m_commands;
	std::vector<std::string> m_history;
	int                      m_historyPos; // -1: new line, 0..History.Size-1 browsing history.
	bool                     m_autoScroll;
};
} // namespace Logicraft