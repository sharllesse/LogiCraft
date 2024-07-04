#pragma once

#include "Widgets/Widget.h"

namespace Logicraft
{
class Selectable : public Widget
{
public:
	Selectable() = default;
	explicit Selectable(const char* name);

	void Update() override;
	void Draw() override;

	const bool& IsSelected() const { return m_isSelected; }
private:
	bool m_isSelected{false};
};
} // namespace Logicraft
