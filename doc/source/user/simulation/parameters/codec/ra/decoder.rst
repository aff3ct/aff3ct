.. _dec-ra-decoder-parameters:

|RA| Decoder parameters
-----------------------

.. _dec-ra-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``RA`` ``CHASE`` ``ML``
   :Default: ``RA``
   :Examples: ``--dec-type CHASE``

|factory::Decoder::p+type,D|

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
   :Allowed values: ``STD``
   :Default: ``STD``
   :Examples: ``--dec-implem STD``

|factory::Decoder::p+implem|

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

|factory::Decoder_RA::p+ite,i|