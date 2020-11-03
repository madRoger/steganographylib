#ifndef CONTAINER_H
#define CONTAINER_H

#include "steganographylib_global.h"

class QBitArray;

namespace Steganography {

class STEGANOGRAPHYLIB_EXPORT Container
{
public:
    virtual ~Container() = default;

    virtual int capacity() const = 0;
    virtual bool isValid() const = 0;

    virtual bool pack(const QBitArray &array) = 0;
    virtual bool packBytes(const QByteArray &array) = 0;
    virtual bool packFile(const QString &file) = 0;

    virtual QBitArray unpack() = 0;
    virtual QByteArray unpackBytes() = 0;
    virtual bool unpackFile(const QString &file) = 0;
};

}

#endif // CONTAINER_H
