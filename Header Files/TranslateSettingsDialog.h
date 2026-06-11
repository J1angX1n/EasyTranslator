#pragma once
#include <QDialog>
#include "TranslateMode.h"

class QComboBox;
class QGridLayout;
class QDialogButtonBox;
class QLabel;

class TranslateSettingsDialog : public QDialog
{
public:
	TranslateSettingsDialog(QWidget* parent = nullptr);
	~TranslateSettingsDialog();

	void InitUI();
	void accept() override;

private:
	QLabel* m_TranslateModeLabel;
	QComboBox* m_TranslateModeCombox;
	QDialogButtonBox* m_ButtonBox;

	QGridLayout* m_DialogLayout;


};

