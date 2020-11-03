#include <QBitArray>

#include "utils.h"

namespace Steganography {

QBitArray bytesToBits(const QByteArray &array)
{
    static constexpr int sizeBytes{4};//Placeholder (in bytes) for array size
    QByteArray bytes{};
    bytes.reserve(sizeBytes+array.size());
    for (int i = sizeBytes-1; i >= 0; --i)
        bytes.append((array.size() >> i*8) % 256);

    bytes.append(array);
    QBitArray bits(bytes.size()*8);
    int index{0};
    for (auto it = bytes.cbegin(), cend = bytes.cend(); it != cend; ++it)
    {
        auto val = *it;
        for (int i = 0; i < 8; ++i)
            bits.setBit(index++, val & (128 >> i));
    }

    return bits;
}

QByteArray bitsToBytes(const QBitArray &array)
{
    static constexpr int sizeBits{32};//Placeholder (in bits) for array size
    if (array.size() < sizeBits)
        return QByteArray{};

    int arraySize{0};
    for (int i = 0; i < sizeBits; ++i)
        arraySize = (arraySize << 1) | array.at(i);

    if (arraySize*8 > array.size()-sizeBits)
        return QByteArray{};

    int index{sizeBits};//first 32 bits contains array size
    QByteArray bytes(arraySize, 0);
    char val{0};
    for (auto it = bytes.begin(), end = bytes.end(); it != end; ++it)
    {
        val = 0;
        for (int i = 0; i < 8; ++i)
            val = (val << 1) | array.at(index++);

        *it = val;
    }

    return bytes;
}

}
