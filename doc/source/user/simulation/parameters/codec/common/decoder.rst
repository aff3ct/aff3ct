.. _dec-common-decoder-parameters:

Common Decoder parameters
-------------------------

This section describes the parameters common to all decoders.

.. _dec-common-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``CHASE`` ``ML``
   :Examples: ``--dec-type ML``

|factory::Decoder::p+type,D|

Description of the allowed values:

+---------------+--------------------------------------------------------------+
| Value         | Description                                                  |
+===============+==============================================================+
| ``CHASE``     | Select the Chase decoder from :cite:`Chase1972`.             |
+---------------+--------------------------------------------------------------+
| ``ML``        | Select the perfect |ML| decoder.                             |
+---------------+--------------------------------------------------------------+

.. note:: The Chase and the |ML| decoders have a very high computationnal
   complexity and cannot be use for large frames.

.. _dec-common-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE`` ``STD``
   :Examples: ``--dec-implem STD``

|factory::Decoder::p+implem|

Description of the allowed values:

+------------+---------------------------+
| Value      | Description               |
+============+===========================+
| ``NAIVE``  | |dec-implem_descr_naive|  |
+------------+---------------------------+
| ``STD``    | |dec-implem_descr_std|    |
+------------+---------------------------+

.. |dec-implem_descr_naive| replace:: Select the naive implementation (very
   slow and only available for the |ML| decoder).
.. |dec-implem_descr_std| replace:: Select the standard implementation.

.. _dec-common-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

|factory::Decoder::p+flips|

.. note:: Used in the Chase decoding algorithm.

.. _dec-common-dec-hamming:

``--dec-hamming``
"""""""""""""""""

.. _Hamming distance: https://en.wikipedia.org/wiki/Hamming_distance
.. _Euclidean distance: https://en.wikipedia.org/wiki/Euclidean_distance

|factory::Decoder::p+hamming|

.. note:: Using the `Hamming distance`_ will heavily degrade the |BFER|
   performances. The |BFER| performances will be the same as an hard input
   decoder.

.. _dec-common-dec-seed:

``--dec-seed``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-seed 1``

|factory::Decoder::p+seed|

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Cmn-
