// A LOT of code based on Dragonbane's code. I only ported it to the Qt Version with a few added
// features. -CGF95

#include "TunerInput.h"
#include <QPushButton>
#include <qtimer.h>

#include "DiscIO/Enums.h"

TunerInput::TunerInput(QWidget* parent)
{
  // Setup the new UI
  setupUi(this);

  QTimer* timer = new QTimer(m_buttons);
  timer->setInterval(1);
  connect(timer, &QTimer::timeout, this, &TunerInput::UpdateButtons);

  connect(m_dpad_up, &QCheckBox::toggled, timer, [timer](bool checked) {
    if (checked)
      timer->start();
    else
      timer->stop();
  });

  connect(m_dpad_left, &QCheckBox::toggled, timer, [timer](bool checked) {
    if (checked)
      timer->start();
    else
      timer->stop();
  });

  connect(m_dpad_right, &QCheckBox::toggled, timer, [timer](bool checked) {
    if (checked)
      timer->start();
    else
      timer->stop();
  });

  connect(m_dpad_down, &QCheckBox::toggled, timer, [timer](bool checked) {
    if (checked)
      timer->start();
    else
      timer->stop();
  });

  QObject::connect(ResetCursor, SIGNAL(clicked()), this, SLOT(ResetCursorPressed()));
  QObject::connect(ExecuteEvents, SIGNAL(clicked()), this, SLOT(OnExecuteEvents()));
}

void TunerInput::OnExecuteEvents()
{
  QMessageBox msgBox;
  if (isEnabled())
  {
    if (!Core::IsRunningAndStarted())
    {
      msgBox.setWindowTitle(QStringLiteral("Warning"));
      msgBox.setText(QStringLiteral("Warning: Game has not started!"));
      msgBox.exec();
    }

    std::string gameID = SConfig::GetInstance().GetGameID();

    if (gameID.compare("GZLJ01") && gameID.compare("GZLE01") && gameID.compare("GZLP01"))
    {
      msgBox.setWindowTitle(QStringLiteral("Warning"));
      msgBox.setText(QStringLiteral("This is not The Wind Waker!"));
      msgBox.exec();
    }

    if (Movie::IsPlayingInput())
    {
      msgBox.setWindowTitle(QStringLiteral("Warning"));
      msgBox.setText(QStringLiteral("Movie is in playback mode, please switch modes!"));
      msgBox.exec();
    }

    if (Movie::tunerActionID > 0 || Movie::tunerExecuteID > 0)
    {
      msgBox.setWindowTitle(QStringLiteral("Warning"));
      msgBox.setText(QStringLiteral("Action still running, try again later!"));
      msgBox.exec();
    }

    int idTuner = 0;

    if (TunerChoices->currentIndex() == 0)
      idTuner = 10;
    else if (TunerChoices->currentIndex() == 1)
      idTuner = 11;
    else if (TunerChoices->currentIndex() == 2)
      idTuner = 12;
    else if (TunerChoices->currentIndex() == 3)
      idTuner = 13;
    else if (TunerChoices->currentIndex() == 4)
      idTuner = 14;
    else if (TunerChoices->currentIndex() == 5)
      idTuner = 15;
    else if (TunerChoices->currentIndex() == 6)
      idTuner = 16;
    else if (TunerChoices->currentIndex() == 7)
      idTuner = 17;
    else
    {
      idTuner = 0;
    }

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

void TunerInput::ResetCursorPressed()
{
  int idTuner = 0;
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
