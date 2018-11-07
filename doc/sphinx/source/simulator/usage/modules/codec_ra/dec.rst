.. _dec-ra-decoder-parameters:

Decoder parameters
------------------

.. _dec-ra-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-ra-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-ra-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE`` ``STD``
   :Examples: ``--dec-implem NAIVE``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-ra-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the decoder.

.. _dec-ra-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``CHASE`` ``ML`` ``RA``
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
| ``RA``    | |dec-type_descr_ra|    |
+-----------+------------------------+

.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML
.. |dec-type_descr_ra| replace:: TODO VALUE RA


