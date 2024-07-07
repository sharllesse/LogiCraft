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

	void Update() override;
	void Draw() override;

	const bool& IsSelected() const { return m_isSelected; }
	void Select(const bool& selected) { m_isSelected = selected; }
private:
	bool m_isSelected{false};

	EventSystem m_eventSystem;
};
} // namespace Logicraft
