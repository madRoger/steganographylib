#ifndef FILECONTAINER_H
#define FILECONTAINER_H

#include <QString>

#include "container.h"
#include "steganographylib_global.h"

namespace Steganography {

class STEGANOGRAPHYLIB_EXPORT FileContainer : public Container
{
public:
    explicit FileContainer(const QString &srcFile, const QString &trgFile = QString{});

    int capacity() const override { return 0; }
    bool isValid() const override { return true; }

    bool pack(const QBitArray &array) override;
    bool packBytes(const QByteArray &array) override;
    bool packFile(const QString &file) override;

    QBitArray unpack() override;
    QByteArray unpackBytes() override;
    bool unpackFile(const QString &file) override;

    const QString& lastErrorString() const { return lastError; }

protected:
    QString sourceFile;
    QString targetFile;
    QString lastError;
};

}

#endif // FILECONTAINER_H
