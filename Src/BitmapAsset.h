#pragma once
#include "Asset.h"
#include <memory>
#include <Bitmap.hpp>

namespace VE
{
	class CBitmap;
	class CBitmapAsset: public IAsset
	{
	protected:
		void OnLoad(void);
		void OnUnload(void);

	public:
		CBitmapAsset(const std::string& file);
		const alx::Bitmap GetBitmap(void) const { return m_bitmap; }
	private:
		alx::Bitmap m_bitmap;
	};
}