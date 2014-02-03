#ifndef MULTIANIMATION_H
#define MULTIANIMATION_H

struct ALLEGRO_BITMAP;


#include <list>
#include <string>
#include "BasicAnimation.h"
namespace VE
{
	struct FrameSequence 
	{ 
		std::string name;
		size_t begin, end;
		FrameSequence(void): begin(1), end(1) {}
		FrameSequence(std::string _name, size_t _begin, size_t _end): name(_name), begin(_begin), end(_end) {}
	};

	class CMultiAnimation: public IAnimation
	{
	protected:
		bool SequenceExists(const std::string& name) const;
		/// \return True is it found the sequence and copied its values to @param fs. False otherwise.
		bool CopySequence(const std::string& name, FrameSequence& fs);
		void Logic(double dt);

	public:
		CMultiAnimation(void);
		~CMultiAnimation(void);

		//bool SetSpriteSheet(const std::string& sheet, size_t rows, size_t cols, double fps);
		bool SetSpriteSheet(const alx::Bitmap& sheet, size_t rows, size_t cols, double fps);

		bool AddFrameSequence(const FrameSequence& fs);

		/// \nreturn Pointer to a basic animation if there is a current animation set, null otherwise.
		const CBasicAnimation* GetCurrentAnim(void) const { if (!GetCurrentAnimName().empty()) return &m_animation; else return nullptr; }
		const std::string& GetCurrentAnimName(void) const { return m_currentAnim; }


		void SetCurrentAnim(const std::string& name);
		void SetAlpha(size_t r, size_t g, size_t b);


		inline const alx::Bitmap& GetSpriteSheet(void) const { return m_animation.GetAnimationSheet(); }

	private:
		std::list<const FrameSequence> m_sequences;
		std::string m_currentAnim;
		CBasicAnimation m_animation;
	};
}
#endif