#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Logicraft
{
class Widget;
using WidgetPtr = std::shared_ptr<Widget>;

class Widget
{
public:
	Widget() = default;
	explicit Widget(const std::string& name);

	virtual void Update();
	virtual void Draw();

protected:
	std::string              m_name;
	std::vector<WidgetPtr> m_children;
};
} // namespace Logicraft