#pragma once

#include <QDialog>
#include <QList>
#include "ui_batchdialog.h"
#include "capsosettings.h"
#include "batchitem.h"

class BatchDialog : public QDialog, private Ui::BatchDialog
{
    Q_OBJECT
public:
    explicit BatchDialog(QWidget *parent = 0, capso::CaType type = capso::LOCAL);

private slots:
    void on_buttonStart_clicked();
    void on_buttonBrowseSettings_clicked();
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();
    void on_buttonClear_clicked();
    void on_buttonBrowse_clicked();
    void on_lineEditPath_textChanged(QString text);

private:
    void processItem(BatchItem& batchItem);

private:
    capso::CaType mType;
    QList<BatchItem> batchItems;
};
