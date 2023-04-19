#pragma once

#include <QtCore/qglobal.h>

#if defined(PYTHON_LIBRARY)
#define PYTHON_EXPORT Q_DECL_EXPORT
#else
#define PYTHON_EXPORT Q_DECL_IMPORT
#endif
