#pragma once

#include <QtGlobal>

#if defined(ASN1ACN_LIBRARY)
#  define ASN1ACNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ASN1ACNSHARED_EXPORT Q_DECL_IMPORT
#endif
