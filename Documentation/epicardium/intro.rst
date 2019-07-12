Epicardium Intro
================

Epicardium is the main component of the firmware.  It runs on core0 and is
based on FreeRTOS.  Epicardium provides an API to a payload running on the
other core which exposes most functionality.

The API functions follow the kernel convention of either returning a boolean if
the function is a predicate or returning a success integer (with negative
values denoting errors) if it is an action or imperative command.  To quote:

      **16) Function return values and names**

      Functions can return values of many different kinds, and one of the
      most common is a value indicating whether the function succeeded or
      failed.  Such a value can be represented as an error-code integer
      (-Exxx = failure, 0 = success) or a ``succeeded`` boolean (0 = failure,
      non-zero = success).

      Mixing up these two sorts of representations is a fertile source of
      difficult-to-find bugs.  If the C language included a strong distinction
      between integers and booleans then the compiler would find these mistakes
      for us... but it doesn't.  To help prevent such bugs, always follow this
      convention:

      .. code-block:: text

              If the name of a function is an action or an imperative command,
              the function should return an error-code integer.  If the name
              is a predicate, the function should return a "succeeded" boolean.

      For example, ``add work`` is a command, and the add_work() function returns 0
      for success or -EBUSY for failure.  In the same way, ``PCI device present`` is
      a predicate, and the pci_dev_present() function returns 1 if it succeeds in
      finding a matching device or 0 if it doesn't.
