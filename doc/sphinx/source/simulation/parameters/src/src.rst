.. _src-source-parameters:

Source parameters
-----------------

The source is the module at the beginning of the simulation chain as it
generates the frames that go through all the other modules.
It is at this step that you can set the number of frames that are simulated
jointly : this is called the *inter frame level*.

.. note::

   The inter frame level can be used in a multi user modulation (SCMA), or
   an inter frame |SIMD| optimization, for examples. It can also help to
   accelerate the simulation (for short frames) by letting each module working
   on more data at the same time instead of switching of module and memory zone
   more often.


.. _src-src-type:

``--src-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``RAND`` ``USER``
   :Default: ``RAND``
   :Examples: ``--src-type AZCW``

Method used to generate the words.
``AZCW`` means *All Zero Code Word*.

Description of the allowed values:

+----------+-----------------------+
| Value    | Description           |
+==========+=======================+
| ``AZCW`` | |src-type_descr_azcw| |
+----------+-----------------------+
| ``RAND`` | |src-type_descr_rand| |
+----------+-----------------------+
| ``USER`` | |src-type_descr_user| |
+----------+-----------------------+

.. |src-type_descr_azcw| replace:: Generates only frames with all bits at 0.
.. |src-type_descr_rand| replace:: Randomly generates frames with an uniform
   distribution.
.. |src-type_descr_user| replace:: Reads in loop the frames from a given file through
   :ref:`src-src-path`.

.. _src-src-fra:

``--src-fra, -F``
"""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--src-fra 8``

Sets the number of inter frame level to process.

.. _src-src-implem:

``--src-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Default: ``STD``
   :Examples: ``--src-implem FAST``

Select the implementation of the algorithm to generate the information bits.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |src-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |src-implem_descr_fast| |
+----------+-------------------------+

.. |src-implem_descr_std|  replace:: A standard implementation working for any
   source type.
.. |src-implem_descr_fast| replace:: A much faster method using |SIMD| but *only
   for* ``RAND`` *type*.


.. _src-src-path:

``--src-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--src-path ../conf/src/GSM-LDPC_2112.src``

Path to a file containing one or a set of pre-computed source bits, to use
with a ``USER`` source type.

.. _src-src-start-idx:

``--src-start-idx``
"""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--src-start-idx 42``

The start index to use in the ``USER`` type source. It is the index of the first
frame to read from the given file.
