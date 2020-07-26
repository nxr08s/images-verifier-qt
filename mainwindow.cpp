#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QProgressBar>
#include <QProcess>

#include <QImageReader>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include <QDebug>

#include <QResizeEvent>
#include <QFileInfoList>
#include <QMessageBox>

#include "helpdialog.h"
#include "imagefile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    desiredSize(1000,666)
{
    ui->setupUi(this);

    createMenus();

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelected()));

    connect(ui->updateBtn, &QPushButton::clicked,[&]{updateInfo();});
    connect (ui->paintOpenBtn, SIGNAL(clicked(bool)), this, SLOT(paintOpenClicked()));
    connect(ui->clearBtn, SIGNAL(clicked(bool)), this, SLOT(clearClicked()));
    connect(ui->removeBtn, SIGNAL(clicked(bool)), this, SLOT(removeClicked()));
    connect(ui->updateListButton, SIGNAL(clicked(bool)), this, SLOT(updateListClicked()));

    connect(ui->radio1000px, SIGNAL(clicked(bool)), this, SLOT(radio1000Clicked()));
    connect(ui->radion460px, SIGNAL(clicked(bool)), this, SLOT(radio460Clicked()));

    connect(ui->arEpsSlider, SIGNAL(valueChanged(int)), this, SLOT(arSliderChanged(int)));
    connect(ui->sizeEpsSlider, SIGNAL(valueChanged(int)), this, SLOT(sizeSliderChanged(int)));

    sb = new QProgressBar(ui->statusBar);
    sb->setTextVisible(false);
    ui->statusBar->addPermanentWidget(sb);

    updateInfo(true);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::itemSelected(){
    updateInfo();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e){
    foreach(const QUrl& url, e->mimeData()->urls()){
        QFileInfo fi(url.toLocalFile());
        if (fi.isDir()){
            addFiles(fi.absoluteFilePath());
            continue;
        }
        QListWidgetItem *itm = new QListWidgetItem;
        itm->setData(Qt::UserRole, QVariant(url.toLocalFile()));
        itm->setText(fi.fileName());
        ui->listWidget->addItem(itm);
    }
    checkAllFiles();
}

void MainWindow::addFiles(QString dirPath){
    QDir dir(dirPath);
    QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (int i = 0; i < files.size(); i++){
        QFileInfo fi = files[i];
        if (fi.isDir()){
            addFiles(fi.absoluteFilePath());
            continue;
        }
        QListWidgetItem *itm = new QListWidgetItem;
        itm->setData(Qt::UserRole, fi.absoluteFilePath());
        itm->setText(fi.fileName());
        ui->listWidget->addItem(itm);
    }
}

bool MainWindow::checkFile(QListWidgetItem* itm){
    ImageFile imf(itm->data(Qt::UserRole).toString());
    switch (imf.check(desiredSize, 0.01f * ui->arEpsSlider->value(),ui->sizeEpsSlider->value())){
    case ImageFile::Status::WrongExtension:
        itm->setBackground(Qt::gray);
        return false;
    case ImageFile::Status::WrongAspectRatio:
        itm->setBackground(Qt::red);
        return false;
    case ImageFile::Status::HighRes:
        itm->setBackground(Qt::blue);
        return false;
    case ImageFile::Status::LowRes:
        itm->setBackground(Qt::darkRed);
        return false;
    case ImageFile::Status::Ok:
        itm->setBackground(Qt::white);
        return true;
    }
    return true;
}

void MainWindow::checkAllFiles(){
    sb->setRange(0, ui->listWidget->count());
    for (int i = 0; i < ui->listWidget->count(); i++) {
        checkFile(ui->listWidget->item(i));
        sb->setValue(i);
    }
    sb->reset();
}

void MainWindow::cropImage(QListWidgetItem* itm){
    ImageFile img(itm->data(Qt::UserRole).toString());
    if (img.check(desiredSize,0.01f * ui->arEpsSlider->value(),ui->sizeEpsSlider->value()) == ImageFile::Status::HighRes){
        img.scale(desiredSize);
    }
}

void MainWindow::clearList(){
    ui->listWidget->clear();
}

void MainWindow::convertAllToJpg(){
    if (!ui->listWidget->count())
        return;
    setEnabled(false);
    sb->setRange(0, ui->listWidget->count());
    for (int i = 0 ; i < ui->listWidget->count(); i++){
        ImageFile img(ui->listWidget->item(i)->data(Qt::UserRole).toString());
        QFileInfo fInfo(img.fileName());
        if (fInfo.suffix() == "jpg")
            continue;
        QString newName(img.convert());
        if (newName == ""){
            QMessageBox::critical(this, "Изменить расширение", "Не удалось изменить файл " + img.fileName());
            return;
        }
        QFileInfo fi(newName);
        ui->listWidget->item(i)->setData(Qt::UserRole, QVariant(newName));
        ui->listWidget->item(i)->setText(fi.completeBaseName() + "." + fi.suffix());
        sb->setValue(i);
    }
    setEnabled(true);
    sb->reset();
    checkAllFiles();
    updateInfo();
}

void MainWindow::paintOpenClicked(){
    QFileInfo fi(ui->listWidget->currentItem()->data(Qt::UserRole).toString());
    QStringList args;
    args << QDir::toNativeSeparators(fi.absoluteFilePath());
    QProcess::execute("C:\\Windows\\System32\\mspaint.exe", args);

    checkFile(ui->listWidget->currentItem());
    updateInfo();
}

void MainWindow::cropAllImages(){
    if (!ui->listWidget->count())
        return;
    setEnabled(false);
    sb->setRange(0, ui->listWidget->count());
    for (int i = 0 ; i < ui->listWidget->count(); i++){
        cropImage(ui->listWidget->item(i));
        sb->setValue(i);
    }
    setEnabled(true);
    sb->reset();
    checkAllFiles();
    updateInfo();
}

void MainWindow::clearClicked(){
    ui->listWidget->clear();
    updateInfo(true);
}

void MainWindow::removeClicked(){
    ImageFile img(ui->listWidget->currentItem()->data(Qt::UserRole).toString());
    QFile file(img.fileName());
    file.remove();
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void MainWindow::arSliderChanged(int newVal){
    ui->arEpsLabel->setText(QString::number(newVal) + "%");
}

void MainWindow::sizeSliderChanged(int newVal){
    ui->sizeEpsLabel->setText(QString::number(newVal) + "px");
}

void MainWindow::radio1000Clicked(){
    desiredSize = QSize(1000, 666);
    checkAllFiles();
    updateInfo();
}

void MainWindow::radio460Clicked(){
    desiredSize = QSize(460, 318);
    checkAllFiles();
    updateInfo();
}

void MainWindow::updateListClicked(){
    checkAllFiles();
}

void MainWindow::helpClicked(){
    hdiag = new HelpDialog(this);
    hdiag->setAttribute(Qt::WA_DeleteOnClose);
    hdiag->setModal(true);
    hdiag->show();
}

void MainWindow::createMenus(){
    QMenu* menu;
    QAction* act;

    menu = new QMenu("Действия", ui->menuBar);
    act = menu->addAction("Перекодирование в jpg");
    connect(act, SIGNAL(triggered(bool)), this, SLOT(convertAllToJpg()));
    act = menu->addAction("Промасштабировать все доступные");
    connect(act, SIGNAL(triggered(bool)), this, SLOT(cropAllImages()));
    ui->menuBar->addMenu(menu);

    act = ui->menuBar->addAction("Справка");
    connect(act, SIGNAL(triggered(bool)), this, SLOT(helpClicked()));
}

void MainWindow::updateInfo(bool erase){
    if (erase | !ui->listWidget->count() | !ui->listWidget->currentItem()){
        ui->imgLabel->clear();
        ui->imageSizeLabel->clear();
        ui->ARLabel->clear();
        ui->fileSizeLabel->clear();
        ui->errLabel->clear();
        ui->paintOpenBtn->setEnabled(false);
        ui->removeBtn->setEnabled(false);
    }else{
        ImageFile img(ui->listWidget->currentItem()->data(Qt::UserRole).toString());
        QFileInfo fi(img.fileName());

        ui->imgLabel->setPixmap(QPixmap::fromImage(img.image().scaled(ui->imgLabel->size() - QSize(3, 3),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
        ui->imageSizeLabel->setText("(" + QString::number(img.size().width()) + "," + QString::number(img.size().height()) + ")");
        ui->ARLabel->setText(QString::number(img.aspectRatio()));
        ui->fileSizeLabel->setText(this->locale().formattedDataSize(fi.size()));
        ui->paintOpenBtn->setEnabled(true);
        ui->removeBtn->setEnabled(true);

        switch(img.check(desiredSize, 0.01f * ui->arEpsSlider->value(), ui->sizeEpsSlider->value())){
        case ImageFile::Status::HighRes:
            ui->errLabel->setText("Слишком высокое разрешение");
            break;
        case ImageFile::Status::LowRes:
            ui->errLabel->setText("Слишком низкое разрешение");
            break;
        case ImageFile::Status::WrongAspectRatio:
            ui->errLabel->setText("Неверное соотношение сторон");
            break;
        case ImageFile::Status::WrongExtension:
            ui->errLabel->setText("Расширение файла не jpg");
            break;
        case ImageFile::Status::Ok:
            ui->errLabel->setText("Отсутствует");
            break;
        }
    }
}
