#ifndef QTNOID_APP_GLOBAL_H
#define QTNOID_APP_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QTNOIDAPP_LIBRARY
#  define QTNOID_APP_EXPORT Q_DECL_EXPORT
#else
#  define QTNOID_APP_EXPORT Q_DECL_IMPORT
#endif

#endif // QTNOID_APP_GLOBAL_H
