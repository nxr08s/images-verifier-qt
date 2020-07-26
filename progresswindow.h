#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QDialog>
#include <QProgressBar>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>

class ProgressWindow : public QDialog{
    unsigned int totalNumber;
    QProgressBar pb;
    QVBoxLayout layout;
    QLabel  actNameLabel,
            progressLabel;

public:
    ProgressWindow(QWidget*, unsigned int, QString);
    void setValue(unsigned int);
};

#endif // PROGRESSWINDOW_H
