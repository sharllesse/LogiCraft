#pragma once

#include "Widgets/Widget.h"

#include <Engine/Core/EventSystem.h>

namespace Logicraft
{
class WidgetSelectableText : public Widget
{
public:
	WidgetSelectableText() = default;
	explicit WidgetSelectableText(const char* name);

	void Draw() override;

	bool IsSelected() const { return m_isSelected; }

	EventSystem& GetEventSystem() { return m_eventSystem; }
private:
	bool m_isSelected{false};

	EventSystem m_eventSystem;
};

using WidgetSelectableTextPtr = std::shared_ptr<WidgetSelectableText>;
} // namespace Logicraft
