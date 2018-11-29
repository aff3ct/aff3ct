.. _dec-bch-decoder-parameters:

Decoder parameters
------------------

.. _dec-bch-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
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

.. |dec-type_descr_algebraic| replace:: Select the BCH algebraic decoder.
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-bch-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``GENIUS`` ``STD``
   :Examples: ``--dec-implem FAST``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``STD``    | |dec-implem_descr_std|    |
+------------+---------------------------+
| ``FAST``   | |dec-implem_descr_fast|   |
+------------+---------------------------+
| ``GENIUS`` | |dec-implem_descr_genius| |
+------------+---------------------------+

.. |dec-implem_descr_std| replace:: TODO VALUE STD
.. |dec-implem_descr_fast| replace:: TODO VALUE FAST
.. |dec-implem_descr_genius| replace:: TODO VALUE GENIUS

.. _dec-bch-dec-corr-pow:

``--dec-corr-pow, -T``
""""""""""""""""""""""

   :Type: integer
   :Examples: ``--dec-corr-pow 1``

Correction power of the BCH code.