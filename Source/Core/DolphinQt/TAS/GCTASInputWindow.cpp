// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinQt/TAS/GCTASInputWindow.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QVBoxLayout>

#include "Common/CommonTypes.h"
#include "Core/Movie.h"
#include "Core/HW/Memmap.h"
#include "Core/Core.h"
#include "InputCommon/GCPadStatus.h"

GCTASInputWindow::GCTASInputWindow(QWidget* parent, int num) : TASInputWindow(parent)
{
  setWindowTitle(tr("GameCube TAS Input %1").arg(num + 1));

  m_main_stick_box = CreateStickInputs(tr("Main Stick"), m_x_main_stick_value, m_y_main_stick_value,
                                       255, 255, Qt::Key_F, Qt::Key_G);
  m_c_stick_box = CreateStickInputs(tr("C Stick"), m_x_c_stick_value, m_y_c_stick_value, 255, 255,
                                    Qt::Key_H, Qt::Key_J);

  auto* top_layout = new QHBoxLayout;
  top_layout->addWidget(m_main_stick_box);
  top_layout->addWidget(m_c_stick_box);

  m_triggers_box = new QGroupBox(tr("Triggers"));

  auto* l_trigger_layout =
      CreateSliderValuePairLayout(tr("Left"), m_l_trigger_value, 255, Qt::Key_N, m_triggers_box);
  auto* r_trigger_layout =
      CreateSliderValuePairLayout(tr("Right"), m_r_trigger_value, 255, Qt::Key_M, m_triggers_box);

  auto* triggers_layout = new QVBoxLayout;
  triggers_layout->addLayout(l_trigger_layout);
  triggers_layout->addLayout(r_trigger_layout);
  m_triggers_box->setLayout(triggers_layout);

  m_buttons[0] = &m_a_button;
  m_buttons[1] = &m_b_button;
  m_buttons[2] = &m_x_button;
  m_buttons[3] = &m_y_button;
  m_buttons[4] = &m_z_button;
  m_buttons[5] = &m_l_button;
  m_buttons[6] = &m_r_button;
  m_buttons[7] = &m_start_button;
  m_buttons[8] = &m_left_button;
  m_buttons[9] = &m_up_button;
  m_buttons[10] = &m_down_button;
  m_buttons[11] = &m_right_button;
  m_buttons[12] = &m_skipDialog;
  m_buttons[13] = &m_analog_lr;
  m_buttons[14] = &m_analog_ud;
  m_buttons[15] = &m_rollassist;

  m_buttons[0]->checkbox = new QCheckBox(QStringLiteral("&A"));
  m_buttons[1]->checkbox = new QCheckBox(QStringLiteral("&B"));
  m_buttons[2]->checkbox = new QCheckBox(QStringLiteral("&X"));
  m_buttons[3]->checkbox = new QCheckBox(QStringLiteral("&Y"));
  m_buttons[4]->checkbox = new QCheckBox(QStringLiteral("&Z"));
  m_buttons[5]->checkbox = new QCheckBox(QStringLiteral("&L"));
  m_buttons[6]->checkbox = new QCheckBox(QStringLiteral("&R"));
  m_buttons[7]->checkbox = new QCheckBox(QStringLiteral("&START"));
  m_buttons[8]->checkbox = new QCheckBox(QStringLiteral("L&eft"));
  m_buttons[9]->checkbox = new QCheckBox(QStringLiteral("&Up"));
  m_buttons[10]->checkbox = new QCheckBox(QStringLiteral("&Down"));
  m_buttons[11]->checkbox = new QCheckBox(QStringLiteral("R&ight"));
  m_buttons[12]->checkbox = new QCheckBox(QStringLiteral("&Auto Dialog"));
  m_buttons[13]->checkbox = new QCheckBox(QStringLiteral("&Swim X"));
  m_buttons[14]->checkbox = new QCheckBox(QStringLiteral("&Swim Y"));
  m_buttons[15]->checkbox = new QCheckBox(QStringLiteral("&Roll Assist"));

  auto* buttons_layout1 = new QHBoxLayout;
  buttons_layout1->addWidget(m_buttons[0]->checkbox);
  buttons_layout1->addWidget(m_buttons[1]->checkbox);
  buttons_layout1->addWidget(m_buttons[2]->checkbox);
  buttons_layout1->addWidget(m_buttons[3]->checkbox);
  buttons_layout1->addWidget(m_buttons[4]->checkbox);
  buttons_layout1->addWidget(m_buttons[5]->checkbox);
  buttons_layout1->addWidget(m_buttons[6]->checkbox);

  auto* buttons_layout2 = new QHBoxLayout;
  buttons_layout2->addWidget(m_buttons[7]->checkbox);
  buttons_layout2->addWidget(m_buttons[8]->checkbox);
  buttons_layout2->addWidget(m_buttons[9]->checkbox);
  buttons_layout2->addWidget(m_buttons[10]->checkbox);
  buttons_layout2->addWidget(m_buttons[11]->checkbox);

  auto* buttons_layout3 = new QHBoxLayout;
  buttons_layout3->addWidget(m_buttons[12]->checkbox);
  buttons_layout3->addWidget(m_buttons[13]->checkbox);
  buttons_layout3->addWidget(m_buttons[14]->checkbox);
  buttons_layout3->addWidget(m_buttons[15]->checkbox);

  auto* buttons_layout = new QVBoxLayout;
  buttons_layout->setSizeConstraint(QLayout::SetFixedSize);
  buttons_layout->addLayout(buttons_layout1);
  buttons_layout->addLayout(buttons_layout2);
  buttons_layout->addLayout(buttons_layout3);

  m_buttons_box = new QGroupBox(tr("Buttons"));
  m_buttons_box->setLayout(buttons_layout);

  auto* layout = new QVBoxLayout;
  layout->addLayout(top_layout);
  layout->addWidget(m_triggers_box);
  layout->addWidget(m_buttons_box);
  layout->addWidget(m_use_controller);
  setLayout(layout);
}

void GCTASInputWindow::GetValues(GCPadStatus* pad)
{
  if (!isVisible())
    return;

  // Dragonbane: Execute custom Button functions
  ExecuteHelpers(pad);

  GetButton<u16>(m_buttons[0]->checkbox, pad->button, PAD_BUTTON_A);
  GetButton<u16>(m_buttons[1]->checkbox, pad->button, PAD_BUTTON_B);
  GetButton<u16>(m_buttons[2]->checkbox, pad->button, PAD_BUTTON_X);
  GetButton<u16>(m_buttons[3]->checkbox, pad->button, PAD_BUTTON_Y);
  GetButton<u16>(m_buttons[4]->checkbox, pad->button, PAD_TRIGGER_Z);
  GetButton<u16>(m_buttons[5]->checkbox, pad->button, PAD_TRIGGER_L);
  GetButton<u16>(m_buttons[6]->checkbox, pad->button, PAD_TRIGGER_R);
  GetButton<u16>(m_buttons[7]->checkbox, pad->button, PAD_BUTTON_START);
  GetButton<u16>(m_buttons[8]->checkbox, pad->button, PAD_BUTTON_LEFT);
  GetButton<u16>(m_buttons[9]->checkbox, pad->button, PAD_BUTTON_UP);
  GetButton<u16>(m_buttons[10]->checkbox, pad->button, PAD_BUTTON_DOWN);
  GetButton<u16>(m_buttons[11]->checkbox, pad->button, PAD_BUTTON_RIGHT);

  if (m_buttons[0]->checkbox->isChecked())
    pad->analogA = 0xFF;
  else
    pad->analogA = 0x00;

  if (m_buttons[1]->checkbox->isChecked())
    pad->analogB = 0xFF;
  else
    pad->analogB = 0x00;

  if (m_buttons[5]->checkbox->isChecked())
    pad->triggerLeft = 0xFF;
  else
    pad->triggerLeft = 0x00;

  if (m_buttons[6]->checkbox->isChecked())
    pad->triggerRight = 0xFF;
  else
    pad->triggerRight = 0x00;

  GetSpinBoxU8(m_l_trigger_value, pad->triggerLeft);
  GetSpinBoxU8(m_r_trigger_value, pad->triggerRight);

  GetSpinBoxU8(m_x_main_stick_value, pad->stickX);
  GetSpinBoxU8(m_y_main_stick_value, pad->stickY);

  GetSpinBoxU8(m_x_c_stick_value, pad->substickX);
  GetSpinBoxU8(m_y_c_stick_value, pad->substickY);
}


//TODO: Finish
void GCTASInputWindow::ExecuteHelpers(GCPadStatus* pad)
{
  if (!Core::IsRunningAndStarted())
    return;

  u32 charPointerAdd;
  u32 isLoadingAdd;

  isLoadingAdd = 0x3ad335;
  charPointerAdd = 0x3ad860;

	// Auto Dialog
  if (m_skipDialog.checkbox->isChecked())
  {
    if (!auto_dialog)
    {
      dialog_timer = Movie::GetCurrentFrame();
      auto_dialog = true;
    }

    if (dialog_timer + 2 == Movie::GetCurrentFrame())
    {
      dialog_timer = Movie::GetCurrentFrame();
    }

    if (dialog_timer + 1 == Movie::GetCurrentFrame())
    {
      m_a_button.checkbox->setChecked(true);
      m_b_button.checkbox->setChecked(false);
    }

    if (dialog_timer == Movie::GetCurrentFrame())
    {
      m_a_button.checkbox->setChecked(false);
      m_b_button.checkbox->setChecked(true);
    }
  }

  if (m_analog_lr.checkbox->isChecked())
  {
    if (!auto_analog_lr)
    {
      analog_timer_lr = Movie::GetCurrentFrame();
      auto_analog_lr = true;
    }

    if (analog_timer_lr + 2 == Movie::GetCurrentFrame())
    {
      analog_timer_lr = Movie::GetCurrentFrame();
    }

    if (analog_timer_lr + 1 == Movie::GetCurrentFrame())
    {
      pad->stickX = 0xFF;
    }

    if (analog_timer_lr == Movie::GetCurrentFrame())
    {
      pad->stickX = 0x00;
    }
  }
  else
  {
    auto_analog_lr = false;
    pad->stickX = 0x7f;
  }

  if (m_analog_ud.checkbox->isChecked())
  {
    if (!auto_analog_ud)
    {
      analog_timer_ud = Movie::GetCurrentFrame();
      auto_analog_ud = true;
    }

    if (analog_timer_ud + 2 == Movie::GetCurrentFrame())
    {
      analog_timer_ud = Movie::GetCurrentFrame();
    }

    if (analog_timer_ud + 1 == Movie::GetCurrentFrame())
    {
      pad->stickY = 0xFF;
    }

    if (analog_timer_lr == Movie::GetCurrentFrame())
    {
      pad->stickY = 0x00;
    }
  }
  else
  {
    auto_analog_ud = false;
    pad->stickY = 0x7f;
  }

  if (m_rollassist.checkbox->isChecked())
  {
    Movie::roll_enabled = false;
  }
  if (Movie::roll_enabled)
  {
    u32 characterpointer = Memory::Read_U32(charPointerAdd);
    if (characterpointer > 0x80000000)
    {
      characterpointer -= 0x80000000;

      float LinkSpeed = Memory::Read_F32(characterpointer + 0x34e4);
      u8 isLoading = Memory::Read_U8(isLoadingAdd);

      if (isLoading > 0 || pad->stickX == 128 && pad->stickY == 128 ||
          LinkSpeed < 13.590374)  // Reset rolling during loading or idle or not enough speed
      {
        Movie::first_roll = false;
        Movie::roll_timer = Movie::GetCurrentFrame() + 1;
        m_a_button.checkbox->setChecked(false);
      }
      else
      {
        if (Movie::GetCurrentFrame() == Movie::roll_timer)
        {
          m_a_button.checkbox->setChecked(true);
        }
        else if (Movie::GetCurrentFrame() == Movie::roll_timer + 1)
        {
          m_a_button.checkbox->setChecked(false);
          Movie::roll_timer = Movie::GetCurrentFrame() + 15;
        }
      }
    }
  }
  else
  {
    Movie::first_roll = false;
    Movie::roll_timer = Movie::GetCurrentFrame() + 1;
    m_a_button.checkbox->setChecked(false);
  }
}
