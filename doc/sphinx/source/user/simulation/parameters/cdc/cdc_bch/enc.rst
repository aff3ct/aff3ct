.. _enc-bch-encoder-parameters:

Encoder parameters
------------------

.. _enc-bch-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1``

The codeword size.

.. _enc-bch-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-bch-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-bch-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-bch-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``BCH`` ``COSET`` ``NO`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``BCH``   | |enc-type_descr_bch|   |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``NO``    | |enc-type_descr_no|    |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_bch| replace:: TODO VALUE BCH
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_user| replace:: TODO VALUE USER


