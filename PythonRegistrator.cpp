#include "PythonRegistrator.h"

#include <ioc/ADIContainer.h>

#include "src/PyFunction.h"

void PythonRegistrator::registerTypes(ADIContainer& diContainer)
{
    diContainer.registerImplementation<IPyFunction, PyFunction>(ADIContainer::InstanceType::InstanceSingletone);
}
