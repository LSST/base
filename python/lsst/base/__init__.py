from .version import *

try:
    # Keep backwards compatibility for Packages.
    from lsst.utils.packages import *
except ImportError:
    pass
