/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) 2024 CIRON Robin
Copyright (c) 2024 GRALLAN Yann
Copyright (c) 2024 LESAGE Charles
Copyright (c) 2024 MENA-BOUR Samy

This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:

- [SFML](https://github.com/SFML)
- [ImGUI](https://github.com/ocornut/imgui)
- [ImNodes](https://github.com/Nelarius/imnodes)
- [SFML-Manager](https://github.com/Xanhos/Cpp-Manager-for-SFML)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
---------------------------------------------------------------------------------*/
#include "GraphEditor.h"
#include "SFML/Window/Keyboard.hpp"

using namespace Logicraft;

GUID Logicraft::GraphEditor::AddNode(const char* name)
{
	Node node;
	node.m_name = name;
	node.m_id   = Logicraft::GuidUtils::CreateGUID();
	m_nodes.emplace_back(std::move(node));
	return node.m_id;
}

void Logicraft::GraphEditor::Update() {}

void GraphEditor::Draw()
{
	ImNodes::BeginNodeEditor();
	for (auto& node : m_nodes)
	{
		ImNodes::BeginNode(node.m_id.Data1);
		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(node.m_name.c_str());
		ImNodes::EndNodeTitleBar();

		for (auto& inputAttribute : node.m_inputAttributes)
		{
			ImNodes::BeginInputAttribute(inputAttribute.first.Data1);
			inputAttribute.second();
			ImNodes::EndInputAttribute();
		}

		for (auto& outputAttribute : node.m_outputAttributes)
		{
			ImNodes::BeginOutputAttribute(outputAttribute.first.Data1);
			outputAttribute.second();
			ImNodes::EndOutputAttribute();
		}

		ImNodes::EndNode();
	}
	DrawLinks();
	ImNodes::MiniMap(0.1f, ImNodesMiniMapLocation_BottomLeft);
	ImNodes::EndNodeEditor();
	CreateLink();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
		DeleteLink(m_links.empty() ? GUID() : m_links.back().m_id);
}

size_t Logicraft::GraphEditor::GetNodeSize()
{
	return m_nodes.size();
}

std::vector<GraphEditor::Node>::iterator Logicraft::GraphEditor::GetNode(REFGUID id)
{
	std::vector<Node>::iterator it =
	  std::find_if(m_nodes.begin(), m_nodes.end(), [id](const Node& nodeCompare) { return IsEqualGUID(nodeCompare.m_id, id); });
	if (it != m_nodes.end())
		return it;
	return std::vector<GraphEditor::Node>::iterator{};
}

std::vector<GraphEditor::Link>::iterator Logicraft::GraphEditor::GetLink(REFGUID id)
{
	std::vector<Link>::iterator it =
	  std::find_if(m_links.begin(), m_links.end(), [id](const Link& nodeCompare) { return IsEqualGUID(nodeCompare.m_id, id); });
	if (it != m_links.end())
		return it;
	return std::vector<GraphEditor::Link>::iterator{};
}

void Logicraft::GraphEditor::CreateLink()
{
	int start_attr, end_attr;

	if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
	{
		Link link = {Logicraft::GuidUtils::CreateGUID(), std::make_pair(start_attr, end_attr)};
		m_links.emplace_back(std::move(link));
	}
}

void Logicraft::GraphEditor::DrawLinks()
{
	for (auto& link : m_links)
		ImNodes::Link(link.m_id.Data1, link.m_nodeLinked.first, link.m_nodeLinked.second);
}

void Logicraft::GraphEditor::DeleteLink(REFGUID id)
{
	std::vector<Link>::iterator it = GetLink(id);
	if (it._Unwrapped())
	{
		if (it != m_links.end())
		{
			m_links.erase(it);
		}
	}
}
