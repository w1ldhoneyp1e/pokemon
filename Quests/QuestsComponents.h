#pragma once
#include "../Components/ParentComponent.h"

enum QuestButtonType {
	Ok,
	Cancel
};

class QuestDialogComponent : public Component {};
class QuestButtonComponent : public Component {
	private:
		QuestButtonType type;
	public:
		QuestButtonComponent(QuestButtonType type) : type(type) {}
		QuestButtonType getType() const { return type; }
};

class QuestAreaComponent : public Component {
	private:
		int trainerId;
	public:
		QuestAreaComponent(int trainerId) : trainerId(trainerId) {}
		int getTrainerId() const { return trainerId; }
};


