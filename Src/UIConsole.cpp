//#include "UIConsole.h"
//#include "Engine.h"
//#include "Utility.h"
//
//namespace VE
//{
//	CUIConsole::CUIConsole(CEngine* engine, GwenCanvas* canvas)
//		: GwenBase(canvas)
//		, m_engine(engine)
//		, m_tb(nullptr)
//		, m_lb(nullptr)
//	{
//		SetSize(canvas->Width(), canvas->Height());
//
//		m_lb = new GwenListBox(this);
//		m_lb->SetSize(canvas->Width(), canvas->Height() / 2);
//		m_lb->AddItem("Please enter a command in the textbox below this window.");
//		m_lb->SetShouldDrawBackground(false);
//		m_lb->SetTabable(true);
//
//		m_tb = new GwenTextBox(this);
//		m_tb->SizeToContents();
//		m_tb->SetPos(0, canvas->Height() - m_tb->Height() );
//		m_tb->SetWidth(canvas->Width());
//
//		m_tb->onReturnPressed.Add(this, &CUIConsole::OnEnter);
//	}
//
//	CUIConsole::~CUIConsole(void)
//	{
//		delete m_tb;
//		m_tb = nullptr;
//
//		delete m_lb;
//		m_lb = nullptr;
//	}
//
//	void CUIConsole::OnEnter(GwenBase* base)
//	{
//		if (!m_engine)
//		{
//			m_lb->AddItem("We have a nullptr to the engine. Cannot run command.");
//			return;
//		}
//		GwenTextBox* tb = static_cast<GwenTextBox*>(base);
//		if (tb->GetText().empty())
//			return;
//		
//		//try
//		//{
//		//	m_lb->AddItem(tb->GetText());
//		//	std::string code = Gwen::Utility::UnicodeToString(tb->GetText());
//		//	DoBuffer(m_engine->GetScriptEnv(), code.data(), code.size());
//		//	m_tb->SetText("");
//		//}
//		//catch(const luabind::error& e)
//		//{
//
//		//	const std::string str = lua_tostring(e.state(), -1);
//		//	lua_pop(e.state(), 1);
//		//	vShowMessage(str, __FILE__, __LINE__);
//		//	//m_engine->GetLogger().LogError(str);
//		//}
//		//catch(const std::exception& e)
//		//{
//		//	vShowMessage(e.what(), __FILE__, __LINE__);
//		//	//m_engine->GetLogger().LogError(e.what());
//		//}
//	}
//}