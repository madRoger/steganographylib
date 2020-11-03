#include <QByteArray>
#include <QFuture>
#include <QFutureSynchronizer>
#include <QtConcurrent>

#include "synchronizer.h"
#include "utils.h"

namespace Steganography {

Synchronizer::Synchronizer(QObject *parent) : QObject(parent) {}

void Synchronizer::setSource(const QByteArray &array)
{
    source = bytesToBits(array);
}

bool Synchronizer::setSource(const QString &file)
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

    setSource(f.readAll());
    f.close();
    return true;
}

bool Synchronizer::appendContainer(Container *container)
{
    if (containers.count(container))
    {
        lastError = QStringLiteral("Can't append container twice");
        return false;
    }

    containers.append(container);
    return true;
}

bool Synchronizer::appendContainers(const QVector<Container *> &conts)
{
    for (auto *c : conts)
        if (containers.count(c))
        {
            lastError = QStringLiteral("One of containers allready present");
            return false;
        }

    containers.append(conts);
    return true;
}

bool Synchronizer::containersValid() const
{
    for(const auto *c: containers)
        if (!c->isValid())
            return false;

    return true;
}

int Synchronizer::containersCapacity() const
{
    int sum{0};
    for(const auto *c: containers)
        sum += c->capacity();

    return sum;
}

bool Synchronizer::pack()
{
    lastError.clear();
    if (containers.empty())
    {
        lastError = QStringLiteral("Containers is empty");
        emit packFinished(false);
        return false;
    }

    if (source.isEmpty())
    {
        lastError = QStringLiteral("Source is empty");
        emit packFinished(false);
        return false;
    }

    if (containersCapacity() < sourceSize())
    {
        lastError = QStringLiteral("Containers capacity is too small");
        emit packFinished(false);
        return false;
    }

    if (!containersValid())
    {
        lastError = QStringLiteral("At least one container is invalid");
        emit packFinished(false);
        return false;
    }

    QFutureSynchronizer<bool> sync{};
    int pos{0};
    for (auto *c: containers)
    {
        QBitArray part(qMin(source.size() - pos, c->capacity()));
        for (int i = 0; i < part.size(); ++i)
            part[i] = source.at(pos+i);

        pos += part.size();
        sync.addFuture(QtConcurrent::run(c, &Container::pack, part));
    }

    sync.waitForFinished();
    for (const auto &future : sync.futures())
        if (!future.result())
        {
            lastError = QStringLiteral("At least one container has bad pack's result");
            emit packFinished(false);
            return false;
        }

    emit packFinished(true);
    return true;
}

QByteArray Synchronizer::unpackBytes()
{
    lastError.clear();
    if (containers.empty())
    {
        lastError = QStringLiteral("Containers is empty");
        emit unpackFinished(false);
        return QByteArray{};
    }

    if(!containersValid())
    {
        lastError = QStringLiteral("At least one container is invalid");
        emit unpackFinished(false);
        return QByteArray{};
    }

    QFutureSynchronizer<QBitArray> sync{};
    for (auto *c: containers)
        sync.addFuture(QtConcurrent::run(c, &Container::unpack));

    sync.waitForFinished();
    int index{0};
    QBitArray bits(containersCapacity());
    for (const auto &future : sync.futures())
    {
        if (future.result().isEmpty())
        {
            lastError = QStringLiteral("At least one container has bad unpack's result");
            emit unpackFinished(false);
            return QByteArray{};
        }

        const QBitArray part{future.result()};
        for (int i = 0; i < part.size(); ++i)
            bits[index++] = part.at(i);
    }

    QByteArray bytes{bitsToBytes(bits)};
    if (bytes.isEmpty())
    {
        lastError = QStringLiteral("Incorrect array. Append another container.");
        emit unpackFinished(false);
    } else
    {
        emit unpackFinished(true);
    }

    return bytes;
}

bool Synchronizer::unpackFile(const QString &file)
{
    lastError.clear();
    QByteArray array{unpackBytes()};
    if (array.isEmpty())
    {
        emit unpackToFileFinished(false);
        return false;
    }

    QFile f{file};
    if (!f.open(QIODevice::WriteOnly))
    {
        lastError = QString{"Can`t open and write file %1"}.arg(file);
        emit unpackToFileFinished(false);
        return false;
    }

    int writeSize = f.write(array);
    f.close();
    if (writeSize != array.size())
        lastError = QString{"Wrote %1 bytes from %2 to file %3"}.arg(writeSize)
                                                                .arg(array.size())
                                                                .arg(file);

    emit unpackToFileFinished(writeSize == array.size());
    return writeSize == array.size();
}

}//namespace Steganography
