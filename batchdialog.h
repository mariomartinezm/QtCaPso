#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>
#include <QList>
#include "ui_batchdialog.h"
#include "catype.h"
#include "batchitem.h"

class BatchDialog : public QDialog, private Ui::BatchDialog
{
    Q_OBJECT
public:
    explicit BatchDialog(QWidget *parent = 0, CaType type = LOCAL);

private slots:
    void startBatch();
    void on_buttonBrowseSettings_clicked();
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();
    void on_buttonClear_clicked();
    void showFileDialog();
    void on_lineEditPath_textChanged(QString text);

private:
    CaType mType;
    QList<BatchItem> batchItems;
};

#endif // BATCHDIALOG_H
