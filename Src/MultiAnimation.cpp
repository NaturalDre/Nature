#include "MultiAnimation.h"
#include "AssetManager.h"
#include "BitmapAsset.h"

namespace VE
{
	CMultiAnimation::CMultiAnimation(void)
		: IAnimation(false)
	{

	}

	CMultiAnimation::~CMultiAnimation(void)
	{

	}

	bool CMultiAnimation::SequenceExists(const std::string& name) const
	{
		for (auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
		{
			if ((*iter).name == name)
				return true;
		}
		return false;
	}

	void CMultiAnimation::Logic(double dt)
	{

	}

	void CMultiAnimation::SetAlpha(size_t r, size_t g, size_t b)
	{

	}

	//bool CMultiAnimation::SetSpriteSheet(const std::string& sheet, size_t rows, size_t cols, double fps)
	//{
	//	auto bitmap = std::static_pointer_cast<CBitmapAsset>(CAssetManager::Instance()->GetAsset(sheet));
	//	return SetSpriteSheet(bitmap->GetBitmap(), rows, cols, fps);
	//}

	bool CMultiAnimation::SetSpriteSheet(const alx::Bitmap& sheet, size_t rows, size_t cols, double fps)
	{
		m_animation = CBasicAnimation(sheet, rows, cols, fps);
		return (m_animation.GetAnimationSheet() ? true : false);
	}

	bool CMultiAnimation::AddFrameSequence(const FrameSequence& fs)
	{
		if (fs.name.empty() || fs.begin < 1 || fs.end < 1)
			return false;

		m_sequences.push_back(fs);
		return true;
	}

	void CMultiAnimation::SetCurrentAnim(const std::string& name)
	{
		FrameSequence fs;

		// If the name is an empty string or is not the key of any known sequence, do nothing.
		if (name.empty() || !CopySequence(name, fs))
			return;
		// The name of the current animation sequence
		m_currentAnim = name;
		// Set the begin and end frames based on the data in the FrameSequence struct.

		m_animation.SetBeginFrame(fs.begin);
		m_animation.SetEndFrame(fs.end);
		m_animation.SetCurrentFrame(fs.begin);
		m_animation.Reset();

	}

	bool CMultiAnimation::CopySequence(const std::string& name, FrameSequence& fs)
	{
		if (!SequenceExists(name))
			return false;

		for (auto iter = m_sequences.begin(); iter != m_sequences.end(); ++iter)
		{
			if (iter->name == name)
			{
				fs = *iter;
				break;
			}
		}

		return true;
	}
}