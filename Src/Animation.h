#ifndef ANIMATION_H
#define ANIMATION_H

namespace VE
{
	class IAnimation
	{
	protected:
		IAnimation(bool autoLogic = true);

	public:
		virtual void Logic(double dt) = 0;
		virtual ~IAnimation(void);

	private:
	};
};
#endif