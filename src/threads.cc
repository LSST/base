#include <cstddef>
#include <cstdlib>

#include <iostream>

#include "lsst/base/library.h"
#include "lsst/base/threads.h"

namespace lsst {
namespace base {

namespace {

typedef int (Getter)(void);
typedef void (Setter)(int);

Getter* getOpenBlasThreads = NULL;
Setter* setOpenBlasThreads = NULL;
Getter* getMklThreads = NULL;
Setter* setMklThreads = NULL;

bool loadOpenBlas() {
    if (haveOpenBlas) {
        return true;
    }

    // Conda names the openblas library with .so.0 on Linux and .dylib
    // on macOS.  Try each of them explicitly, as we don't want to
    // accidentally find a non-conda .so version.
    try {
        getOpenBlasThreads = loadSymbol<Getter>("libopenblas.so.0", "goto_get_num_procs");
        // Believe it or not, the function which returns the number of threads
        // that OpenBLAS will actually use is called "goto_get_num_procs". The
        // number returned by THIS function, and not "openblas_get_num_threads"
        // nor "openblas_get_num_procs", is modified by calls to
        // "openblas_set_num_threads". Confused? Me too.
        setOpenBlasThreads = loadSymbol<Setter>("libopenblas.so.0", "openblas_set_num_threads");
        return true;
    } catch (LibraryException const&) {
        ;
    }
    try {
        getOpenBlasThreads = loadSymbol<Getter>("libopenblas.dylib", "goto_get_num_procs");
        setOpenBlasThreads = loadSymbol<Setter>("libopenblas.dylib", "openblas_set_num_threads");
    } catch (LibraryException const&) {
        return false;
    }
    return true;
}

bool loadMkl() {
    if (haveMkl) {
        return true;
    }

    if (!canLoadLibrary("libmkl_core")) {
        return false;
    }

    // We will set the number of threads through OMP because that's easier.
    // (There are multiple mkl libraries, and each has a function to set the number of threads.)
    try {
        getMklThreads = loadSymbol<Getter>("libiomp5", "omp_get_max_threads");
        setMklThreads = loadSymbol<Setter>("libiomp5", "omp_set_num_threads");
    } catch (LibraryException const&) {
        return false;
    }
    return true;
}

bool disableImplicitThreadingImpl(
    std::string const& package,                    // Name of package
    std::initializer_list<std::string const> envvars, // Environment variables to check
    Getter* getter,                      // Function to get number of threads
    Setter* setter                       // Function to set number of threads
    )
{
    for (auto&& ss : envvars) {
        if (std::getenv(ss.c_str())) {
            return false;               // User is being explicit
        }
    }
    unsigned int numThreads = getter();
    if (numThreads <= 1) {
        return false;
    }

    std::cerr << "WARNING: You are using " << package << " with multiple threads (" << numThreads <<
        "), but have not\n" <<
        "specified the number of threads using one of the " << package <<
        " environment variables:\n";

    // Join list of environment variables. An alternative is
    //     boost::algorithm::join(envvars, ", ")
    // but we are trying to control the proliferation of Boost usage.
    std::cerr << *envvars.begin();
    for (auto iter = envvars.begin() + 1; iter != envvars.end(); ++iter) {
        std::cerr << ", " << *iter;
    }

    std::cerr << ".\n" <<
        "This may indicate that you are unintentionally using multiple threads, which may\n"
        "cause problems. WE HAVE THEREFORE DISABLED " << package << " THREADING. If you know\n" <<
        "what you are doing and want threads enabled implicitly, set the environment\n" <<
        "variable " << allowEnvvar << ".\n";
    setter(1);
    return true;
}


} // anonymous namespace


extern bool const haveOpenBlas = loadOpenBlas();
extern bool const haveMkl = loadMkl();

void setNumThreads(unsigned int numThreads)
{
    if (!haveOpenBlas && !haveMkl && numThreads != 0 && numThreads != 1) {
        throw NoThreadsException();
    }
    if (haveOpenBlas) {
        setOpenBlasThreads(numThreads);
    }
    if (haveMkl) {
        setMklThreads(numThreads);
    }
}

unsigned int getNumThreads()
{
    unsigned int numThreads = 0;
    if (haveOpenBlas) {
        numThreads = std::max(numThreads, static_cast<unsigned int>(getOpenBlasThreads()));
    }
    if (haveMkl) {
        numThreads = std::max(numThreads, static_cast<unsigned int>(getMklThreads()));
    }
    return numThreads;
}

bool disableImplicitThreading()
{
    if (std::getenv(allowEnvvar.c_str())) {
        return false; // The user knows what he's doing; no intervention performed
    }

    bool intervened = false;            // Did we intervene on behalf of the user?
    if (haveOpenBlas) {
        intervened |= disableImplicitThreadingImpl(
            "OpenBLAS",
            {"OPENBLAS_NUM_THREADS", "GOTO_NUM_THREADS", "OMP_NUM_THREADS"},
            getOpenBlasThreads,
            setOpenBlasThreads
            );
    }
    if (haveMkl) {
        intervened |= disableImplicitThreadingImpl(
            "MKL",
            {"MKL_NUM_THREADS", "MKL_DOMAIN_NUM_THREADS", "OMP_NUM_THREADS"},
            getMklThreads,
            setMklThreads
            );
    }
    return intervened;
}

}} // namespace lsst::base
