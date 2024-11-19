#include "SelectionManager.h"

#include "Core/Editor.h"

#include <assert.h>

using namespace Logicraft;

SelectionManager* s_pSelectionManager = nullptr;

SelectionManager& SelectionManager::Get()
{
	assert(s_pSelectionManager);
	return *s_pSelectionManager;
}

SelectionManager::SelectionManager()
{
	assert(!s_pSelectionManager);
	s_pSelectionManager = this;
}

SelectionManager::~SelectionManager()
{
	m_pSelectedObject = nullptr;
}

void Logicraft::SelectionManager::UnSelectObject(EditorObjectPtr pObject)
{
	m_pSelectedObject = nullptr;
	Editor::Get().GetEventSystem().SendEvent(EventObjectSelected(pObject, false));
}

void Logicraft::SelectionManager::SelectObject(EditorObjectPtr pObject)
{
	m_pSelectedObject = pObject;
	Editor::Get().GetEventSystem().SendEvent(EventObjectSelected(pObject, true));
}
