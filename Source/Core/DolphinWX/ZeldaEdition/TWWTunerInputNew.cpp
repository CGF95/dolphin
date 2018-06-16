#include "TWWTunerInputNew.h"

TWWTunerInput::TWWTunerInput( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticText = new wxStaticText( this, wxID_ANY, wxT("Tuner Actions:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText->Wrap( -1 );
	bSizer->Add( m_staticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxArrayString m_tuneridchoiceChoices;
	m_tuneridchoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_tuneridchoiceChoices, wxCB_SORT );
	m_tuneridchoice->SetSelection( 0 );
	bSizer->Add( m_tuneridchoice, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_button_exec = new wxButton( this, wxID_ANY, wxT("Execute Action"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer->Add( m_button_exec, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer->Add( m_staticline6, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer;
	sbSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("GBA Pad") ), wxVERTICAL );
	
	wxGridSizer* gSizer;
	gSizer = new wxGridSizer( 3, 0, 0, 0 );
	
	
	gSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_dpad_up = new wxCheckBox( sbSizer->GetStaticBox(), wxID_ANY, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer->Add( m_dpad_up, 0, wxALL, 5 );
	
	
	gSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_dpad_left = new wxCheckBox( sbSizer->GetStaticBox(), wxID_ANY, wxT("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer->Add( m_dpad_left, 0, wxALL, 5 );
	
	
	gSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_dpad_right = new wxCheckBox( sbSizer->GetStaticBox(), wxID_ANY, wxT("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer->Add( m_dpad_right, 0, wxALL, 5 );
	
	
	gSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_dpad_down = new wxCheckBox( sbSizer->GetStaticBox(), wxID_ANY, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer->Add( m_dpad_down, 0, wxALL, 5 );
	
	
	sbSizer->Add( gSizer, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	
	bSizer->Add( sbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TWWTunerInput::TWWTunerInputOnClose ) );
}

TWWTunerInput::~TWWTunerInput()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TWWTunerInput::TWWTunerInputOnClose ) );
	
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
			wxMessageBox("This is not Wind Waker!");
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
