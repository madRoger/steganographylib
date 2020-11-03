#include "filecontainer.h"

#include <QBitArray>
#include <QFile>

#include "utils.h"

namespace Steganography {

FileContainer::FileContainer(const QString &srcFile, const QString &trgFile) :
    sourceFile(srcFile), targetFile(trgFile) {}

bool FileContainer::pack(const QBitArray &array)
{
    Q_UNUSED(array);
    return false;
}

bool FileContainer::packBytes(const QByteArray &array)
{
    return pack(bytesToBits(array));
}

bool FileContainer::packFile(const QString &file)
{
    QFile f{file};
    if (!f.exists())
    {
        lastError = QString{"%1 is not exists"}.arg(file);
        return false;
    }

    if (!f.open(QIODevice::ReadOnly))
    {
        lastError = QString{"Can`t open and read file %1"}.arg(file);
        return false;
    }

    QByteArray array{f.readAll()};
    f.close();
    return packBytes(array);
}

QBitArray FileContainer::unpack()
{
    return QBitArray{};
}

QByteArray FileContainer::unpackBytes()
{
    return bitsToBytes(unpack());
}

bool FileContainer::unpackFile(const QString &file)
{
    QByteArray array{unpackBytes()};
    if (array.isEmpty())
        return false;

    QFile f{file};
    if (!f.open(QIODevice::WriteOnly))
    {
        lastError = QString{"Can`t open and write file %1"}.arg(file);
        return false;
    }

    int writeSize = f.write(array);
    f.close();
    if (writeSize != array.size())
        lastError = QString{"Wrote %1 bytes from %2 to file %3"}.arg(writeSize)
                                                                .arg(array.size())
                                                                .arg(file);

    return writeSize == array.size();
}

}
