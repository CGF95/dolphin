#include "QtTunerInput.h"

QtTunerInput::QtTunerInput(QWidget* parent)
{
  int x = dw.width()*0.7;
  int y = dw.height()*0.7;
  w.setFixedSize(x, y);

  QBoxLayout* bSizer1;
  bSizer1 = new QBoxLayout(QBoxLayout::Up);

  //Headline
  m_staticText = new QStaticText(QString& "Text")

}

QtTunerInput::~QtTunerInput()
{
}

void QtTunerInput::OnButtonPressed(QCommandLinkButton & event)
{
}

void QtTunerInput::OnCloseWindow(QCloseEvent & event)
{
}

void QtTunerInput::ResetValues()
{
}

void QtTunerInput::UpdateButtons()
{
}
