/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
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
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
#if !defined(LSST_BASE_BASE_H)
#define LSST_BASE_BASE_H 1
/**
 * @file
 *
 * Basic LSST definitions
 */
#include <memory>

/**
 * Make a symbol visible even if visiblity is hidden (e.g. for pybind11 modules)
 *
 * This implementation works for gcc and clang; it has not been tested for other compilers
 */
# define LSST_EXPORT __attribute__ ((visibility("default")))

/**
 * Make a symbol hidden even if default visiblity is public.
 *
 * This implementation works for gcc and clang; it has not been tested for other compilers.
 *
 * This macro should be used for most utility code for pybind11 wrapping (at
 * least non-template, non-inline code).  It can also be used in non-pybind11
 * code to make a symbol private to a library without also forcing it to be
 * private to a single source file.
 */
# define LSST_PRIVATE __attribute__ ((visibility("hidden")))

#endif
