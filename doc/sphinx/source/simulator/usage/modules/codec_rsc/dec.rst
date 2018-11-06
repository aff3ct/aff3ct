.. _dec-rsc-decoder-parameters:

Decoder parameters
------------------

.. _dec-rsc-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-rsc-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-rsc-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``GENERIC`` ``NAIVE`` ``STD`` ``VERY_FAST`` 
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+---------------+------------------------------+
| Value         | Description                  |
+===============+==============================+
| ``FAST``      | |dec-implem_descr_fast|      |
+---------------+------------------------------+
| ``GENERIC``   | |dec-implem_descr_generic|   |
+---------------+------------------------------+
| ``NAIVE``     | |dec-implem_descr_naive|     |
+---------------+------------------------------+
| ``STD``       | |dec-implem_descr_std|       |
+---------------+------------------------------+
| ``VERY_FAST`` | |dec-implem_descr_very_fast| |
+---------------+------------------------------+

.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_very_fast| replace:: TODO VALUE VERY_FAST


.. _dec-rsc-dec-max:

``--dec-max``
"""""""""""""

   :type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS`` 
   :Examples: ``--dec-max MAX``

The MAX implementation for the nodes.

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MAX``  | |dec-max_descr_max|  |
+----------+----------------------+
| ``MAXL`` | |dec-max_descr_maxl| |
+----------+----------------------+
| ``MAXS`` | |dec-max_descr_maxs| |
+----------+----------------------+

.. |dec-max_descr_max| replace:: TODO VALUE MAX
.. |dec-max_descr_maxl| replace:: TODO VALUE MAXL
.. |dec-max_descr_maxs| replace:: TODO VALUE MAXS


.. _dec-rsc-dec-simd:

``--dec-simd``
""""""""""""""

   :type: text
   :Allowed values: ``INTER`` ``INTRA`` 
   :Examples: ``--dec-simd INTER``

The SIMD strategy you want to use.

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


.. _dec-rsc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-type BCJR``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``BCJR``  | |dec-type_descr_bcjr|  |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_bcjr| replace:: TODO VALUE BCJR
.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML


