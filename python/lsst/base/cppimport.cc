/*
 * LSST Data Management System
 * Copyright 2008-2016  AURA/LSST.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */

#include "nanobind/nanobind.h"

#include "lsst/base/ModuleImporter.h"

namespace nb = nanobind;

namespace lsst {
namespace base {

class PythonModuleImporter : public ModuleImporter {
public:
    static ModuleImporter const* get() {
        static PythonModuleImporter const instance;
        return &instance;
    }

private:
    PythonModuleImporter() {}

protected:
    bool _import(std::string const& name) const override;
};

bool PythonModuleImporter::_import(std::string const& name) const {
    PyObject* mod = PyImport_ImportModule(name.c_str());
    if (mod) {
        Py_DECREF(mod);
        return true;
    } else {
        // If the Python C API call returned a null pointer, it will
        // also have set an exception.  We don't want that, because
        // this isn't necessarily an error (that's up to the caller).
        PyErr_Clear();
    }
    return false;
}

void installPythonModuleImporter() { ModuleImporter::install(PythonModuleImporter::get()); }

NB_MODULE(cppimport, mod) {
    lsst::base::installPythonModuleImporter();
}

}
}  // namespace lsst::base
