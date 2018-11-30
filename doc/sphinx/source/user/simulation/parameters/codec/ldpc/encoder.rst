.. _enc-ldpc-encoder-parameters:

|LDPC| Encoder parameters
-------------------------

.. _enc-ldpc-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1024``

The codeword size.

.. _enc-ldpc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 512``

Useful number of bit transmitted (information bits).

.. _enc-ldpc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``LDPC`` ``LDPC_DVBS2`` ``LDPC_H`` ``LDPC_IRA`` ``LDPC_QC``
                    ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
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
| ``AZCW``       | |enc-type_descr_azcw|       |
+----------------+-----------------------------+
| ``COSET``      | |enc-type_descr_coset|      |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_ldpc| replace:: TODO VALUE LDPC
.. |enc-type_descr_ldpc_dvbs2| replace:: TODO VALUE LDPC_DVBS2
.. |enc-type_descr_ldpc_h| replace:: TODO VALUE LDPC_H
.. |enc-type_descr_ldpc_ira| replace:: TODO VALUE LDPC_IRA
.. |enc-type_descr_ldpc_qc| replace:: TODO VALUE LDPC_QC
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-ldpc-enc-g-method:

``--enc-g-method``
""""""""""""""""""

   :Type: text
   :Allowed values: ``IDENTITY`` ``LU_DEC``
   :Examples: ``--enc-g-method IDENTITY``

The method used to generate G from H when using 'LDPC_H' encoder type. 'LU_DEC'
method generates a hollow G thanks to the LU decomposition with a guarantee to
have the systematic identity (do not work with irregular matrices) when the
'IDENTITY' method generate an identity on H to get the parity part.

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

.. _enc-ldpc-enc-save-g:

``--enc-save-g``
""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-save-g example/path/to/the/right/file``

Path where the generated G matrix with the 'LDPC_H' encoder type will be saved.