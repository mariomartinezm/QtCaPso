#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>
#include "ui_batchdialog.h"
#include "catype.h"

class BatchDialog : public QDialog, private Ui::BatchDialog
{
    Q_OBJECT
public:
    explicit BatchDialog(QWidget *parent = 0, CaType type = LOCAL);

private slots:
    void startBatch();
    void on_buttonBrowseSettings_clicked();
    void showFileDialog();
    void on_lineEditPath_textChanged(QString text);

private:
    CaType mType;
};

#endif // BATCHDIALOG_H
