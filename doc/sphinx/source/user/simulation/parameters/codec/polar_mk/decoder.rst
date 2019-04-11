.. _dec-polar_mk-decoder-parameters:

Polar |MK| Decoder parameters
-----------------------------

.. _dec-polar_mk-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``SC`` ``CHASE`` ``ML``
   :Default: ``SC``
   :Examples: ``--dec-type ML``

|factory::Decoder::parameters::p+type,D|

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``SC``       | Select the original |SC| algorithm from :cite:`Arikan2009`.   |
+--------------+---------------------------------------------------------------+
| ``SCL``      | Select the |SCL| algorithm from :cite:`Tal2011`.              |
+--------------+---------------------------------------------------------------+
| ``CHASE``    | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+
| ``ML``       | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+

.. _dec-polar_mk-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE``
   :Default: ``NAIVE``
   :Examples: ``--dec-implem NAIVE``

|factory::Decoder::parameters::p+implem|

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+

.. |dec-implem_descr_naive| replace:: Select the naive implementation which is
   typically slow.

.. _dec-polar_mk-dec-lists:

``--dec-lists, -L``
"""""""""""""""""""

   :Type: integer
   :Default: ``8``
   :Examples: ``--dec-lists 1``

|factory::Decoder_polar_MK::parameters::p+lists,L|

References
""""""""""

.. bibliography:: references_dec.bib