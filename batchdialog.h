#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>
#include "ui_batchdialog.h"

class BatchDialog : public QDialog, private Ui::BatchDialog
{
    Q_OBJECT
public:
    explicit BatchDialog(QWidget *parent = 0);

signals:

public slots:

};

#endif // BATCHDIALOG_H
