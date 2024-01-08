.. _enc-polar_mk-encoder-parameters:

Polar |MK| Encoder parameters
-----------------------------

.. _enc-polar_mk-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``POLAR_MK`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``POLAR_MK``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::p+type|

Description of the allowed values:

+--------------+------------------------+
| Value        | Description            |
+==============+========================+
| ``POLAR_MK`` | |enc-type_descr_polar| |
+--------------+------------------------+
| ``AZCW``     | |enc-type_descr_azcw|  |
+--------------+------------------------+
| ``COSET``    | |enc-type_descr_coset| |
+--------------+------------------------+
| ``USER``     | |enc-type_descr_user|  |
+--------------+------------------------+

.. |enc-type_descr_polar| replace:: Select the standard Polar |MK| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-polar_mk-enc-no-sys:

``--enc-sys``
"""""""""""""

|factory::Encoder_polar_MK::p+sys|

.. _enc-polar_mk-enc-pc-kernel:

``--enc-pc-kernel``
"""""""""""""""""""

   :Type: list of (list of (boolean:including set={0|1}):limited length [1;inf]), elements of same length
   :Default: ``"10,11"``
   :Examples: ``--enc-pc-kernel "111,101,011"``

|factory::Polar_code::p+kernel|

The Kronecker powers of the kernel matrix :math:`T_s` defines the :math:`G`
generator matrix. The frame size :math:`N` can only be a power of the matrix
size :math:`s`. For traditional Polar codes :math:`s = 2`.

.. note:: This parameter can only be used to define mono-kernel polar codes.

.. _enc-polar_mk-enc-pc-path:

``--enc-pc-path``
"""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-pc-path example/path/to/the/right/place/``

|factory::Polar_code::p+path|

An |ASCII| file is expected:

.. code-block:: bash

   # number of different kernels
   2

   # dimension of kernel 0
   2
   # kernel 0
   1 0
   1 1

   # dimension of kernel 1
   3
   # kernel 1
   1 1 1
   1 0 1
   0 1 1

   # number of stages
   5
   # type of kernel per stage
   0 0 0 0 1

The previous file describes a Polar |MK| code which is build from the following
Kronecker product:
:math:`G_{48} = T_2 \otimes T_2 \otimes T_2 \otimes T_2 \otimes T_3` with
:math:`N = 2 \times 2 \times 2 \times 2 \times 3 = 48.`

.. note:: When this parameter is used it overrides the
   :ref:`enc-polar_mk-enc-pc-kernel` parameter.

.. note:: This parameter is the only way to define multi-kernel polar codes.

.. _enc-polar_mk-enc-fb-gen-method:

``--enc-fb-gen-method``
"""""""""""""""""""""""

   :Type: text
   :Default: ``GA``
   :Allowed values: ``GA`` ``GAA`` ``FILE``
   :Examples: ``--enc-fb-gen-method FILE``

|factory::Frozenbits_generator_MK::p+gen-method|

Description of the allowed values:

+----------+-------------------------------------------------------------------+
| Value    | Description                                                       |
+==========+===================================================================+
| ``GA``   | Select the |GA| method from :cite:`Trifonov2012a`.                |
+----------+-------------------------------------------------------------------+
| ``GAA``  | Select the |GA| method from :cite:`Trifonov2012a` (works only for |
|          | Arikan mono-kernel polar codes).                                  |
+----------+-------------------------------------------------------------------+
| ``FILE`` | Read the best channels from an external file, to use with the     |
|          | :ref:`enc-polar_mk-enc-fb-awgn-path` parameter.                   |
+----------+-------------------------------------------------------------------+

.. note:: When using the ``FILE`` method, the frozen bits are always the same
   regardless of the |SNR| value.

.. _enc-polar_mk-enc-fb-awgn-path:

``--enc-fb-awgn-path``
""""""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-fb-awgn-path example/path/to/the/right/place/``

|factory::Frozenbits_generator_MK::p+awgn-path|

An |ASCII| file is expected, for instance, the following file describes the
most reliable channels optimized for a codeword of size :math:`N = 8` and for an
|AWGN| channel where the noise variance is :math:`\sigma = 0.435999`:

.. code-block:: bash

   8
   awgn
   0.435999
   7 6 5 3 4 2 1 0

Given the previous file, if we suppose a Polar code of size :math:`N = 8` with
:math:`K = 4` information bits, the frozen bits are at the ``0, 1, 2, 4``
positions in the codeword. The strategy is to freeze the less reliable channels.

.. warning:: The ``FILE`` frozen bits generator expects a file and not a
   directory.

.. _enc-polar_mk-enc-fb-dump-path:

``--enc-fb-dump-path``
""""""""""""""""""""""

   :Type: folder
   :Rights: write only
   :Examples: ``--enc-fb-dump-path example/path/to/the/right/place/``

|factory::Frozenbits_generator_MK::p+dump-path|

.. note:: Works only for the ``GA`` and ``GAA`` frozen bits generation methods.

.. _enc-polar_mk-enc-fb-noise:

``--enc-fb-noise``
""""""""""""""""""

   :Type: real number
   :Examples: ``--enc-fb-noise 1.0``

|factory::Frozenbits_generator_MK::p+noise|

References
""""""""""

.. bibliography:: references_enc.bib
   :labelprefix: Pmke-
