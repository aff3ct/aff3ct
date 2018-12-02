.. _dec-ra-decoder-parameters:

|RA| Decoder parameters
-----------------------

.. _dec-ra-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``RA`` ``CHASE`` ``ML``
   :Examples: ``--dec-type CHASE``

Select the decoder algorithm.

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

.. |dec-type_descr_ra| replace:: Select the |RA| decoder based on the |MS|
   update rule in the |CNs|.
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

Select the algorithm implementation.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_std| replace:: Select the |STD| implementation.

.. _dec-ra-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

Set the number of iterations to perform in the decoder.