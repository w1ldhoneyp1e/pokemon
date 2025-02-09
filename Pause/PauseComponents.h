#pragma once
#include "../Components/ParentComponent.h"

enum class PauseButtonType {
	Continue,
	Restart
};

class PauseComponent : public Component {};
class PauseButtonComponent : public Component {
	private:
		PauseButtonType type;
	public:
		PauseButtonComponent(PauseButtonType type) : type(type) {}
		PauseButtonType getType() { return type; }
};
