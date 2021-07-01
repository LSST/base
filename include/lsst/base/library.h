#ifndef LSST_BASE_LIBRARY_H
#define LSST_BASE_LIBRARY_H

#include <stdexcept>
#include <string>

#include <dlfcn.h>
#ifndef RTLD_DEEPBIND // Non-POSIX flag, so it may not exist
#define RTLD_DEEPBIND 0 // Will be ignored
#endif

namespace lsst {
namespace base {

/// Unable to load library
class LibraryException : public std::runtime_error {
public:
    LibraryException(std::string const& name) :
        std::runtime_error("Unable to dynamically load library " + name) {};
    LibraryException(std::string const& lib, std::string const& func) :
        std::runtime_error("Unable to find function " + func + " from dynamically loaded library " + lib) {};
};

/// Return filename extension for libraries
///
/// Typically ".so" for Linux and ".dylib" for Mac.
std::string libraryExtension();

/// Get filename for library
///
/// We'll add the typical filename extension for the platform unless
/// the user specifies a ".so" or ".dylib" extension.
std::string getLibraryFilename(std::string const& name);

/// Return whether we can load a library
///
/// The proper filename extension will be added to the library name
/// unless one is specified.
bool canLoadLibrary(
    std::string const& libName          ///< Library name
    );


/// Load a symbol from a dynamic library
///
/// The proper filename extension will be added to the library name
/// unless one is specified.
///
/// No mangling is performed on the symbol name.
template <typename T>
T* loadSymbol(
    std::string const& libName,         ///< Library name (NOT including ".so" or ".dylib")
    std::string const& symName          ///< Symbol name
    )
{
    void* lib = dlopen(getLibraryFilename(libName).c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);
    if (!lib) {
        throw LibraryException(libName);
    }

    T* sym;
    (void*&)sym = dlsym(lib, symName.c_str());
    if (!sym) {
        throw LibraryException(libName, symName);
    }
    return sym;
}

}} // lsst::base


#endif
