#include "stdafx.h"
#include "TranslateSettingsDialog.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QComboBox>
#include "Translator.h"
#include "TranslateMode.h"

TranslateSettingsDialog::TranslateSettingsDialog(QWidget* parent): QDialog(parent)
{
    InitUI();
    setAttribute(Qt::WA_DeleteOnClose);
}

TranslateSettingsDialog::~TranslateSettingsDialog()
{
}

void TranslateSettingsDialog::InitUI()
{
    m_TranslateModeLabel = new QLabel();
    m_TranslateModeCombox = new QComboBox();
    m_DialogLayout = new QGridLayout();
    m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel);

    m_TranslateModeLabel->setText("Translate Mode: ");
    QVector<QString> modelNameList = TranslateMode::GetModelNameList();
    for (QString name : modelNameList)
    {
        m_TranslateModeCombox->addItem(name);
    }
    m_TranslateModeCombox->setCurrentText(TranslateMode::GetNameByMode(TranslateMode::Type::COMMON));

    connect(m_ButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_DialogLayout->addWidget(m_TranslateModeLabel, 0, 0);
    m_DialogLayout->addWidget(m_TranslateModeCombox, 0, 1);
    m_DialogLayout->addWidget(m_ButtonBox, 2, 1, 1, 2, Qt::AlignCenter);

    setLayout(m_DialogLayout);
}

void TranslateSettingsDialog::accept()
{
    Translator* translator = qobject_cast<Translator*>(this->parentWidget());
    translator->SetTranslateMode(TranslateMode::GetModeByName(m_TranslateModeCombox->currentText()));

    QDialog::accept();
}
