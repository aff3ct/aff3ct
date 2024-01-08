.. _dec-rsc-decoder-parameters:

|RSC| Decoder parameters
------------------------

.. _dec-rsc-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML``
   :Examples: ``--dec-type BCJR``

|factory::Decoder::p+type,D|

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``BCJR``  | Select the |BCJR| algorithm from :cite:`Bahl1974`.               |
+-----------+------------------------------------------------------------------+
| ``CHASE`` | See the common :ref:`dec-common-dec-type` parameter.             |
+-----------+------------------------------------------------------------------+
| ``ML``    | See the common :ref:`dec-common-dec-type` parameter.             |
+-----------+------------------------------------------------------------------+

.. _dec-rsc-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``GENERIC`` ``STD`` ``FAST`` ``VERY_FAST``
   :Default: ``STD``
   :Examples: ``--dec-implem FAST``

|factory::Decoder::p+implem|

Description of the allowed values:

+---------------+--------------------------------------------------------------+
| Value         | Description                                                  |
+===============+==============================================================+
| ``GENERIC``   | Select the generic |BCJR| implementation that can decode any |
|               | trellis (slow compared to the other implementations).        |
+---------------+--------------------------------------------------------------+
| ``STD``       | Select the |STD| |BCJR| implementation, specialized for the  |
|               | ``{013,015}`` polynomials (c.f. the :ref:`enc-rsc-enc-poly`  |
|               | parameter).                                                  |
+---------------+--------------------------------------------------------------+
| ``FAST``      | Select the fast |BCJR| implementation, specialized for the   |
|               | ``{013,015}`` polynomials (c.f. the :ref:`enc-rsc-enc-poly`  |
|               | parameter).                                                  |
+---------------+--------------------------------------------------------------+
| ``VERY_FAST`` | Select the very fast |BCJR| implementation,                  |
|               | specialized for the ``{013,015}`` polynomials (c.f. the      |
|               | :ref:`enc-rsc-enc-poly` parameter).                          |
+---------------+--------------------------------------------------------------+

.. _dec-rsc-dec-simd:

``--dec-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER`` ``INTRA``
   :Examples: ``--dec-simd INTER``

|factory::Decoder_RSC::p+simd|

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``INTER`` | Select the inter-frame strategy, only available for the |BCJR|   |
|           | ``STD``, ``FAST`` and ``VERY_FAST`` implementation (see          |
|           | :cite:`Cassagne2016a`).                                          |
+-----------+------------------------------------------------------------------+
| ``INTRA`` | Select the intra-frame strategy, only available for the |BCJR|   |
|           | ``STD`` and ``FAST`` implementations (see :cite:`Wu2013`).       |
+-----------+------------------------------------------------------------------+

.. note:: In **the intra-frame strategy**, |SIMD| units process several LLRs in
   parallel within a single frame decoding. In **the inter-frame strategy**,
   SIMD units decodes several independent frames in parallel in order to
   saturate the |SIMD| unit. This approach improves the throughput of the
   decoder but requires to load several frames before starting to decode,
   increasing both the decoding latency and the decoder memory footprint.

.. note:: When the inter-frame |SIMD| strategy is set, the simulator will run
   with the right number of frames depending on the |SIMD| length. This number
   of frames can be manually set with the :ref:`sim-sim-inter-fra` parameter. Be
   aware that running the simulator with the :ref:`sim-sim-inter-fra` parameter
   set to 1 and the :ref:`dec-polar-dec-simd` parameter set to ``INTER`` will
   completely be counterproductive and will lead to no throughput improvements.

.. _dec-rsc-dec-max:

``--dec-max``
"""""""""""""

   :Type: text
   :Allowed values: ``MAXS`` ``MAXL`` ``MAX``
   :Examples: ``--dec-max MAX``

|factory::Decoder_RSC::p+max|

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MAXS`` | |dec-max_descr_maxs| |
+----------+----------------------+
| ``MAXL`` | |dec-max_descr_maxl| |
+----------+----------------------+
| ``MAX``  | |dec-max_descr_max|  |
+----------+----------------------+

.. |dec-max_descr_maxs|  replace:: :math:`\max^*(a,b) = \max(a,b) +
   \log(1 + \exp(-|a - b|))`.
.. |dec-max_descr_maxl|  replace:: :math:`\max^*(a,b) \approx \max(a,b) +
   \max(0, 0.301 - (0.5 |a - b|))`.
.. |dec-max_descr_max|   replace:: :math:`\max^*(a,b) \approx \max(a,b)`.

``MAXS`` for *Max Star* is the exact :math:`\max^*` operator. ``MAXL`` for
*Max Linear* is a linear approximation of the :math:`\max^*` function. ``MAX``
for *Max* is the simplest :math:`\max^*` approximation with only a
:math:`\max` function.

.. note:: The |BCJR| with the :math:`\max` approximation is also called the
   max-log-|MAP| algorithm.

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Rsc-
