#ifndef QTNOIDJSON_GLOBAL_H
#define QTNOIDJSON_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QTNOIDJSON_LIBRARY
#  define QTNOIDJSON_EXPORT Q_DECL_EXPORT
#else
#  define QTNOIDJSON_EXPORT Q_DECL_IMPORT
#endif

#endif // QTNOIDJSON_GLOBAL_H
