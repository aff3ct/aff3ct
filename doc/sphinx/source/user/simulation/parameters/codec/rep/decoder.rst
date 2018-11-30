.. _dec-rep-decoder-parameters:

Repetition Decoder parameters
-----------------------------

.. _dec-rep-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``REPETITION`` ``CHASE`` ``ML``
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``REPETITION`` | |dec-type_descr_repetition| |
+----------------+-----------------------------+
| ``CHASE``      | |dec-type_descr_chase|      |
+----------------+-----------------------------+
| ``ML``         | |dec-type_descr_ml|         |
+----------------+-----------------------------+

.. |dec-type_descr_repetition| replace:: TODO VALUE REPETITION
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-rep-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE`` ``STD`` ``FAST``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+

.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_fast| replace:: TODO VALUE FAST