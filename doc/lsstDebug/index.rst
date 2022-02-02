.. py:currentmodule:: lsstDebug

.. _lsstDebug:

#########
lsstDebug
#########

.. Paragraph that describes what this Python module does and links to related modules and frameworks.

.. Add subsections with toctree to individual topic pages.

.. _lsstDebug-contributing:

Contributing
============

``lsstDebug`` is developed at https://github.com/lsst/base.
You can find Jira issues for this module under the `base <https://jira.lsstcorp.org/issues/?jql=project%20%3D%20DM%20AND%20component%20%3D%20base>`_ component.

.. _lsstDebug-using:

Using lsstDebug to control debugging output
===========================================

The class `lsstDebug`` can be used to turn on debugging output in a non-intrusive way.
For example, the variable ``lsstDebug.Info("lsst.meas.astrom.astrom").debug`` is used to control debugging output from the ``lsst.meas.astrom.astrom`` module.

You may interrogate `lsstDebug` for any string in `sys.modules`, i.e. for the ``__name__`` of any package that has been imported;  for example, if the ``Robert.Hugh.Lupton`` package is loaded then ``lsstDebug.Info("Robert.Hugh.Lupton").parameter`` will return `False` for any named parameter that has not already been set to True elsewhere.

The convention is that the name (``lsst.meas.astrom.astrom``) is the  ``__name__`` of the module, so the source code will typically look something like:

.. code-block:: python

   import lsstDebug

   print(lsstDebug.Info(__name__).display)

which will print `False` unless ``lsstDebug.Info(\__name__).display`` has somehow been set to `True`.

Why is this interesting?
Because you can replace `lsstDebug.Info` with your own version, *e.g.* if you put

.. code-block:: python

   import lsstDebug

   def DebugInfo(name):
       di = lsstDebug.getInfo(name)  # N.b. lsstDebug.Info(name) would call us recursively
       if name == "foo":
           di.display = True

       return di


   lsstDebug.Info = DebugInfo

into a file ``debug.py`` available in the ``PYTHONPATH`` and

.. code-block:: python

   import lsstDebug

   print("display is", lsstDebug.Info(__name__).display)

into ``foo.py``, then

.. code-block:: bash

    $ python -c "import foo"
    display is False

but

.. code-block:: bash

    $ python -c "import debug; import foo"
    display is True

The ``pipetask run`` command line interface supports a flag ``--debug`` to import ``debug.py`` from your ``PYTHONPATH``.


.. _lsstDebug-pyapi:

Python API reference
====================

.. automodapi:: lsstDebug
   :no-main-docstr:
