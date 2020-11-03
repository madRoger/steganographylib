#ifndef STEGANOGRAPHYLIB_GLOBAL_H
#define STEGANOGRAPHYLIB_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(STEGANOGRAPHYLIB)
#  define STEGANOGRAPHYLIB_EXPORT Q_DECL_EXPORT
#else
#  define STEGANOGRAPHYLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // STEGANOGRAPHYLIB_GLOBAL_H
