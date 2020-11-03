#include <QImage>
#include <QBitArray>

#include "imagecontainer.h"

namespace Steganography {

ImageContainer::ImageContainer(const QString &srcFile, const QString &trgFile) :
    FileContainer(srcFile, trgFile) {}

int ImageContainer::capacity() const
{
    QImage image{sourceFile};
    return image.height() * image.width() * 3;
}

bool ImageContainer::isValid() const
{
    return QImage{sourceFile}.format() != QImage::Format_Invalid;
}

bool ImageContainer::pack(const QBitArray &array)
{
    lastError.clear();
    QImage image{sourceFile};
    if (image.format() == QImage::Format_Invalid)
    {
        lastError = QString{"%1 has invalid image format"}.arg(sourceFile);
        return false;
    }

    image.convertTo(QImage::Format_ARGB32);
    if (targetFile.isEmpty())
        targetFile = sourceFile;

    if (!targetFile.endsWith(".png"))
        targetFile.append(QString{".png"});

    if ( array.isEmpty())
        return image.save(targetFile, "PNG");

    int index{0};
    QColor rgb{};
    for (int row = 0; row < image.height(); ++row)
    {
        for (int column = 0; column < image.width(); ++column)
        {
            rgb = image.pixelColor(column, row);
            rgb.setRed(array.at(index++) ? (rgb.red() | 1) : (rgb.red() & 254));
            if (index >= array.size())
            {
                image.setPixelColor(column, row, rgb);
                return image.save(targetFile, "PNG");
            }

            rgb.setGreen(array.at(index++) ? (rgb.green() | 1) : (rgb.green() & 254));
            if (index >= array.size())
            {
                image.setPixelColor(column, row, rgb);
                return image.save(targetFile, "PNG");
            }

            rgb.setBlue(array.at(index++) ? (rgb.blue() | 1) : (rgb.blue() & 254));
            image.setPixelColor(column, row, rgb);
            if (index >= array.size())
                return image.save(targetFile, "PNG");
        }
    }

    return image.save(targetFile, "PNG");
}

QBitArray ImageContainer::unpack()
{
    lastError.clear();
    QImage image{targetFile.isNull() ? sourceFile : targetFile};
    if (image.format() != QImage::Format_ARGB32)
    {
        lastError = QString{"%1 has invalid image format"}.arg(sourceFile);
        return QBitArray{};
    }

    int index{0};
    QColor rgb{};
    QBitArray array(capacity());
    for (int row = 0; row < image.height(); ++row)
    {
        for (int column = 0; column < image.width(); ++column)
        {
            rgb = image.pixelColor(column, row);
            array[index++] = rgb.red() & 1;
            array[index++] = rgb.green() & 1;
            array[index++] = rgb.blue() & 1;
        }
    }

    return array;
}

} //namespace Steganography
