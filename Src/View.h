#ifndef VIEW_H
#define VIEW_H

namespace VE
{
	class IRenderService;
	class IView
	{
	protected:
		IView(int drawOrder = 0);


	public:
		virtual ~IView(void);
		virtual void Draw(void) = 0;

		inline int DrawOrder(void) const { return m_drawOrder; }
		inline void SetDrawOrder(int drawOrder) { m_drawOrder = drawOrder; }

	private:
		int m_drawOrder;
	};
}
#endif