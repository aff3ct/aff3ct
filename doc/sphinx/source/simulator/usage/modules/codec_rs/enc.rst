.. _enc-rs-encoder-parameters:

Encoder parameters
------------------

.. _enc-rs-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-cw-size 1``

The codeword size.

.. _enc-rs-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-rs-enc-path:

``--enc-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-rs-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-rs-enc-type:

``--enc-type``
""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``RS`` ``USER`` 
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``NO``    | |enc-type_descr_no|    |
+-----------+------------------------+
| ``RS``    | |enc-type_descr_rs|    |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_rs| replace:: TODO VALUE RS
.. |enc-type_descr_user| replace:: TODO VALUE USER


