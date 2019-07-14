Epicardium API Development
==========================
If you are interested in augmenting the Epicardium API with new calls, this
page is the right place for you.

API Call Declaration
--------------------
All API calls are declared in ``epicardium/epicardium.h``.  We also generated
documentation for all API calls using sphinx.  Please document any new calls in
the same format as existing calls.  An example API call definition might look
like this:

.. code-block:: cpp

   /* At the top of epicardium.h, add a unique ID for your call */
   #define API_ROCKET           0xc0ffee

   /**
    * Turn on card10's rocket engines.
    *
    * :param force: Force in Newtons.
    * :param color: 24-bit exhaust color.
    * :return: ``0`` on success or ``-Exxx`` on error.  The following errors
    *     might occur:
    *
    *     - ``-ENODEV``: Sadly, card10 does not have rocket engines.
    */
   API(API_ROCKET, int epic_rocket_start(float force, uint32_t color));

There are a number of rules you should follow when defining new calls:

* API calls have the prefix ``epic_`` which of course is just an abbreviation
  for *Epicardium*.
* There are no special rules regarding call numbers, choose any number not yet
  in use.  Call numbers are ``uint32_t`` so you have plenty to choose from.
* Only use types from the standard library or types defined (and documented!)
  in ``epicardium.h``.  **Never** include another header in ``epicardium.h``.
* When passing pointers, keep in mind that this will mean the other core will
  potentially modify stuff in your address space.  Please prefer passing pointers
  into core 1 (Pycardium) address space and refrain from returning pointers into
  Epicardium address space if possible.
* API calls follow the kernel convention of either returning a boolean if the
  name is a predicate or a success integer (with 0 denoting success and
  negative values denoting errors) if they are an imperative command or action
  (ref `Kernel Coding Style`_).  If you are reasonably sure your call cannot fail
  or an error is non-recoverable from core 1, return ``void``.

.. _Kernel Coding Style: https://www.kernel.org/doc/html/v5.2/process/coding-style.html#function-return-values-and-names

API Call Definition
-------------------
API calls should be defined in a source file in ``epicardium/modules``.  If
there is not yet one where your call fits in, create a new one.  Don't forget
to also add it in ``epicardium/modules/meson.build`` to make the build-system
aware of it.

For the example above, the definition might look like this:

.. code-block:: cpp

   /* epicardium/modules/rocket.c */
   #include <stdio.h>
   #include "epicardium.h"

   int epic_rocket_start(float force, uint32_t color)
   {
           printf("Starting rocket engines with %fN in color 0x%x"
                  force,
                  color);

           /* Aw :( */
           return -ENODEV;
   }

To keep code-style uniform across the project, please format your code using
``./tools/code-style.sh``.  Note that this is not a definite style, however:
If something looks better when manually formatted, don't be afraid to do so.

.. warning::

   When writing you calls, **never** make assumptions about which FreeRTOS task
   you are running in.  While all calls from core 1 will end up in the
   "Dispatcher" task, other FreeRTOS tasks might might call your code at any
   time just as well.

   This is especially important if you use a `task semaphore`_.  Always call
   ``xTaskGetCurrentTaskHandle()``.

   .. _task semaphore: https://freertos.org/RTOS_Task_Notification_As_Binary_Semaphore.html
