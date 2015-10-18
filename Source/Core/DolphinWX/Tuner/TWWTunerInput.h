// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

//Dragonbane

#include <wx/defs.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/translation.h>
#include <wx/windowid.h>
#include <wx/artprov.h>
#include <wx/stattext.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>

#include "Common/CommonTypes.h"
#include <string>



class wxWindow;
class wxButton;
class wxFilePickerCtrl;
class wxCheckBox;

class TWWTunerInput : public wxDialog
{
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
  wxTextCtrl* m_text_ctrl;
  wxButton* m_button_start;
  wxChoice* m_choice_type;
  wxGridSizer* m_buttons_dpad;

public:

  TWWTunerInput(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tuner Input by DB"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP);
  void OnButtonPressed(wxCommandEvent& event);
  void OnCloseWindow(wxCloseEvent& event);
  void ResetValues();
  void UpdateButtons();
};
