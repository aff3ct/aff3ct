.. _enc-ldpc-encoder-parameters:

Encoder parameters
------------------

.. _enc-ldpc-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1``

The codeword size.

.. _enc-ldpc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-ldpc-enc-g-method:

``--enc-g-method``
""""""""""""""""""

   :Type: text
   :Allowed values: ``IDENTITY`` ``LU_DEC``
   :Examples: ``--enc-g-method IDENTITY``

The method used to generate G from H when using 'LDPC_H' encoder type. 'LU_DEC' method generates a hollow G thanks to the LU decomposition with a guarantee to have the systematic identity (do not work with irregular matrices) when the 'IDENTITY' method generate an identity on H to get the parity part.

Description of the allowed values:

+--------------+-------------------------------+
| Value        | Description                   |
+==============+===============================+
| ``IDENTITY`` | |enc-g-method_descr_identity| |
+--------------+-------------------------------+
| ``LU_DEC``   | |enc-g-method_descr_lu_dec|   |
+--------------+-------------------------------+

.. |enc-g-method_descr_identity| replace:: TODO VALUE IDENTITY
.. |enc-g-method_descr_lu_dec| replace:: TODO VALUE LU_DEC


.. _enc-ldpc-enc-g-path:

``--enc-g-path``
""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-g-path example/path/to/the/right/file``

Path to the G matrix (AList formated file, required by the "LDPC" encoder).

.. _enc-ldpc-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-ldpc-enc-save-g:

``--enc-save-g``
""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-save-g example/path/to/the/right/file``

Path where the generated G matrix with the 'LDPC_H' encoder type will be saved.

.. _enc-ldpc-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-ldpc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``LDPC`` ``LDPC_DVBS2`` ``LDPC_H`` ``LDPC_IRA`` ``LDPC_QC`` ``NO`` ``USER``
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
| ``LDPC``       | |enc-type_descr_ldpc|       |
+----------------+-----------------------------+
| ``LDPC_DVBS2`` | |enc-type_descr_ldpc_dvbs2| |
+----------------+-----------------------------+
| ``LDPC_H``     | |enc-type_descr_ldpc_h|     |
+----------------+-----------------------------+
| ``LDPC_IRA``   | |enc-type_descr_ldpc_ira|   |
+----------------+-----------------------------+
| ``LDPC_QC``    | |enc-type_descr_ldpc_qc|    |
+----------------+-----------------------------+
| ``NO``         | |enc-type_descr_no|         |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_ldpc| replace:: TODO VALUE LDPC
.. |enc-type_descr_ldpc_dvbs2| replace:: TODO VALUE LDPC_DVBS2
.. |enc-type_descr_ldpc_h| replace:: TODO VALUE LDPC_H
.. |enc-type_descr_ldpc_ira| replace:: TODO VALUE LDPC_IRA
.. |enc-type_descr_ldpc_qc| replace:: TODO VALUE LDPC_QC
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_user| replace:: TODO VALUE USER


