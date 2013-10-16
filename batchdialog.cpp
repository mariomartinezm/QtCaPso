#include "batchdialog.h"
#include "localcapso.h"
#include "globalcapso.h"

BatchDialog::BatchDialog(QWidget *parent, CaType type) :
    QDialog(parent), mType(type)
{
    this->setupUi(this);
}
