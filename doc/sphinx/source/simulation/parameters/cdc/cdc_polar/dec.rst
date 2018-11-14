.. _dec-polar-decoder-parameters:

Decoder parameters
------------------

.. _dec-polar-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-polar-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-polar-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``NAIVE``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+

.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE


.. _dec-polar-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the SCAN decoder.

.. _dec-polar-dec-lists:

``--dec-lists, -L``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-lists 1``

Maximal number of paths in the SCL decoder.

.. _dec-polar-dec-partial-adaptiv:

``--dec-partial-adaptiv``
"""""""""""""""""""""""""


Enable the partial adaptive mode for the ASCL decoder (by default full adaptive is selected).

.. _dec-polar-dec-polar-nodes:

``--dec-polar-nodes``
"""""""""""""""""""""

   :Type: text
   :Examples: ``--dec-polar-nodes "TODO CHECK VALUE"``

The type of nodes you want to detect in the Polar tree (ex: "{R0,R1,R0L,REP_2-8,REPL,SPC_4+}").

.. _dec-polar-dec-simd:

``--dec-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER`` ``INTRA``
   :Examples: ``--dec-simd INTER``

The |SIMD| strategy you want to use.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``INTER`` | |dec-simd_descr_inter| |
+-----------+------------------------+
| ``INTRA`` | |dec-simd_descr_intra| |
+-----------+------------------------+

.. |dec-simd_descr_inter| replace:: TODO VALUE INTER
.. |dec-simd_descr_intra| replace:: TODO VALUE INTRA


.. _dec-polar-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``ASCL`` ``ASCL_MEM`` ``CHASE`` ``ML`` ``SC`` ``SCAN`` ``SCL`` ``SCL_MEM``
   :Examples: ``--dec-type ASCL``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``ASCL``     | |dec-type_descr_ascl|     |
+--------------+---------------------------+
| ``ASCL_MEM`` | |dec-type_descr_ascl_mem| |
+--------------+---------------------------+
| ``CHASE``    | |dec-type_descr_chase|    |
+--------------+---------------------------+
| ``ML``       | |dec-type_descr_ml|       |
+--------------+---------------------------+
| ``SC``       | |dec-type_descr_sc|       |
+--------------+---------------------------+
| ``SCAN``     | |dec-type_descr_scan|     |
+--------------+---------------------------+
| ``SCL``      | |dec-type_descr_scl|      |
+--------------+---------------------------+
| ``SCL_MEM``  | |dec-type_descr_scl_mem|  |
+--------------+---------------------------+

.. |dec-type_descr_ascl| replace:: TODO VALUE ASCL
.. |dec-type_descr_ascl_mem| replace:: TODO VALUE ASCL_MEM
.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML
.. |dec-type_descr_sc| replace:: TODO VALUE SC
.. |dec-type_descr_scan| replace:: TODO VALUE SCAN
.. |dec-type_descr_scl| replace:: TODO VALUE SCL
.. |dec-type_descr_scl_mem| replace:: TODO VALUE SCL_MEM


