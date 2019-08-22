``simple_menu`` - Draw a Menu
=============================
.. versionadded:: 1.4

To allow quickly hacking some scripts, Pycardium has a small library for
displaying menus.  You can use it like this:

.. code-block:: python

   import color
   import simple_menu


   class MyMenu(simple_menu.Menu):
       color_1 = color.CAMPGREEN
       color_2 = color.CAMPGREEN_DARK

       def on_select(self, name, index):
           print("{!r} was selected!".format(name))


   if __name__ == "__main__":
       MyMenu(["foo", "bar", "baz"]).run()

.. autoclass:: simple_menu.Menu
   :members:

.. autofunction:: simple_menu.button_events
