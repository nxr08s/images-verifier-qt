#include "helpdialog.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QTextBrowser>

HelpDialog::HelpDialog(QWidget* parent) : QDialog(parent){
    setWindowTitle("Справка");
    browser = new QTextBrowser();
    lt = new QVBoxLayout;
    lt->addWidget(browser);
    this->setLayout(lt);

    browser->setText(R"(
                     <h1 style="text-align:center;"><b>Тут должна быть справка</b></h1>
                     <div style="text-align:right;">*но пока ее тут нет</div>)"
                     );
}
