#pragma once

#include "DolphinQt/TunerWindow.h"

#include <QDialog>

#include "Common/CommonTypes.h"

class QCheckBox;
class QGroupBox;
class QSpinBox;

class GCTingleTunerInput : public TunerWindow
{
  Q_OBJECT
public:
  explicit GCTingleTunerInput(QWidget* parent, int num);
	
	//void OnButtonPressed(QActionEvent& event);
  void ResetValues();
	void UpdateButtons();

private:
	QCheckBox* m_buttons[4];

	QCheckBox* m_left_button;
  QCheckBox* m_up_button;
  QCheckBox* m_down_button;
  QCheckBox* m_right_button;
	QGroupBox* m_buttons_box;
	QPushButton* m_tuner_action;
};
