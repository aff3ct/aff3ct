.. _enc-turbo_prod-encoder-parameters:

Encoder parameters
------------------

.. _enc-turbo_prod-enc-sub-cw-size:

``--enc-sub-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-sub-cw-size 1``

The codeword size.

.. _enc-turbo_prod-enc-sub-info-bits:

``--enc-sub-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-sub-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-turbo_prod-enc-ext:

``--enc-ext``
"""""""""""""


Extends code with a parity bits.

.. _enc-turbo_prod-enc-path:

``--enc-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-turbo_prod-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo_prod-enc-sub-path:

``--enc-sub-path``
""""""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--enc-sub-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-turbo_prod-enc-sub-seed:

``--enc-sub-seed, -S``
""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-sub-seed 1``

Seed used to initialize the pseudo random generators.

.. _enc-turbo_prod-enc-sub-start-idx:

``--enc-sub-start-idx``
"""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-sub-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo_prod-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``BCH`` ``COSET`` ``NO`` ``USER`` 
   :Examples: ``--enc-sub-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``AZCW``  | |enc-sub-type_descr_azcw|  |
+-----------+----------------------------+
| ``BCH``   | |enc-sub-type_descr_bch|   |
+-----------+----------------------------+
| ``COSET`` | |enc-sub-type_descr_coset| |
+-----------+----------------------------+
| ``NO``    | |enc-sub-type_descr_no|    |
+-----------+----------------------------+
| ``USER``  | |enc-sub-type_descr_user|  |
+-----------+----------------------------+

.. |enc-sub-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-sub-type_descr_bch| replace:: TODO VALUE BCH
.. |enc-sub-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-sub-type_descr_no| replace:: TODO VALUE NO
.. |enc-sub-type_descr_user| replace:: TODO VALUE USER


.. _enc-turbo_prod-enc-type:

``--enc-type``
""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``TURBO_PROD`` ``USER`` 
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
| ``TURBO_PROD`` | |enc-type_descr_turbo_prod| |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_turbo_prod| replace:: TODO VALUE TURBO_PROD
.. |enc-type_descr_user| replace:: TODO VALUE USER


