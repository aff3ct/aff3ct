.. _dec-uncoded-decoder-parameters:

Uncoded Decoder parameters
--------------------------

.. _dec-uncoded-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``NONE`` ``CHASE`` ``ML``
   :Default: ``NONE``
   :Examples: ``--dec-type CHASE``

**STRING**: |factory::Decoder::parameters::p+type,D|

Select the decoder algorithm.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``NONE``  | |dec-type_descr_none|  |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_none| replace:: Select the ``NONE`` decoder.
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   arameter.

.. _dec-uncoded-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``HARD_DECISION``
   :default: ``HARD_DECISION``
   :Examples: ``--dec-implem HARD_DECISION``

**STRING**: |factory::Decoder::parameters::p+implem|

Select the decoder implementation.

Description of the allowed values:

+-------------------+----------------------------------+
| Value             | Description                      |
+===================+==================================+
| ``HARD_DECISION`` | |dec-implem_descr_hard_decision| |
+-------------------+----------------------------------+

.. |dec-implem_descr_hard_decision| replace:: Take the hard decision on the
   input |LLRs|.