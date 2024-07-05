#include "SelectionManager.h"

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

void Logicraft::SelectionManager::SelectGameObject(const EditorObjectPtr& pObject)
{
	m_pSelectedObject = pObject;
}
