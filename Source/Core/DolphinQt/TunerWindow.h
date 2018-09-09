// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <QDialog>

#include "Common/CommonTypes.h"

struct GCPadStatus;
class QBoxLayout;
class QCheckBox;
class QDialog;
class QGroupBox;
class QSpinBox;
class QString;

class TunerWindow : public QDialog
{
	Q_OBJECT
public:
	explicit TunerWindow(QWidget* parent);

protected:
	template <typename UX>
	void GetButton(QCheckBox* button, UX& pad, UX mask);
	QCheckBox* m_use_gba;

private:
	std::map<QCheckBox*, bool> m_checkbox_set_by_gba;
};
