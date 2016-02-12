#include "mlinterface.h"

#if defined(_WIN32)
    #include "./windows/mlinterfacewindows.h"
#else
#error "Non-Windows systems not implemented; please write a MLInterface for " \
    "your OS"
#endif


// This is the OS-specific selector
std::unique_ptr<MLInterface> MLInterface::getInterface() {
#if defined(_WIN32)
    std::unique_ptr<MLInterface> ptr(new MLInterfaceWindows());
    ptr.get()->init_();
    return ptr;
#else
#error "Non-Windows systems not implemented; please write a MLInterface for " \
    "your OS"
#endif
}
