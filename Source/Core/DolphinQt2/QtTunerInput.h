#pragma once
//CGF
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckbox>
#include <QDialog>
#include <QString>
#include <QStaticText>
#include <QFont>
#include <QColor>
#include <QInputDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QMainWindow>
#include <qcommandlinkbutton>

//Dragonbane
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

class QWindow;
class QButton;
class QCheckBox;

class QtTunerInput : public QWidget
{
  Q_OBJECT;
private:
  struct Button
  {
    QCheckBox* checkbox;
    int id;
  };

  QDesktopWidget dw;
  QMainWindow w;
  QString* "Text";
  QStaticText* m_staticText;
  Button CreateButton(const std::string& name);
  Button m_dpad_up, m_dpad_down, m_dpad_left, m_dpad_right;
  Button* m_buttons[4];
  int m_eleID = 1005;

protected:
  QLineEdit* m_text_ctrl;
  QButton* m_button_start;
  QComboBox* m_choice_type;
  QBoxLayout* m_buttons_dpad;

public:
  QtTunerInput(QWidget* parent);
  ~QtTunerInput();
  void OnButtonPressed(QCommandLinkButton& event);
  void OnCloseWindow(QCloseEvent& event);
  void ResetValues();
  void UpdateButtons();
};

