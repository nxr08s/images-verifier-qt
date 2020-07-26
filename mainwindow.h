#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListWidgetItem;
class QProgressBar;
class HelpDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT


    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);

    QProgressBar *sb;
    QAction* act1000px;
    QAction* act460px;
    HelpDialog *hdiag;

    unsigned int numOfCorrectFiles;
    QSize desiredSize;

    void addFiles(QString);
    bool checkFile(QListWidgetItem*);
    void checkAllFiles();
    void convertToJpg(QListWidgetItem*);
    void cropImage(QListWidgetItem*);
    void clearList();

    void createMenus();

    void updateInfo(bool erase = false);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void itemSelected();
    void convertAllToJpg();
    void paintOpenClicked();
    void cropAllImages();
    void clearClicked();
    void removeClicked();
    void arSliderChanged(int);
    void sizeSliderChanged(int);
    void radio1000Clicked();
    void radio460Clicked();
    void updateListClicked();
    void helpClicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
