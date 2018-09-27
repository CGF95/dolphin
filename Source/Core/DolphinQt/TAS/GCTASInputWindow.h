// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "DolphinQt/TAS/TASInputWindow.h"

class QCheckBox;
class QGroupBox;
class QSpinBox;
struct GCPadStatus;

class GCTASInputWindow : public TASInputWindow
{
  Q_OBJECT
public:
  explicit GCTASInputWindow(QWidget* parent, int num);
  void GetValues(GCPadStatus* pad);

  void ExecuteHelpers(GCPadStatus* pad);

private:

  // Dragonbane
  int quickspin_timer = 0;
  bool quickspin_enabled = false;

  int dialog_timer = 0;
  bool auto_dialog = false;

  int analog_timer_ud = 0;
  bool auto_analog_ud = false;

  int analog_timer_lr = 0;
  bool auto_analog_lr = false;

  struct Button //TODO
  { 
    QCheckBox* checkbox;
    bool turbo_on = false;
    bool value = false;
  };

  Button* m_buttons[16];
  Button m_a_button, m_b_button, m_x_button, m_y_button, m_z_button, m_l_button, m_r_button;
  Button m_start_button, m_left_button, m_up_button, m_down_button, m_right_button, m_analog_lr, m_analog_ud;
  Button m_skipDialog, m_rollassist;
  QSpinBox* m_l_trigger_value;
  QSpinBox* m_r_trigger_value;
  QSpinBox* m_x_main_stick_value;
  QSpinBox* m_y_main_stick_value;
  QSpinBox* m_x_c_stick_value;
  QSpinBox* m_y_c_stick_value;
  QGroupBox* m_main_stick_box;
  QGroupBox* m_c_stick_box;
  QGroupBox* m_triggers_box;
  QGroupBox* m_buttons_box;
};
