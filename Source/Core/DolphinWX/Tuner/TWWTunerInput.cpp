// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <wx/bitmap.h>
#include <wx/defs.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/translation.h>
#include <wx/windowid.h>
#include <wx/msgdlg.h>
#include <array>
#include <wx/filepicker.h>
#include <wx/utils.h>

#include "Common/Common.h"
#include "TWWTunerInput.h"
#include "Core/Core.h"
#include "Core/HW/Memmap.h"
#include "Common/IniFile.h"

#include "Common/CommonTypes.h"
#include "Common/StringUtil.h"
#include "Common/FileUtil.h"
#include "DiscIO/Filesystem.h"

#include "DiscIO/FileSystemGCWii.h"
#include "DiscIO/Volume.h"
#include "Core/ConfigManager.h"
#include "Core/Movie.h"


#include "DolphinWX/ISOProperties/ISOProperties.h"
#include "DolphinWX/WxUtils.h"

//Dragonbane
BEGIN_EVENT_TABLE(TWWTunerInput, wxDialog)

EVT_BUTTON(1, TWWTunerInput::OnButtonPressed)

END_EVENT_TABLE()


TWWTunerInput::TWWTunerInput(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	//Headline
	m_staticText = new wxStaticText(this, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText->Wrap(-1);
	m_staticText->SetFont(wxFont(12, 74, 90, 92));
	bSizer1->Add(m_staticText, 0, wxALL, 5);

	//Action Type Choice
	wxString m_choice_typeChoices[] = {
		wxT("Tingle Bomb"),
		wxT("Tingle Balloon"),
		wxT("Tingle Shield"),
		wxT("Kooloo-Limpah"),
		wxT("Red Ting"),
		wxT("Green Ting"),
		wxT("Blue Ting"),
		wxT("Heyyy Tingle"),
		wxT("Reset Cursor")
	};
	int m_choice_typeNChoices = sizeof(m_choice_typeChoices) / sizeof(wxString);
	m_choice_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_typeNChoices, m_choice_typeChoices, 0);
	bSizer1->Add(m_choice_type, 0, wxALL, 5);

	//Execute Button
	bSizer1->AddSpacer(10);

	m_button_start = new wxButton(this, 1, "Execute Action", wxDefaultPosition, wxDefaultSize);
	m_button_start->Enable();

	bSizer1->Add(m_button_start, 1, wxALIGN_CENTER, 5);

	bSizer1->AddSpacer(10);


	//GBA D-Pad
	for (unsigned int i = 0; i < ArraySize(m_buttons); ++i)
		m_buttons[i] = nullptr;

	m_buttons[0] = &m_dpad_down;
	m_buttons[1] = &m_dpad_up;
	m_buttons[2] = &m_dpad_left;
	m_buttons[3] = &m_dpad_right;

	m_dpad_up = CreateButton("Up");
	m_dpad_right = CreateButton("Right");
	m_dpad_down = CreateButton("Down");
	m_dpad_left = CreateButton("Left");

	m_buttons_dpad = new wxGridSizer(3);
	m_buttons_dpad->AddSpacer(20);
	m_buttons_dpad->Add(m_dpad_up.checkbox);
	m_buttons_dpad->AddSpacer(20);
	m_buttons_dpad->Add(m_dpad_left.checkbox);
	m_buttons_dpad->AddSpacer(20);
	m_buttons_dpad->Add(m_dpad_right.checkbox);
	m_buttons_dpad->AddSpacer(20);
	m_buttons_dpad->Add(m_dpad_down.checkbox);
	m_buttons_dpad->AddSpacer(20);

	wxStaticBoxSizer* const m_buttons_box = new wxStaticBoxSizer(wxVERTICAL, this, _("D-Pad"));

	m_buttons_box->Add(m_buttons_dpad);

	bSizer1->Add(m_buttons_box, 0, wxBOTTOM, 5);

	SetSizer(bSizer1);
	Layout();

	bSizer1->Fit(this);

	Centre(wxBOTH);

	ResetValues();

	Bind(wxEVT_CLOSE_WINDOW, &TWWTunerInput::OnCloseWindow, this);
}



TWWTunerInput::Button TWWTunerInput::CreateButton(const std::string& name)
{
	Button temp;
	wxCheckBox* checkbox = new wxCheckBox(this, m_eleID++, name);
	temp.checkbox = checkbox;
	temp.id = m_eleID - 1;
	return temp;
}

void TWWTunerInput::ResetValues()
{
	for (Button* const button : m_buttons)
	{
		if (button != nullptr)
		{
			button->checkbox->SetValue(false);
		}
	}
}

void TWWTunerInput::UpdateButtons()
{
	if (!IsShown())
		return;

	if (!Core::IsRunningAndStarted())
		return;

	std::string gameID = SConfig::GetInstance().GetGameID();

	if (gameID.compare("GZLJ01") && gameID.compare("GZLE01") && gameID.compare("GZLP01"))
		return;

	if (Movie::IsPlayingInput())
		return;

	if (Movie::tunerActionID > 0 || Movie::tunerExecuteID > 0)
		return;

	int actionID = 0;

	//Single Buttons
	if (m_dpad_up.checkbox->IsChecked())
	{
		actionID = 1;
	}
	else if (m_dpad_right.checkbox->IsChecked())
	{
		actionID = 3;
	}
	else if (m_dpad_down.checkbox->IsChecked())
	{
		actionID = 5;
	}
	else if (m_dpad_left.checkbox->IsChecked())
	{
		actionID = 7;
	}

	//Directionals
	if (m_dpad_up.checkbox->IsChecked() && m_dpad_right.checkbox->IsChecked())
	{
		actionID = 2;
	}
	else if (m_dpad_right.checkbox->IsChecked() && m_dpad_down.checkbox->IsChecked())
	{
		actionID = 4;
	}
	else if (m_dpad_down.checkbox->IsChecked() && m_dpad_left.checkbox->IsChecked())
	{
		actionID = 6;
	}
	else if (m_dpad_left.checkbox->IsChecked() && m_dpad_up.checkbox->IsChecked())
	{
		actionID = 8;
	}


	if (Movie::IsRecordingInput())
	{
		Movie::tunerActionID = actionID;
	}
	else
	{
		Movie::tunerExecuteID = actionID;
	}
}

void TWWTunerInput::OnButtonPressed(wxCommandEvent& event)
{
	if (event.GetId() == 1) //Action Button Pressed
	{
		if (!Core::IsRunningAndStarted())
		{
			wxMessageBox("Please start the game before trying to execute events!");
			return;
		}

		std::string gameID = SConfig::GetInstance().GetGameID();

		if (gameID.compare("GZLJ01") && gameID.compare("GZLE01") && gameID.compare("GZLP01"))
		{
			wxMessageBox("This is not The Wind Waker!");
			return;
		}

		if (Movie::IsPlayingInput())
		{
			wxMessageBox("Not available, movie is in Playback!");
			return;
		}

		if (Movie::tunerActionID > 0 || Movie::tunerExecuteID > 0)
		{
			wxMessageBox("Action still running! Please retry later");
			return;
		}

		if (m_choice_type->GetStringSelection() == wxEmptyString)
		{
			wxMessageBox("No Action selected!");
			return;
		}

		/*std::string idStr = WxStrToStr(m_text_ctrl->GetValue());

		if (idStr.empty() || !idStr.compare("?"))
		{
		wxMessageBox("1-8: Cursor movement clockwise\n  9: Reset Cursor\n10: Bomb\n11: Balloon\n12: Shield\n13: Kooloo-Limpah\n14: Red Ting\n15: Green Ting\n16: Blue Ting\n17: Hey Tingle\n18: Activate fake GBA\n19: Deactivate fake GBA");
		return;
		}

		if (idTuner < 1 || idTuner > 19)
		{
		wxMessageBox("Minimum ID is 1 and maximum ID is 19!");
		return;
		}

		int idTuner = 0;
		idTuner = atoi(idStr.c_str());
		*/

		int idTuner = 0;

		if (m_choice_type->GetSelection() == 0)
			idTuner = 10;
		else if (m_choice_type->GetSelection() == 1)
			idTuner = 11;
		else if (m_choice_type->GetSelection() == 2)
			idTuner = 12;
		else if (m_choice_type->GetSelection() == 3)
			idTuner = 13;
		else if (m_choice_type->GetSelection() == 4)
			idTuner = 14;
		else if (m_choice_type->GetSelection() == 5)
			idTuner = 15;
		else if (m_choice_type->GetSelection() == 6)
			idTuner = 16;
		else if (m_choice_type->GetSelection() == 7)
			idTuner = 17;
		else if (m_choice_type->GetSelection() == 8)
			idTuner = 9;


		if (Movie::IsRecordingInput())
		{
			Movie::tunerActionID = idTuner;
		}
		else
		{
			Movie::tunerExecuteID = idTuner;
		}
	}
}

void TWWTunerInput::OnCloseWindow(wxCloseEvent& event)
{
	if (event.CanVeto())
	{
		event.Skip(false);
		Show(false);
	}
}