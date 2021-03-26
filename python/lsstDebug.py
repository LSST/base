#
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <http://www.lsstcorp.org/LegalNotices/>.
#
__all__ = ("Info", "getDebugFrame")


#
# Define a class to configure debugging information
#
class Info:
    """An object cognisant of debugging parameters appropriate for module
    ``modname``.

    Any request for a value will return False unless that value has
    been set, either in the module or as an attribute of this object.

    E.g.

    .. code-block:: python

        import lsstDebug

        display = lsstDebug.Info(__name__).display

    will set display to False, unless display has been set with

    .. code-block:: python

        lsstDebug.Info(__name__).display = True

    Why is this interesting?  Because you can replace `lsstDebug.Info` with
    your own version, e.g.

    .. code-block:: python

        import lsstDebug

        def DebugInfo(name):
            # N.b. lsstDebug.Info(name) would call us recursively
            di = lsstDebug.getInfo(name)
            if name == "foo":
                di.display = dict(repair=1, background=2, calibrate=3)

            return di

        lsstDebug.Info = DebugInfo

    Parameters
    ----------
    modname : `str`
        Module name.

    Raises
    ------
    KeyError :
        Raised if ``modname`` is not loaded.
    """
    def __init__(self, modname):
        import sys
        self.__dict__["_dict"] = sys.modules[modname].__dict__
        self._modname = modname

    def __getattr__(self, what):
        """Return the value of the variable "what" in ``self.__modname``
        if set, else False"""
        return self._dict.get(what, False)

    def __setattr__(self, what, value):
        """Set the value of the variable "what" in ``self.__modname``
        to value"""
        self._dict[what] = value


getInfo = Info


def getDebugFrame(debugDisplay, name):
    """
    Attempt to extract a frame for displaying a product called ``name``
    from the ```debugDisplay`` variable.

    Per the above, an instance of `Info` can return an arbitrary object
    (or nothing) as its ``display`` attribute. It is convenient -- though not
    required -- that it be a dictionary mapping data products to frame
    numbers, as shown in the `lsstDebug.Info` example. Given such a dictionary,
    this function extracts and returns the appropriate frame number. If
    ``debugDisplay`` is not a collection, or if ``name`` is not found within
    it, we return `None`.

    Parameters
    ----------
    debugDisplay : `object`
        The contents of lsstDebug.Info(__name__).display.
    name : `str`
        The name of the data product to be displayed.

    Returns
    -------
    frame : `int`
        A frame number, or `None`.
    """
    if hasattr(debugDisplay, "__contains__") and name in debugDisplay:
        return debugDisplay[name]
    else:
        return None
