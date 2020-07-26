#include "imagefile.h"

#include <QString>
#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QDir>

ImageFile::ImageFile(QString str) : fname(str){
    QImageReader imgReader(str);
    QFileInfo fi(str);
    imageSize = imgReader.size();
    fileSize = fi.size();
    currentAspectRatio = (double)imgReader.size().width() / (double)imgReader.size().height();
}

ImageFile::~ImageFile(){}

double ImageFile::aspectRatio() const{ return currentAspectRatio; }

QString ImageFile::convert(){
    QFileInfo fi(fname);
    QFileInfo newName(fi.absolutePath() + QStringLiteral("\\") + fi.completeBaseName() + ".jpg");
    QImage img(fname);
    this->remove();
    if (img.save(newName.absoluteFilePath(), "jpg"))
        return newName.absoluteFilePath();
    else
        return "";

}

bool ImageFile::scale(QSize newSize){
    QImage img(fname);
    img = img.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return img.save(fname);
}

QString ImageFile::fileName() const{ return fname; }

QSize ImageFile::size() const{ return imageSize; }

QImage ImageFile::image(){ return QImage(fname); }

bool ImageFile::remove(){ return QDir().remove(fname); }

ImageFile::Status ImageFile::ImageFile::check(QSize desiredSize, double eps, int pixEps) const{
    double compAR = (double)desiredSize.width() / (double)desiredSize.height();
    double compAREps = compAR * eps;
    QFileInfo fi(fname);

    // check for 'jpg' file extension
    if (fi.suffix() != "jpg")
        return Status::WrongExtension;

    // check for aspect ratio
    if (!(currentAspectRatio - compAR <= compAREps &&
          currentAspectRatio - compAR >= -compAREps)){
        return Status::WrongAspectRatio;
    }

    // check for low res
    if ((imageSize.width() < desiredSize.width() - pixEps) ||
            (imageSize.height() < desiredSize.height() - pixEps)){
        return Status::LowRes;
    }

    // check for high res
    if (imageSize.width() > desiredSize.width() + pixEps ||
            imageSize.height() > desiredSize.height() + pixEps){
        return Status::HighRes;
    }
    return Status::Ok;
}


