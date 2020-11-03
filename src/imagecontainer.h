#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include "filecontainer.h"
#include "steganographylib_global.h"

class QString;

namespace Steganography {

class STEGANOGRAPHYLIB_EXPORT ImageContainer : public FileContainer
{
public:
    explicit ImageContainer(const QString &infile, const QString &outfile = QString{});

    int capacity() const override;
    bool isValid() const override;

    bool pack(const QBitArray &array) override;
    QBitArray unpack() override;
};

}

#endif // IMAGECONTAINER_H
