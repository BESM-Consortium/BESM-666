#include <dlfcn.h>

#include "besm-666/util/dynamic-library.hpp"

namespace besm::util {

DynamicLibrary::DynamicLibrary(std::filesystem::path const& path) {
    handle_ = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if(handle_ == nullptr) {
        char const* dl_error = dlerror();
        throw DynamicLibraryError(dl_error);
    }
}
DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) {
    handle_ = other.handle_;
    other.handle_ = nullptr;
}

DynamicLibrary const& DynamicLibrary::operator=(DynamicLibrary&& other) {
    std::swap(handle_, other.handle_);
    return *this;
}

DynamicLibrary::~DynamicLibrary() {
    if(handle_) {
        dlclose(handle_);
    }
}

void* DynamicLibrary::getSymbol(std::string const& symbol) {
    if(handle_) {
        void* symbolAddr = dlsym(handle_, symbol.c_str());
        if(symbolAddr == nullptr) {
            throw DynamicLibrarySymbolNotFound(symbol);
        }
        return symbolAddr;
    } else {
        return nullptr;
    }
}

}
