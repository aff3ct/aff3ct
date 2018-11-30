.. _dec-rsc-decoder-parameters:

|RSC| Decoder parameters
------------------------

.. _dec-rsc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
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
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.


.. _dec-rsc-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``GENERIC`` ``STD`` ``FAST`` ``VERY_FAST``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+---------------+------------------------------+
| Value         | Description                  |
+===============+==============================+
| ``GENERIC``   | |dec-implem_descr_generic|   |
+---------------+------------------------------+
| ``STD``       | |dec-implem_descr_std|       |
+---------------+------------------------------+
| ``FAST``      | |dec-implem_descr_fast|      |
+---------------+------------------------------+
| ``VERY_FAST`` | |dec-implem_descr_very_fast| |
+---------------+------------------------------+

.. |dec-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_very_fast| replace:: TODO VALUE VERY_FAST

.. _dec-rsc-dec-simd:

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

.. _dec-rsc-dec-max:

``--dec-max``
"""""""""""""

   :Type: text
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