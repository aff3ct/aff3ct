.. _enc-turbo-encoder-parameters:

Turbo Encoder parameters
------------------------

.. _enc-turbo-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Set the number of information bits :math:`K`.
The codeword size :math:`N` is automatically deduced:
:math:`N = 3 \times K + 4 \times \log_2(ts)` where :math:`ts` is the trellis
size.

.. _enc-turbo-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``TURBO`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``TURBO``
   :Examples: ``--enc-type AZCW``

Select the encoder type.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``TURBO`` | |enc-type_descr_turbo| |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_turbo| replace:: Select the standard Turbo encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

Please refer to the |RSC| :ref:`enc-rsc-enc-type` parameter.

.. _enc-turbo-enc-json-path:

``--enc-json-path``
"""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-json-path example/path/to/the/right/file``

Select the file path to dump the encoder and decoder internal values (in |JSON|
format). Those values can be observed with the dedicated `Turbo Code Reader`
available on the |AFF3CT| website: http://aff3ct.github.io/turbo_reader.html.

.. note:: Using this parameter will **slowdown considerably the encoder and
   decoder throughputs**.

.. _enc-turbo-enc-sub-no-buff:

``--enc-sub-no-buff``
"""""""""""""""""""""

Disable the buffered encoding.

**Without the buffered encoding**, considering the following sequence of
:math:`K` information bits: :math:`U_0, U_1, [...], U_{K-1}`, the encoded bits
will be organized as follow:
:math:`X_0^{sn}, X_0^{pn}, X_0^{pi}, [...], X_{K-1}^{sn}, X_{K-1}^{pn}, X_{K-1}^{pi}, X_{0}^{sn^t}, X_{0}^{pn^t}, [...], X_{\log_2(ts)-1}^{sn^t}, X_{\log_2(ts)-1}^{pn^t}, X_{0}^{si^t}, X_{0}^{pi^t}, [...], X_{\log_2(ts)-1}^{si^t}, X_{\log_2(ts)-1}^{pi^t}`,
where :math:`sn` and :math:`pn` are respectively *systematic* and *parity* bits
in the *natural domain*,  :math:`si` and :math:`pi` are respectively
*systematic* and *parity* bits in the *interleaved domain*, :math:`t` the
*tail bits* and and :math:`ts` the *trellis size*.

**With the buffered encoding**, considering the following sequence of :math:`K`
information bits: :math:`U_0, U_1, [...], U_{K-1}`, the encoded bits will be
organized as follow:
:math:`X_0^{sn}, [...], X_{K-1}^{sn}, X_{0}^{sn^t}, [...], X_{\log_2(ts)-1}^{sn^t}, X_0^{pn}, [...], X_{K-1}^{pn}, X_{0}^{pn^t}, [...], X_{\log_2(ts)-1}^{pn^t}, X_{0}^{si^t}, [...], X_{\log_2(ts)-1}^{si^t}, X_0^{pi}, [...], X_{K-1}^pi, X_{0}^{pi^t}, [...], X_{\log_2(ts)-1}^{pi^t}`,
where :math:`sn` and :math:`pn` are respectively *systematic* and *parity* bits
in the *natural domain*,  :math:`si` and :math:`pi` are respectively
*systematic* and *parity* bits in the *interleaved domain*, :math:`t` the
*tail bits* and and :math:`ts` the *trellis size*.

.. _enc-turbo-enc-sub-poly:

``--enc-sub-poly``
""""""""""""""""""

Please refer to the |RSC| :ref:`enc-rsc-enc-poly` parameter.

.. _enc-turbo-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``CCSDS`` ``LTE``
   :Examples: ``--enc-sub-std CCSDS``

Select a standard: set automatically some parameters (can be overwritten by user
given arguments).

Description of the allowed values:

+-----------+---------------------------+
| Value     | Description               |
+===========+===========================+
| ``CCSDS`` | |enc-sub-std_descr_ccsds| |
+-----------+---------------------------+
| ``LTE``   | |enc-sub-std_descr_lte|   |
+-----------+---------------------------+

.. |enc-sub-std_descr_ccsds| replace:: Set the :ref:`enc-turbo-enc-sub-poly`
   parameter to ``{023,033}`` according to the |CCSDS| standard (16-stage
   trellis) and select the |CCSDS| interleaver (see the :ref:`itl-itl-type`
   parameter).
.. |enc-sub-std_descr_lte| replace:: Set the :ref:`enc-turbo-enc-sub-poly`
   parameter to ``{013,015}`` according to the |LTE| standard (8-stage trellis)
   and select the |LTE| interleaver (see the :ref:`itl-itl-type` parameter).