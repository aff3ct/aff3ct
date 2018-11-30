.. _dec-ra-decoder-parameters:

|RA| Decoder parameters
-----------------------

.. _dec-ra-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``RA`` ``CHASE`` ``ML``
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``RA``    | |dec-type_descr_ra|    |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_ra| replace:: TODO VALUE RA
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

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
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_std| replace:: TODO VALUE STD

.. _dec-ra-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the decoder.