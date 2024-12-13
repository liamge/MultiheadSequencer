#include "plugin.hpp"

// Define the global pluginInstance
Plugin* pluginInstance = nullptr;

// Forward declare the Model if needed, or include your module header
extern Model* modelMultiheadSequencer;

void init(Plugin* p) {
    pluginInstance = p;
    p->addModel(modelMultiheadSequencer);
}
