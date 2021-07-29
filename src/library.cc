#include "lsst/base/library.h"

namespace lsst {
namespace base {

namespace {

/// Does the string end with another string?
bool endsWith(std::string const& str, std::string const& end)
{
    return str.size() >= end.size() && str.compare(str.size() - end.size(), end.size(), end) == 0;
}

} // anonymous namespace


std::string libraryExtension()
{
#ifdef __APPLE__
    return ".dylib";
#else
    return ".so";
#endif
}


std::string getLibraryFilename(std::string const& name)
{
    if (endsWith(name, ".so") || endsWith(name, ".dylib")) {
        // User asked for a library with a certain extension, so give it to them
        return name;
    }
    return name + libraryExtension();
}


bool canLoadLibrary(std::string const& libName)
{
    return dlopen(getLibraryFilename(libName).c_str(), RTLD_NOW);
}


}} // lsst::base
