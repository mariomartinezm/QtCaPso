#pragma once

#include <QDialog>
#include <QMap>
#include "ui_localsettingsdialog.h"
#include "util.h"

class LocalSettingsDialog : public QDialog, private Ui::LocalSettingsDialog
{
    Q_OBJECT

public:
    LocalSettingsDialog(CaPsoSettings& settings, QWidget* parent = 0);
    ~LocalSettingsDialog();

signals:
    void settingsChanged();

protected:
    void showEvent(QShowEvent*);

private slots:
    void showFileDialog();
    virtual void accept();

private:
    CaPsoSettings& mSettings;
};
