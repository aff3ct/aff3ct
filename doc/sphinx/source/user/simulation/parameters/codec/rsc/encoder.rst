.. _enc-rsc-encoder-parameters:

|RSC| Encoder parameters
------------------------

.. _enc-rsc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

**STRING**: |factory::Encoder::parameters::p+info-bits,K|

Set the number of information bits :math:`K`.
The codeword size :math:`N` is automatically deduced:
:math:`N = 2 \times (K + \log_2(ts))` where :math:`ts` is the trellis size.

.. _enc-rsc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``RSC`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``RSC``
   :Examples: ``--enc-type AZCW``

**STRING**: |factory::Encoder::parameters::p+type|

Select the encoder type.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``RSC``   | |enc-type_descr_rsc|   |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_rsc| replace:: Select the standard |RSC| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-rsc-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""

**STRING**: |factory::Encoder_RSC::parameters::p+no-buff|

Disable the buffered encoding.

**Without the buffered encoding**, considering the following sequence of
:math:`K` information bits: :math:`U_0, U_1, [...], U_{K-1}`, the encoded bits
will be organized as follow:
:math:`X_0^s, X_0^p, X_1^s, X_1^p, [...], X_{K-1}^s, X_{K-1}^p, X_{0}^{s^t}, X_{0}^{p^t}, X_{1}^{s^t}, X_{1}^{p^t}, [...], X_{\log_2(ts)-1}^{s^t}, X_{\log_2(ts)-1}^{p^t}`,
where :math:`s` and :math:`p` are respectively *systematic* and *parity* bits,
:math:`t` the *tail bits* and :math:`ts` the *trellis size*.

**With the buffered encoding**, considering the following sequence of :math:`K`
information bits: :math:`U_0, U_1, [...], U_{K-1}`, the encoded bits will be
organized as follow:
:math:`X_0^s, X_1^s, [...], X_{K-1}^s, X_{0}^{s^t}, X_{1}^{s^t}, [...], X_{\log_2(ts)-1}^{s^t}, X_0^p, X_1^p, [...], X_{K-1}^p, X_{0}^{p^t}, X_{1}^{p^t}, [...], X_{\log_2(ts)-1}^{p^t}`,
where :math:`s` and :math:`p` are respectively *systematic* and *parity* bits,
:math:`t` the *tail bits* and :math:`ts` the *trellis size*.

.. _enc-rsc-enc-poly:

``--enc-poly``
""""""""""""""

   :Type: text
   :Default: ``"{013,015}"``
   :Examples: ``--enc-poly "{023, 033}"``

**STRING**: |factory::Encoder_RSC::parameters::p+poly|

Set the polynomials that define the |RSC| code (or the trellis structure).
The expected form is :math:`\{A,B\}` where :math:`A` and :math:`B` are given in
octal.

.. _enc-rsc-enc-std:

``--enc-std``
"""""""""""""

   :Type: text
   :Allowed values: ``CCSDS`` ``LTE``
   :Examples: ``--enc-std CCSDS``

**STRING**: |factory::Encoder_RSC::parameters::p+std|

Select a standard: set automatically some parameters (can be overwritten by user
given arguments).

Description of the allowed values:

+-----------+-----------------------+
| Value     | Description           |
+===========+=======================+
| ``CCSDS`` | |enc-std_descr_ccsds| |
+-----------+-----------------------+
| ``LTE``   | |enc-std_descr_lte|   |
+-----------+-----------------------+

.. |enc-std_descr_ccsds| replace:: Set the :ref:`enc-rsc-enc-poly` parameter to
   ``{023,033}`` according to the |CCSDS| standard (16-stage trellis).
.. |enc-std_descr_lte| replace:: Set the :ref:`enc-rsc-enc-poly` parameter to
   ``{013,015}`` according to the |LTE| standard (8-stage trellis).