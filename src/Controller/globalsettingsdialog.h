#ifndef GLOBALSETTINGSDIALOG_H
#define GLOBALSETTINGSDIALOG_H

#include <QDialog>
#include <QMap>
#include "ui_globalsettingsdialog.h"

class GlobalSettingsDialog : public QDialog, private Ui::GlobalSettingsDialog
{
	Q_OBJECT

public:
	GlobalSettingsDialog(QWidget* parent = 0);
	~GlobalSettingsDialog();

signals:
	void settingsChanged(QMap<QString, QVariant> settings);

private slots:
	void showFileDialog();
	virtual void accept();

private:
	// Object used to send the new settings
	QMap<QString, QVariant> mSettings;
};

#endif // GLOBALSETTINGSDIALOG_H
