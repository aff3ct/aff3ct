.. _enc-bch-encoder-parameters:

|BCH| Encoder parameters
------------------------

.. _enc-bch-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 127``

|factory::Encoder::p+cw-size,N|

:math:`N = 2^m – 1`, where :math:`m` is an integer from 3.

.. _enc-bch-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 92``

|factory::Encoder::p+info-bits,K|

This argument is not required if :ref:`dec-bch-dec-corr-pow` is given, as it
is calculated automatically.

.. _enc-bch-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BCH`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``BCH``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::p+type|

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``BCH``   | |enc-type_descr_bch|   |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_bch| replace:: Select the standard |BCH| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-bch-enc-simd:

``--enc-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER``
   :Examples: ``--enc-simd INTER``

|factory::Encoder_BCH::p+simd|

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``INTER`` | |dec-simd_descr_inter| |
+-----------+------------------------+

.. |dec-simd_descr_inter| replace:: Select the inter-frame strategy.

.. note:: Be aware that running the simulator with the :ref:`src-src-fra`
   parameter set to 1 and the :ref:`dec-polar-dec-simd` parameter set to
   ``INTER`` will completely be counterproductive and will lead to no throughput
   improvements.