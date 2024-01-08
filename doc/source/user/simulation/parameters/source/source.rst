.. _src-source-parameters:

Source parameters
-----------------

The source generates :math:`K` information bits: it is the simulation starting
point.

.. _src-src-info-bits:

``--src-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--src-info-bits 64`` ``-K 128``

|factory::Source::p+info-bits,K|

.. warning:: This argument is required only with the ``UNCODED`` simulation code
   type (cf. the :ref:`sim-sim-cde-type` parameter).

.. _src-src-type:

``--src-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``RAND`` ``USER`` ``USER_BIN``
   :Default: ``RAND``
   :Examples: ``--src-type AZCW``

|factory::Source::p+type|

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``AZCW``     | Set all the information bits to 0.                            |
+--------------+---------------------------------------------------------------+
| ``RAND``     | Generate randomly the information bits based on the |MT 19937||
|              | |PRNG| :cite:`Matsumoto1998b`.                                |
+--------------+---------------------------------------------------------------+
| ``USER``     | Read the information bits from a given file, the path can be  |
|              | set with the :ref:`src-src-path` parameter.                   |
+--------------+---------------------------------------------------------------+
| ``USER_BIN`` | Read the information bits from a given file in binary mode    |
|              | , the path can be set with the :ref:`src-src-path` parameter. |
+--------------+---------------------------------------------------------------+

.. note:: For the ``USER`` type, when the number of simulated frames exceeds the
   number of frames contained in the files, the frames are replayed from the
   beginning of the file and this is repeated until the end of the simulation.

.. note:: For the ``USER_BIN`` type, raw data is read from the file that is open
   in binary mode. When the end of the file is reached, the source goes back to
   the beginning of the file and continues to generate bits from there.

.. _src-src-implem:

``--src-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Default: ``STD``
   :Examples: ``--src-implem FAST``

|factory::Source::p+implem|

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |src-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |src-implem_descr_fast| |
+----------+-------------------------+

.. |src-implem_descr_std|  replace:: Standard implementation working for any
   source type.
.. |src-implem_descr_fast| replace:: Fast implementation, only available for the
   ``RAND`` source type.

.. _src-src-path:

``--src-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--src-path conf/src/GSM-LDPC_2112.src``

|factory::Source::p+path|

With the ``USER_BIN`` source type, raw binary data is directly read from the
file.

With the ``USER`` source type, an |ASCII| file is expected:

.. code-block:: bash

   # 'F' has to be replaced by the number of contained frames.
   F

   # 'K' has to be replaced by the number of information bits.
   K

   # a sequence of 'F * K' bits (separated by spaces)
   B_0 B_1 B_2 B_3 B_4 B_5 [...] B_{(F*K)-1}

.. _src-src-no-reset:

``--src-no-reset``
""""""""""""""""""

|factory::Source::p+no-reset|

.. _src-src-fifo:

``--src-fifo``
""""""""""""""

|factory::Source::p+fifo|

.. _src-src-start-idx:

``--src-start-idx``
"""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--src-start-idx 42``

|factory::Source::p+start-idx|

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Src-
