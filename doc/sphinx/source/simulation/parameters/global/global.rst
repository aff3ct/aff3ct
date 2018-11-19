.. _global-other-parameters:

Other parameters
----------------

Those arguments are not associated to any module.


.. _global-help:

``--help, -h``
""""""""""""""

Prints the help with all the *required* and *optional* arguments.
The last change in function of the selected simulation type and code.

.. _global-advanced-help:

``--Help, -H``
""""""""""""""

Prints the help with in more the advanced arguments.

.. _global-version:

``--version, -v``
"""""""""""""""""

Prints informations about the version of the source code and
compilation options.

.. _global-except-a2l:

``--except-a2l`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""

Enhances the backtrace when displaying exception.
This change the program addresses into filenames and lines.
It may take some seconds to do this work.

.. note:: This option works only on Unix based OS and if |AFF3CT| has been
   :ref:`compiled <compilation_compiler_options>` with debug symbols
   (``-g`` compile flag) and **without** ``NDEBUG`` macro (``-DNDEBUG`` flag).

.. _global-except-no-bt:

``--except-no-bt`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""

Disables the print of backtrace when displaying exception.
