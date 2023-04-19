#pragma once

#include <plugin/IPluginInterface.h>

class PythonRegistrator : public IPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(IPluginInterface)
    Q_PLUGIN_METADATA(IID "Python")

public:
    void registerTypes(ADIContainer& diContainer) override;
};
