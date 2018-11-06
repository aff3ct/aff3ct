.. _dec-uncoded-decoder-parameters:

Decoder parameters
------------------

.. _dec-uncoded-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-uncoded-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-uncoded-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``HARD_DECISION`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-implem HARD_DECISION``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-------------------+----------------------------------+
| Value             | Description                      |
+===================+==================================+
| ``HARD_DECISION`` | |dec-implem_descr_hard_decision| |
+-------------------+----------------------------------+
| ``NAIVE``         | |dec-implem_descr_naive|         |
+-------------------+----------------------------------+
| ``STD``           | |dec-implem_descr_std|           |
+-------------------+----------------------------------+

.. |dec-implem_descr_hard_decision| replace:: TODO VALUE HARD_DECISION
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-uncoded-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``CHASE`` ``ML`` ``NONE`` 
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+
| ``NONE``  | |dec-type_descr_none|  |
+-----------+------------------------+

.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML
.. |dec-type_descr_none| replace:: TODO VALUE NONE


