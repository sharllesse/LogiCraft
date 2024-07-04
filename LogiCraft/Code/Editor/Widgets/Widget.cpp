#include "Widget.h"

using namespace Logicraft;

Logicraft::Widget::Widget(const std::string& name) 
	: m_name(name)
{
}

void Logicraft::Widget::AddChild(WidgetPtr pChild)
{
	m_children.push_back(pChild);
}

void Logicraft::Widget::Draw()
{
	for (WidgetPtr& child : m_children)
	{
		child->Draw();
	}
}
