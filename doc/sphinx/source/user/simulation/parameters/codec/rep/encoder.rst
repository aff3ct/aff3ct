.. _enc-rep-encoder-parameters:

Repetition Encoder parameters
-----------------------------

.. _enc-rep-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1``

|factory::Encoder::parameters::p+cw-size,N|

:math:`N` has to be divisible by `K`.

.. _enc-rep-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

|factory::Encoder::parameters::p+info-bits,K|

.. _enc-rep-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``REP`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::parameters::p+type|

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``REP``        | |enc-type_descr_repetition| |
+----------------+-----------------------------+
| ``AZCW``       | |enc-type_descr_azcw|       |
+----------------+-----------------------------+
| ``COSET``      | |enc-type_descr_coset|      |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_repetition| replace:: Select the standart repetition decoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-rep-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""

|factory::Encoder_repetition::parameters::p+no-buff|

**Without the buffered encoding**, considering :math:`K` information bits
:math:`U_0, U_1, [...], U_{K-1}`, the corresponding sequence of bits in the
codeword is organized as follow:
:math:`X_0^0, X_0^1, [...], X_0^{rep-1}, X_1^0, X_1^1, [...], X_1^{rep-1}, [[...]], X_{K-1}^0, X_{K-1}^1, [...], X_{K-1}^{rep-1},`
with :math:`rep = N / K.`

**With the buffered encoding**, considering :math:`K` information bits
:math:`U_0, U_1, [...], U_{K-1}`, the corresponding sequence of bits in the
codeword is organized as follow:
:math:`X_0^0, X_1^0, [...], X_{K-1}^0, X_0^1, X_1^1, [...], X_{K-1}^1, [[...]], X_0^{rep-1}, X_1^{rep-1}, [...], X_{K-1}^{rep-1},`
with :math:`rep = N / K.`