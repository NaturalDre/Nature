#pragma once

union ALLEGRO_EVENT;
namespace VE
{
	class IInputController
	{
	public:
		IInputController(void);
		virtual ~IInputController(void);
		virtual void HandleEvent(const ALLEGRO_EVENT& ev) = 0;
	};
}