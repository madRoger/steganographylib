#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>
#include <QBitArray>
#include <QVector>

#include "container.h"
#include "steganographylib_global.h"

namespace Steganography {

class STEGANOGRAPHYLIB_EXPORT Synchronizer : public QObject
{
    Q_OBJECT
public:
    explicit Synchronizer(QObject *parent = nullptr);

    void setSource(const QByteArray &array);
    bool setSource(const QString &file);
    bool appendContainer(Container *container);
    bool appendContainers(const QVector<Container *> &conts);
    bool removeContainer(Container *container) { return containers.removeOne(container); }
    bool containersValid() const;
    const QString& lastErrorString() const { return lastError; }
    int sourceSize() const { return source.size(); }
    int containersCapacity() const;
    bool pack();
    QByteArray unpackBytes();
    bool unpackFile(const QString &file);

private:
    QVector<Container *> containers;
    QBitArray source;
    QString lastError;

signals:
    void packFinished(bool result);
    void unpackFinished(bool result);
    void unpackToFileFinished(bool result);
};

} //namespace Steganography

#endif // SYNCHRONIZER_H
