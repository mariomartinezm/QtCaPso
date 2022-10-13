#pragma once

#include <QDialog>
#include <QMap>
#include "ui_localsettingsdialog.h"

class LocalSettingsDialog : public QDialog, private Ui::LocalSettingsDialog
{
	Q_OBJECT

public:
    LocalSettingsDialog(QWidget* parent = 0);
	~LocalSettingsDialog();

signals:
    void settingsChanged();

protected:
    void showEvent(QShowEvent*);

private slots:
	void showFileDialog();
	virtual void accept();
};
