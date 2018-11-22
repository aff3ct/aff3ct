.. _enc-rep-encoder-parameters:

Encoder parameters
------------------

.. _enc-rep-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1``

The codeword size.

.. _enc-rep-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-rep-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""


Disable the buffered encoding.

.. _enc-rep-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-rep-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-rep-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``REPETITION`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``AZCW``       | |enc-type_descr_azcw|       |
+----------------+-----------------------------+
| ``COSET``      | |enc-type_descr_coset|      |
+----------------+-----------------------------+
| ``NO``         | |enc-type_descr_no|         |
+----------------+-----------------------------+
| ``REPETITION`` | |enc-type_descr_repetition| |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_repetition| replace:: TODO VALUE REPETITION
.. |enc-type_descr_user| replace:: TODO VALUE USER


