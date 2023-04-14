import warnings


warnings.warn(
    "lsstimport has been a no-op since v19.0 and will be removed after "
    "v27.0. Please remove the import from your __init__.py.",
    category=FutureWarning,
    stacklevel=2
)
