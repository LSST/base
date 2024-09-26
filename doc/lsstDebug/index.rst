.. py:currentmodule:: lsstDebug

.. _lsstDebug:

#########
lsstDebug
#########

``lsstDebug`` is a tool to help with live debugging of Science Pipelines packages.
Many `~lsst.pipe.base.Task`\s are configured to show debugging information when certain parameters are set in ``lsstDebug``, for example ``display`` might show some Task-specific images and plots.

.. _lsstDebug-contributing:

Contributing
============

``lsstDebug`` is developed at https://github.com/lsst/base.
You can find Jira issues for this module under the `base <https://jira.lsstcorp.org/issues/?jql=project%20%3D%20DM%20AND%20component%20%3D%20base>`_ component.

.. _lsstDebug-using:

Using lsstDebug to control debugging output
===========================================

The ``lsstDebug`` module is used to turn on debugging output and plots without changing the code being executed.
For example, the variable ``lsstDebug.Info("lsst.meas.astrom.astrometry").debug`` is used to control debugging output from the ``lsst.meas.astrom.astrometry`` module.

You may interrogate `lsstDebug` for any string in `sys.modules`, i.e. for the ``__name__`` of any package that has been imported; for example, if the ``lsst.meas.algorithms`` package is loaded then ``lsstDebug.Info("lsst.meas.algorithms").parameter`` will return `False` unless ``parameter`` has been set to True elsewhere.

The convention is that the name (e.g. ``lsst.meas.astrom.astrometry``) is the  ``__name__`` of the module, so that in ``astrometry.py`` you can check whether ``display`` was set like this:

.. code-block:: python

   import lsstDebug

   print(lsstDebug.Info(__name__).display)

which will print `False` unless ``lsstDebug.Info(__name__).display`` has somehow been set to `True`.

You can use this to turn on debug parameters in specific packages, by creating a ``debug.py`` file that is available in the ``PYTHONPATH``:

.. code-block:: python

   import lsstDebug

   def DebugInfo(name):
       di = lsstDebug.getInfo(name)  # N.b. lsstDebug.Info(name) would call us recursively
       if name == "debugExample":
           di.display = True

       return di


   lsstDebug.Info = DebugInfo

Then, if you have a ``debugExample.py`` module like this:

.. code-block:: python

   import lsstDebug

   print("display is", lsstDebug.Info(__name__).display)

Running without importing `debug.py` will result in ``display==False``:

.. code-block:: bash

    $ python -c "import foo"
    display is False

while importing `debug.py` will show:

.. code-block:: bash

    $ python -c "import debug; import foo"
    display is True

The ``pipetask run`` command line interface supports a flag ``--debug`` to import ``debug.py`` from your ``PYTHONPATH``.

To use lsstDebug when running tests with pytest, put the ``debug.py`` file described above in your package's ``tests/`` directory and ``import debug`` in the test file you are running.

Note that you may need to setup ``display_ds9``, ``display_firefly``, or ``display_astrowidgets`` in order to see the generated debug images.

.. _lsstDebug-pyapi:

Python API reference
====================

.. automodapi:: lsstDebug
   :no-main-docstr:
   :no-inheritance-diagram:

