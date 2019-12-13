.. _dec-polar_mk-decoder-parameters:

Polar |MK| Decoder parameters
-----------------------------

.. _dec-polar_mk-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``SC`` ``SCL`` ``ASCL`` ``CHASE`` ``ML``
   :Default: ``SC``
   :Examples: ``--dec-type SCL``

|factory::Decoder::p+type,D|

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``SC``       | Select the original |SC| algorithm from :cite:`Arikan2009`.   |
+--------------+---------------------------------------------------------------+
| ``SCL``      | Select the |SCL| algorithm from :cite:`Tal2011`, also support |
|              | the improved |CA|-|SCL| algorithm.                            |
+--------------+---------------------------------------------------------------+
| ``ASCL``     | Select the |A-SCL| algorithm from :cite:`Li2012`, only the    |
|              | |PA-SCL| variant is available.                                |
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

|factory::Decoder::p+implem|

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

|factory::Decoder_polar_MK::p+lists,L|

.. _dec-polar_mk-dec-node-type:

``--dec-node-type``
"""""""""""""""""""

   :Type: text
   :Allowed values: ``MS`` ``SPA``
   :Default: ``MS``
   :Examples: ``--dec-node-type SPA``

|factory::Decoder_polar_MK::p+node-type|

Description of the allowed values:

+-----------+-----------------------+
| Value     | Description           |
+===========+=======================+
| ``MS``    | |dec-node-type_ms|    |
+-----------+-----------------------+
| ``SPA``   | |dec-node-type_spa|   |
+-----------+-----------------------+

.. |dec-node-type_ms|  replace::
  :math:`L_a \boxplus L_b \simeq
  \text{sign}(L_a).\text{sign}(L_b).\min(|L_a|,|L_b|)`.
.. |dec-node-type_spa| replace::
   :math:`L_a \boxplus L_b =
   2\tanh^{-1}(\tanh(\frac{L_a}{2}).\tanh(\frac{L_b}{2}))`.

References
""""""""""

.. bibliography:: references_dec.bib