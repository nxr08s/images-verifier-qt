#include "progresswindow.h"

#include <QProgressBar>
#include <QPalette>

ProgressWindow::ProgressWindow(QWidget* parent, unsigned int count, QString actName) :
    QDialog(parent, Qt::WindowStaysOnTopHint),
    totalNumber(count),
    actNameLabel(actName)
{

    setModal(true);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::lightGray);
    setPalette(pal);
    actNameLabel.setAlignment(Qt::AlignCenter);
    layout.addWidget(&actNameLabel);
    layout.addWidget(&pb);
    layout.addWidget(&progressLabel);
    setLayout(&layout);

    pb.setRange(0, totalNumber);
    pb.setTextVisible(false);
}

void ProgressWindow::setValue(unsigned int newValue){
    pb.setValue(newValue);
}
