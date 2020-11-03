#ifndef UTILS_H
#define UTILS_H

#include "steganographylib_global.h"

class QByteArray;
class QBitArray;

namespace Steganography {

STEGANOGRAPHYLIB_EXPORT QBitArray bytesToBits(const QByteArray &array);
STEGANOGRAPHYLIB_EXPORT QByteArray bitsToBytes(const QBitArray &array);

} //namespace Steganography

#endif // UTILS_H
