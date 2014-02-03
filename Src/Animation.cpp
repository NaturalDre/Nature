#include "Animation.h"
#include "Locator.h"
#include "GameManager.h"

namespace VE
{
	IAnimation::IAnimation(bool autoLogic)
	{
		if (autoLogic)
			CLocator::GetGameManager()->AddAnimation(this);
	}

	IAnimation::~IAnimation(void)
	{
		CLocator::GetGameManager()->RemoveAnimation(this);
	}
}