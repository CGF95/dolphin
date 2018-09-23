#include <QCommandLinkButton>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include "Common/Common.h"
#include "Common/IniFile.h"
#include "Core/Core.h"
#include "Core/HW/Memmap.h"

#include "Common/FileUtil.h"
#include "Common/StringUtil.h"
#include "DiscIO/Filesystem.h"

#include "Core/ConfigManager.h"
#include "Core/Movie.h"
#include "DiscIO/FileSystemGCWii.h"
#include "DiscIO/Volume.h"

#include "Common/CommonTypes.h"

class QCheckBox;
class QPushButton;
class QWindow;
class QWidget;
class QString;
class QObject;

class TunerInput : public QWidget
{
  Q_OBJECT
public:
  QPushButton* ExecuteEvents;
  QGroupBox* groupBox;
  QComboBox* TunerChoices;
  QWidget* widget;
  QGridLayout* gridLayout;
  QPushButton* ResetCursor;
  QCheckBox* m_dpad_left;
  QCheckBox* m_dpad_right;
  QCheckBox* m_dpad_up;
  QCheckBox* m_dpad_down;
  QButtonGroup* m_buttons;

  explicit TunerInput(QWidget* parent = nullptr);

  void setupUi(QWidget* TunerInputDialog)
  {
    if (TunerInputDialog->objectName().isEmpty())
      TunerInputDialog->setObjectName(QStringLiteral("Dialog"));
    TunerInputDialog->resize(386, 122);
    ExecuteEvents = new QPushButton(TunerInputDialog);
    ExecuteEvents->setObjectName(QStringLiteral("ExecuteEvents"));
    ExecuteEvents->setGeometry(QRect(20, 50, 100, 21));
    groupBox = new QGroupBox(TunerInputDialog);
    groupBox->setObjectName(QStringLiteral("groupBox"));
    groupBox->setGeometry(QRect(0, 0, 381, 123));
    TunerChoices = new QComboBox(groupBox);
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->addItem(QString());
    TunerChoices->setObjectName(QStringLiteral("TunerChoices"));
    TunerChoices->setGeometry(QRect(20, 20, 101, 21));
    widget = new QWidget(groupBox);
    widget->setObjectName(QStringLiteral("widget"));
    widget->setGeometry(QRect(130, 20, 251, 90));
    gridLayout = new QGridLayout(widget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    m_buttons = new QButtonGroup(TunerInputDialog);
    m_buttons->setObjectName(QStringLiteral("m_buttons"));
    m_buttons->setExclusive(false);
    ResetCursor = new QPushButton(widget);
    ResetCursor->setObjectName(QStringLiteral("ResetCursor"));

    gridLayout->addWidget(ResetCursor, 1, 1, 1, 1);

    m_dpad_left = new QCheckBox(widget);
    m_buttons->addButton(m_dpad_left);
    m_dpad_left->setObjectName(QStringLiteral("m_dpad_left"));

    gridLayout->addWidget(m_dpad_left, 1, 0, 1, 1);

    m_dpad_right = new QCheckBox(widget);
    m_buttons->addButton(m_dpad_right);
    m_dpad_right->setObjectName(QStringLiteral("m_dpad_right"));

    gridLayout->addWidget(m_dpad_right, 1, 2, 1, 1);

    m_dpad_up = new QCheckBox(widget);
    m_buttons->addButton(m_dpad_up);
    m_dpad_up->setObjectName(QStringLiteral("m_dpad_up"));

    gridLayout->addWidget(m_dpad_up, 0, 1, 1, 1);

    m_dpad_down = new QCheckBox(widget);
    m_buttons->addButton(m_dpad_down);
    m_dpad_down->setObjectName(QStringLiteral("m_dpad_down"));
    gridLayout->addWidget(m_dpad_down, 2, 1, 1, 1);

    groupBox->raise();
    m_dpad_right->raise();
    ResetCursor->raise();
    m_dpad_up->raise();
    m_dpad_down->raise();
    m_dpad_left->raise();
    ExecuteEvents->raise();

    retranslateUi(TunerInputDialog);
  }  // setupUi

  void retranslateUi(QWidget* Dialog)
  {
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Tingle Tuner Input", nullptr));
    ExecuteEvents->setText(QApplication::translate("Dialog", "Execute", nullptr));
    groupBox->setTitle(QApplication::translate(
        "Dialog", "Tuner Input by Dragonbane, Qt Version by CGF95", nullptr));
    TunerChoices->setItemText(0, QApplication::translate("Dialog", "Tingle Bomb", nullptr));
    TunerChoices->setItemText(1, QApplication::translate("Dialog", "Tingle Balloon", nullptr));
    TunerChoices->setItemText(2, QApplication::translate("Dialog", "Tingle Shield", nullptr));
    TunerChoices->setItemText(3, QApplication::translate("Dialog", "Kooloo-Limpah", nullptr));
    TunerChoices->setItemText(4, QApplication::translate("Dialog", "Red Ting", nullptr));
    TunerChoices->setItemText(5, QApplication::translate("Dialog", "Green Ting", nullptr));
    TunerChoices->setItemText(6, QApplication::translate("Dialog", "Blue Ting", nullptr));
    TunerChoices->setItemText(7, QApplication::translate("Dialog", "Tingle: Heyyy!", nullptr));

    ResetCursor->setText(QApplication::translate("Dialog", "Reset Cursor", nullptr));
    m_dpad_left->setText(QApplication::translate("Dialog", "Left", nullptr));
    m_dpad_right->setText(QApplication::translate("Dialog", "Right", nullptr));
    m_dpad_up->setText(QApplication::translate("Dialog", "Up", nullptr));
    m_dpad_down->setText(QApplication::translate("Dialog", "Down", nullptr));
    widget->setWindowTitle(QApplication::translate("Widget", "Tingle Tuner Input", nullptr));
  }  // retranslateUi
private slots:
  void ResetCursorPressed();
  void OnExecuteEvents();

  void UpdateButtons()
  {
    if (!isEnabled())
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

    // Single Buttons
    if (m_dpad_up->isChecked())
    {
      actionID = 1;
    }
    else if (m_dpad_right->isChecked())
    {
      actionID = 3;
    }
    else if (m_dpad_down->isChecked())
    {
      actionID = 5;
    }
    else if (m_dpad_left->isChecked())
    {
      actionID = 7;
    }

    // Directionals
    if (m_dpad_up->isChecked() && m_dpad_right->isChecked())
    {
      actionID = 2;
    }
    else if (m_dpad_right->isChecked() && m_dpad_down->isChecked())
    {
      actionID = 4;
    }
    else if (m_dpad_down->isChecked() && m_dpad_left->isChecked())
    {
      actionID = 6;
    }
    else if (m_dpad_left->isChecked() && m_dpad_up->isChecked())
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
};
