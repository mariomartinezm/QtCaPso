#ifndef LOCALSETTINGSDIALOG_H
#define LOCALSETTINGSDIALOG_H

#include <qdialog.h>
#include <qmap.h>
#include "ui_localsettingsdialog.h"

class LocalSettingsDialog : public QDialog, private Ui::LocalSettingsDialog
{
	Q_OBJECT

public:
	LocalSettingsDialog(QWidget* parent = 0);
	~LocalSettingsDialog();

signals:
	void settingsChanged(QMap<QString, QVariant> settings);

private slots:
	void showFileDialog();
	virtual void accept();

private:
	// Object used to send the new settings
	QMap<QString, QVariant> mSettings;
};

#endif // LOCALSETTINGSDIALOG_H