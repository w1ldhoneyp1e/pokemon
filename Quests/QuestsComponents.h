#pragma once
#include "../Components/ParentComponent.h"

enum QuestButtonType {
	Ok,
	Cancel
};

class QuestAreaComponent : public Component {};
class QuestDialogComponent : public Component {};
class QuestButtonComponent : public Component {
	private:
		QuestButtonType type;
	public:
		QuestButtonComponent(QuestButtonType type) : type(type) {}
		QuestButtonType getType() const { return type; }
};


