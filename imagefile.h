#ifndef IMAGEFILE_H
#define IMAGEFILE_H

#include <QString>
#include <QSize>
#include <QImage>

class ImageFile{
    QString fname;
    double currentAspectRatio;
    QSize imageSize;
    unsigned long long fileSize;

public:
    enum class Status{
        Ok,
        WrongAspectRatio,
        WrongExtension,
        LowRes,
        HighRes
    };

    ImageFile(QString);
    ~ImageFile();
    double aspectRatio() const;
    QString convert();
    bool scale(QSize);
    QString fileName() const;
    QSize size() const;
    QImage image();
    Status check(QSize, double, int) const;
    bool remove();

};

#endif // IMAGEFILE_H
