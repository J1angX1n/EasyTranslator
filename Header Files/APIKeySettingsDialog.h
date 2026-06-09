#pragma once
#include <QDialog>

class QLabel;
class QLineEdit;
class QComboBox;
class QVBoxLayout;
class QHBoxLayout;
class QDialogButtonBox;

class APIKeySettingsDialog : public QDialog
{
	Q_OBJECT
public:
	APIKeySettingsDialog(QWidget* parent = nullptr);
	~APIKeySettingsDialog();

	void InitUI();
	void accept() override;

private:
	QLabel* m_ModelLabel;
	QLabel* m_APIKeyLabel;
	QComboBox* m_ModelComboBox;
	QLineEdit* m_APIKeyLineEdit;

	QVBoxLayout* m_DialogLayout;
	QHBoxLayout* m_ModelLayout;
	QHBoxLayout* m_APIKeyLayout;

	QDialogButtonBox* m_ButtonBox;
};

