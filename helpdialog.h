#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

class QTextBrowser;
class QVBoxLayout;
class QWidget;

class HelpDialog : public QDialog{
    QTextBrowser *browser;
    QVBoxLayout *lt;

public:
    HelpDialog(QWidget*);
};

#endif // HELPDIALOG_H
