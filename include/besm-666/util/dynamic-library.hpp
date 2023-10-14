#include <string>
#include <stdexcept>
#include <filesystem>

#include "besm-666/util/non-copyable.hpp"

namespace besm::util {

class DynamicLibraryError : public std::runtime_error {
public:
    DynamicLibraryError(std::string const& message) :
        runtime_error(message) {}
    DynamicLibraryError(char const* message) :
        runtime_error(message) {}
};

class DynamicLibrarySymbolNotFound : public std::runtime_error {
public:
    DynamicLibrarySymbolNotFound(std::string const& message) :
        runtime_error(message) {}
    DynamicLibrarySymbolNotFound(char const* message) :
        runtime_error(message) {}
};

class DynamicLibrary : public INonCopyable {
public:
    explicit DynamicLibrary(std::filesystem::path const& path);
    DynamicLibrary(DynamicLibrary&& other);
    ~DynamicLibrary();

    DynamicLibrary const& operator=(DynamicLibrary&& other);

    void* getSymbol(std::string const& symbol);

    template<typename FunctionType>
    FunctionType getFunction(std::string const& symbol);

private:
    void* handle_;
};

template<typename FunctionType>
inline FunctionType DynamicLibrary::getFunction(std::string const& symbol) {
    return reinterpret_cast<FunctionType>(this->getSymbol(symbol));
}

    
}
