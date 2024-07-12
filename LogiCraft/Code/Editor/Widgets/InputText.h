#pragma once
#include <imgui/imgui.h>
#include <string>
#include "Widget.h"

namespace Logicraft
{

class InputText : public Widget
{
public:
	InputText(const std::string& name);
	~InputText();
	void Draw();
	const std::string& GetBuffer() { return m_buffer; }

private:
	std::string m_buffer;
};
} // namespace Logicraft