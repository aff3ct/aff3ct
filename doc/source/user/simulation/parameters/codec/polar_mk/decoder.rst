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
| ``SCL``      | Select the |SCL| algorithm from :cite:`Tal2011a`, also        |
|              | support the improved |CA|-|SCL| algorithm.                    |
+--------------+---------------------------------------------------------------+
| ``ASCL``     | Select the |A-SCL| algorithm from :cite:`Li2012b`, only the   |
|              | |PA-SCL| variant is available.                                |
+--------------+---------------------------------------------------------------+
| ``CHASE``    | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+
| ``ML``       | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+

At this time, the ``SC``, ``SCL`` and ``ASCL`` decoders support only a subset of
polar kernels listed below.

.. math::

   T2_{Arikan} =
   \begin{bmatrix}
      1 & 0 \\
      1 & 1
   \end{bmatrix}.

:math:`T2_{Arikan}` is the original :math:`2 \times 2` kernel proposed by Arikan
:cite:`Arikan2009a`. This matrix is invertible and can be used for systematic
encoding/decoding schemes.

.. math::

   T3_{Huawei1} =
   \begin{bmatrix}
      1 & 1 & 1 \\
      1 & 0 & 1 \\
      0 & 1 & 1
   \end{bmatrix}.

:math:`T3_{Huawei1}` is a :math:`3 \times 3` kernel proposed in
:cite:`Gabry2017`. This matrix is **not** invertible and **cannot** be used for
systematic encoding/decoding schemes.

.. math::

   T3_{Huawei2} =
   \begin{bmatrix}
      1 & 0 & 0 \\
      1 & 1 & 0 \\
      1 & 0 & 1
   \end{bmatrix}.

:math:`T3_{Huawei2}` is a :math:`3 \times 3` kernel proposed in
:cite:`Bioglio2018c`. This matrix is invertible and can be used for systematic
encoding/decoding schemes.

.. math::

   T4_{Huawei} =
   \begin{bmatrix}
      1 & 0 & 0 & 0 \\
      1 & 1 & 0 & 0 \\
      1 & 0 & 1 & 0 \\
      1 & 1 & 1 & 1
   \end{bmatrix}.

:math:`T4_{Huawei}` is a :math:`4 \times 4` kernel proposed in
:cite:`Bioglio2018c`. This matrix is invertible and can be used for systematic
encoding/decoding schemes.

.. math::

   T5_{Huawei} =
   \begin{bmatrix}
      1 & 0 & 0 & 0 & 0 \\
      1 & 1 & 0 & 0 & 0 \\
      1 & 0 & 1 & 0 & 0 \\
      1 & 0 & 0 & 1 & 0 \\
      1 & 1 & 1 & 0 & 1
   \end{bmatrix}.

:math:`T5_{Huawei}` is a :math:`5 \times 5` kernel proposed in
:cite:`Bioglio2018c`. This matrix is invertible and can be used for systematic
encoding/decoding schemes.

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
   :labelprefix: Pmkd-
