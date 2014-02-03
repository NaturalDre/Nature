#include "InputController.h"
#include "InputManager.h"

namespace VE
{
	IInputController::IInputController(void)
	{
		CInputManager::Instance()->Add(this);
	}

	IInputController::~IInputController(void)
	{
		CInputManager::Instance()->Remove(this);
	}
}