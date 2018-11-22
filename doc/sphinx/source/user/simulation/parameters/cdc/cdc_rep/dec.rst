.. _dec-rep-decoder-parameters:

Decoder parameters
------------------

.. _dec-rep-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-rep-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-rep-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``NAIVE`` ``STD``
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
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-rep-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``CHASE`` ``ML`` ``REPETITION``
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``CHASE``      | |dec-type_descr_chase|      |
+----------------+-----------------------------+
| ``ML``         | |dec-type_descr_ml|         |
+----------------+-----------------------------+
| ``REPETITION`` | |dec-type_descr_repetition| |
+----------------+-----------------------------+

.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML
.. |dec-type_descr_repetition| replace:: TODO VALUE REPETITION


