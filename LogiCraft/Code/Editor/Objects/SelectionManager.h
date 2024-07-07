#pragma once
#include "Objects/EditorObject.h"

namespace Logicraft
{
class SelectionManager
{
public:
	struct EventObjectSelected
	{
		explicit EventObjectSelected(EditorObjectPtr pObject, bool selected)
		  : pObject(pObject), selected(selected)
		{
		}
	
		inline static int ID = 5;
		EditorObjectPtr pObject;
		bool          selected;
	};

	static SelectionManager& Get();

	SelectionManager();
	~SelectionManager();

	void                   UnSelectObject(EditorObjectPtr pObject);
	void                   SelectObject(EditorObjectPtr pObject);
	EditorObjectPtr SelectedObject() const { return m_pSelectedObject; }

private:
	EditorObjectPtr m_pSelectedObject;
};
} // namespace Logicraft
