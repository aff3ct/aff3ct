.. _dec-polar-decoder-parameters:

Polar Decoder parameters
------------------------

.. _dec-polar-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``SC`` ``SCAN`` ``SCL`` ``SCL_MEM`` ``ASCL`` ``ASCL_MEM``
                    ``CHASE`` ``ML``
   :Examples: ``--dec-type ASCL``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``SC``       | |dec-type_descr_sc|       |
+--------------+---------------------------+
| ``SCAN``     | |dec-type_descr_scan|     |
+--------------+---------------------------+
| ``SCL``      | |dec-type_descr_scl|      |
+--------------+---------------------------+
| ``SCL_MEM``  | |dec-type_descr_scl_mem|  |
+--------------+---------------------------+
| ``ASCL``     | |dec-type_descr_ascl|     |
+--------------+---------------------------+
| ``ASCL_MEM`` | |dec-type_descr_ascl_mem| |
+--------------+---------------------------+
| ``CHASE``    | |dec-type_descr_chase|    |
+--------------+---------------------------+
| ``ML``       | |dec-type_descr_ml|       |
+--------------+---------------------------+

.. |dec-type_descr_sc| replace:: TODO VALUE SC
.. |dec-type_descr_scan| replace:: TODO VALUE SCAN
.. |dec-type_descr_scl| replace:: TODO VALUE SCL
.. |dec-type_descr_scl_mem| replace:: TODO VALUE SCL_MEM
.. |dec-type_descr_ascl| replace:: TODO VALUE ASCL
.. |dec-type_descr_ascl_mem| replace:: TODO VALUE ASCL_MEM
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-polar-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE`` ``FAST``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+

.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_fast| replace:: TODO VALUE FAST

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

Enable the partial adaptive mode for the ASCL decoder (by default full adaptive
is selected).

.. _dec-polar-dec-polar-nodes:

``--dec-polar-nodes``
"""""""""""""""""""""

   :Type: text
   :Examples: ``--dec-polar-nodes "TODO CHECK VALUE"``

The type of nodes you want to detect in the Polar tree (ex:
"{R0,R1,R0L,REP_2-8,REPL,SPC_4+}").