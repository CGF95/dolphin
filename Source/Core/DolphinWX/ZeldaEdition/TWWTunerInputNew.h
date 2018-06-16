#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/frame.h>

#include "Common/Common.h"
#include "Core/Core.h"
#include "Core/HW/Memmap.h"
#include "Common/IniFile.h"

#include "Common/StringUtil.h"
#include "Common/FileUtil.h"
#include "DiscIO/Filesystem.h"

#include "DiscIO/FileSystemGCWii.h"
#include "DiscIO/Volume.h"
#include "Core/ConfigManager.h"
#include "Core/Movie.h"

#include "DolphinWX/ISOProperties/ISOProperties.h"
#include "DiscIO/Enums.h"
#include "DolphinWX/WxUtils.h"
#include "Common/CommonTypes.h"
#include <string>




class TWWTunerInput : public wxFrame 
{
	private:
		struct Button
		{
			wxCheckBox* checkbox;
			int id;
		};

private:

	DECLARE_EVENT_TABLE();

	struct Button
	{
		wxCheckBox* checkbox;
		int id;
	};

	Button CreateButton(const std::string& name);

	Button m_dpad_up, m_dpad_down, m_dpad_left, m_dpad_right;
	Button* m_buttons[4];

	int m_eleID = 1005;

	protected:
		wxStaticText* m_staticText;
		wxChoice* m_tuneridchoice;
		wxButton* m_button_exec;
		wxStaticLine* m_staticline6;

		Button CreateButton(const std::string& name);

		Button m_dpad_up, m_dpad_down, m_dpad_left, m_dpad_right;
		Button* m_buttons[4];
		
		// Virtual event handlers, overide them in your derived class
		virtual void TWWTunerInputOnClose( wxCloseEvent& event ) { event.Skip(); }
		
	
	public:
		
		TWWTunerInput( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tuner Input by DB"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 423,435 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );
		
		~TWWTunerInput();
	
};

