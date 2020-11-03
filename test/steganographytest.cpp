#include <QtTest/QtTest>

#include "synchronizer.h"
#include "imagecontainer.h"

using namespace Steganography;

class SteganographyTest: public QObject
{
    Q_OBJECT
private slots:
    void oneContainerBytesTest();
    void oneContainerFileTest();
    void twoContainersBytesTest();
    void twoContainersFileTest();
};

void SteganographyTest::oneContainerBytesTest()
{
    ImageContainer ic(QString("1.png"), QString("1_packed.png"));
    ic.packBytes(QByteArray("0123456789"));

    QCOMPARE(QByteArray("0123456789"), ic.unpackBytes());
}

void SteganographyTest::oneContainerFileTest()
{
    ImageContainer ic(QString("1.png"), QString("1_packed.png"));
    ic.packFile(QStringLiteral("1.txt"));
    ic.unpackFile(QStringLiteral("1_unpacked.txt"));

    QFile file1(QStringLiteral("1.txt"));
    QFile file2(QStringLiteral("1_unpacked.txt"));

    file1.open(QIODevice::ReadOnly);
    file2.open(QIODevice::ReadOnly);

    QByteArray array1(file1.readAll());
    QByteArray array2(file2.readAll());

    file1.close();
    file2.close();

    QCOMPARE(array1, array2);
}

void SteganographyTest::twoContainersBytesTest()
{
    ImageContainer ic1(QString("1.png"), QString("1_packed.png"));
    ImageContainer ic2(QString("2.png"), QString("2_packed.png"));
    Synchronizer sync;
    sync.setSource(QByteArray("0123456789"));
    sync.appendContainer(&ic1);
    sync.appendContainer(&ic2);
    sync.pack();

    QCOMPARE(QByteArray("0123456789"), sync.unpackBytes());
}

void SteganographyTest::twoContainersFileTest()
{
    ImageContainer ic1(QString("1.png"), QString("1_packed.png"));
    ImageContainer ic2(QString("2.png"), QString("2_packed.png"));
    Synchronizer sync;
    sync.setSource(QStringLiteral("1.txt"));
    sync.appendContainer(&ic1);
    sync.appendContainer(&ic2);
    sync.pack();
    sync.unpackFile(QStringLiteral("1_unpacked.txt"));

    QFile file1(QStringLiteral("1.txt"));
    QFile file2(QStringLiteral("1_unpacked.txt"));

    file1.open(QIODevice::ReadOnly);
    file2.open(QIODevice::ReadOnly);

    QByteArray array1(file1.readAll());
    QByteArray array2(file2.readAll());

    file1.close();
    file2.close();

    QCOMPARE(array1, array2);
}

QTEST_MAIN(SteganographyTest)
#include "steganographytest.moc"
