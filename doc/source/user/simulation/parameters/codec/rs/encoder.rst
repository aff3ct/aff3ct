.. _enc-rs-encoder-parameters:

|RS| Encoder parameters
-----------------------

.. _enc-rs-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 127``

|factory::Encoder::p+cw-size,N|

:math:`N = 2^m – 1`, where :math:`m` is an integer from 3 that represents also
the number of bits per symbol. Thus, the binary codeword size is
:math:`N \times m`.

.. _enc-rs-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

|factory::Encoder::p+info-bits,K|

This argument is not required if the correction power :math:`T` is given with
:ref:`dec-rs-dec-corr-pow`, as it
is calculated automatically with the formula :math:`K = N - 2.T`.

.. _enc-rs-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``RS`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``RS``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::p+type|

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``RS``    | |enc-type_descr_rs|    |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_rs| replace:: Select the standard |RS| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.


