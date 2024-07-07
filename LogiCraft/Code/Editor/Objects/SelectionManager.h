#pragma once
#include "Objects/EditorObject.h"

namespace Logicraft
{
class SelectionManager
{
public:
	static SelectionManager& Get();

	SelectionManager();
	~SelectionManager();

	void                   SelectGameObject(const EditorObjectPtr& pObject);
	const EditorObjectPtr& SelectedGameObject() const { return m_pSelectedObject; }

private:
	EditorObjectPtr m_pSelectedObject;
};
} // namespace Logicraft
