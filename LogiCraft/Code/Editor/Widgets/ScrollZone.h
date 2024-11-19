#pragma once
#include <utility>
#include "Widget.h"
#include <string>
#include <vector>
#include <typeinfo>
#include <Engine/Core/Logger.h>
#include <imgui/imgui.h>

namespace Logicraft
{

class ScrollZone : public Widget
{
public:
	ScrollZone(const std::string& name);
	~ScrollZone();
	void Draw();
};
} // namespace Logicraft
