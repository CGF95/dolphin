#include "DolphinQt/TAS/GCTingleTunerInput.h"

#include <QVariant>
#include <QApplication>
#include <QCheckBox.h>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QWidget>
#include <QDialog>

#include "Common/CommonTypes.h"
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

#include "DiscIO/Enums.h"

//Start of Qt Tuner Input Code
GCTingleTunerInput::GCTingleTunerInput(QWidget* parent, int num) : TunerWindow(parent)
{
	setWindowTitle(tr("The Wind Waker Tuner Input"));

	m_up_button = new QCheckBox(QStringLiteral("&Up"));
	m_left_button = new QCheckBox(QStringLiteral("L&eft"));
	m_down_button = new QCheckBox(QStringLiteral("&Down"));
	m_right_button = new QCheckBox(QStringLiteral("R&ight"));
	m_tuner_action = new QPushButton(QStringLiteral("Execute Action!"));

	auto* buttons_layout2 = new QHBoxLayout;
	buttons_layout2->addWidget(m_left_button);
	buttons_layout2->addWidget(m_up_button);
	buttons_layout2->addWidget(m_down_button);
	buttons_layout2->addWidget(m_right_button);
  buttons_layout2->addWidget(m_tuner_action);

	auto* buttons_layout = new QVBoxLayout;
	buttons_layout->setSizeConstraint(QLayout::SetFixedSize);
	buttons_layout->addLayout(buttons_layout2);

	m_buttons_box = new QGroupBox(tr("GBA Controls"));
}

void GCTingleTunerInput::ResetValues()
{
  for (QCheckBox* const button : m_buttons)
  {
    if (button != nullptr)
    {
      button->setChecked(false);
    }
  }
}

void GCTingleTunerInput::UpdateButtons()
{

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
	if (m_up_button->isChecked())
	{
		actionID = 1;
	}
	else if (m_right_button->isChecked())
	{
		actionID = 3;
	}
	else if (m_down_button->isChecked())
	{
		actionID = 5;
	}
	else if (m_left_button->isChecked())
	{
		actionID = 7;
	}

	//Directionals
	if (m_up_button->isChecked() && m_right_button->isChecked())
	{
		actionID = 2;
	}
	else if (m_right_button->isChecked() && m_down_button->isChecked())
	{
		actionID = 4;
	}
	else if (m_down_button->isChecked() && m_left_button->isChecked())
	{
		actionID = 6;
	}
	else if (m_left_button->isChecked() && m_up_button->isChecked())
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
