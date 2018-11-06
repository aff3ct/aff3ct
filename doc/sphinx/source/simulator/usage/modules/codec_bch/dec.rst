.. _dec-bch-decoder-parameters:

Decoder parameters
------------------

.. _dec-bch-dec-corr-pow:

``--dec-corr-pow, -T``
""""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-corr-pow 1``

Correction power of the BCH code.

.. _dec-bch-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-bch-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-bch-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``GENIUS`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``FAST``   | |dec-implem_descr_fast|   |
+------------+---------------------------+
| ``GENIUS`` | |dec-implem_descr_genius| |
+------------+---------------------------+
| ``NAIVE``  | |dec-implem_descr_naive|  |
+------------+---------------------------+
| ``STD``    | |dec-implem_descr_std|    |
+------------+---------------------------+

.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_genius| replace:: TODO VALUE GENIUS
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-bch-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``ALGEBRAIC`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-type ALGEBRAIC``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+---------------+----------------------------+
| Value         | Description                |
+===============+============================+
| ``ALGEBRAIC`` | |dec-type_descr_algebraic| |
+---------------+----------------------------+
| ``CHASE``     | |dec-type_descr_chase|     |
+---------------+----------------------------+
| ``ML``        | |dec-type_descr_ml|        |
+---------------+----------------------------+

.. |dec-type_descr_algebraic| replace:: TODO VALUE ALGEBRAIC
.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML


