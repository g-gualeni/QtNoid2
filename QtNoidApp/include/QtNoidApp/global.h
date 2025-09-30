#ifndef QTNOIDAPP_GLOBAL_H
#define QTNOIDAPP_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QTNOIDAPP_LIBRARY
#  define QTNOIDAPP_EXPORT Q_DECL_EXPORT
#else
#  define QTNOIDAPP_EXPORT Q_DECL_IMPORT
#endif

#endif // QTNOIDAPP_GLOBAL_H
