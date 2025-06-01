#ifndef QTNOIDCORE_GLOBAL_H
#define QTNOIDCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QTNOIDCORE_LIBRARY
#  define QTNOIDCORE_EXPORT Q_DECL_EXPORT
#else
#  define QTNOIDCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // QTNOIDCORE_GLOBAL_H