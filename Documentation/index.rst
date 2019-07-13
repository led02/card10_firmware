card10 firmware docs
====================

**Dear traveller,**

these transcripts describe how you can write code for your card10.  This
includes the Python modules that are available but also documentation of the
lower level firmware components.

If you want to write Python code for card10, you will want to take a look at
the :ref:`pycardium_index` docs.  If you are interested in writing applications
in other languages, you'll probably want to interface with
:ref:`epicardium_api_index` directly.

Last but not least, if you want to start hacking the lower-level firmware, the
:ref:`firmware_index` section of these docs is a good starting place.


.. _pycardium_index:
.. toctree::
   :maxdepth: 1
   :caption: Pycardium

   pycardium/overview
   pycardium/color
   pycardium/leds

.. _epicardium_api_index:
.. toctree::
   :maxdepth: 1
   :caption: Epicardium API

   epicardium/overview
   epicardium/api

.. _firmware_index:
.. toctree::
   :maxdepth: 1
   :caption: Firmware

   overview
   how-to-build
   how-to-flash
   debugger
