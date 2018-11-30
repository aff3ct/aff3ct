.. _enc-ldpc-encoder-parameters:

Encoder parameters
------------------

.. _enc-ldpc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``LDPC`` ``LDPC_H`` ``LDPC_DVBS2`` ``LDPC_IRA``
                    ``LDPC_QC`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``AZCW``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``LDPC``       | |enc-type_descr_ldpc|       |
+----------------+-----------------------------+
| ``LDPC_H``     | |enc-type_descr_ldpc_h|     |
+----------------+-----------------------------+
| ``LDPC_DVBS2`` | |enc-type_descr_ldpc_dvbs2| |
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

.. |enc-type_descr_ldpc|       replace:: Encode from a given generator matrix.
.. |enc-type_descr_ldpc_h|     replace:: Build the generator matrix from the
   given parity matrix and then encode with the ``LDPC`` method.
.. |enc-type_descr_ldpc_dvbs2| replace:: Optimized encoding from the chosen
   |DVB-S2| matrix. It is selected from its given dimensions.
.. |enc-type_descr_ldpc_ira|   replace:: Optimized encoding from a given
   **IRA** parity matrix.
.. |enc-type_descr_ldpc_qc|    replace:: Optimized encoding from a given
   **QC** parity matrix.
.. |enc-type_descr_azcw|       replace:: See the common
   :ref:`enc-common-enc-type` parameter.
.. |enc-type_descr_coset|      replace:: See the common
   :ref:`enc-common-enc-type` parameter.
.. |enc-type_descr_user|       replace:: See the common
   :ref:`enc-common-enc-type` parameter.


.. note:: The ``LDPC_DVBS2`` encoder type allow the simulation of the
   |DVB-S2| standard but without the |BCH| code. All matrices described by
   `the standard (Tables 5a/5b page 22/23) <https://www.etsi.org/deliver/etsi_en/302300_302399/30230701/01.04.01_60/en_30230701v010401p.pdf>`_
   are available. You just need to give to the arguments
   :ref:`enc-ldpc-enc-info-bits` and
   :ref:`enc-ldpc-enc-cw-size` the real :math:`K` and :math:`N` |LDPC|
   dimensions, respectively.

.. _enc-ldpc-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1024``

Set the codeword size. This argument is not required as the size is known
through the parity matrix given with :ref:`dec-ldpc-dec-h-path` or the
generator matrix given with :ref:`enc-ldpc-enc-g-path`.

.. _enc-ldpc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 512``

Give the useful number of information bits. This argument is not required as the
size is known through the generator matrix when given with
:ref:`enc-ldpc-enc-g-path`. If not it is calculated from
the parity matrix given with :ref:`dec-ldpc-dec-h-path`,
as :math:`K = N - M` where :math:`N` and :math:`M`
are the parity matrix dimensions, considering then a regular matrix. For
*non-regular matrices*, :math:`K` has to be given.

.. _enc-ldpc-enc-g-path:

``--enc-g-path``
""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-g-path example/path/to/the/G_matrix.alist``

Give the path to the generator matrix in a AList or Qc formated file.

.. _enc-ldpc-enc-g-method:

``--enc-g-method``
""""""""""""""""""

   :Type: text
   :Allowed values: ``IDENTITY`` ``LU_DEC``
   :Default: ``IDENTITY``
   :Examples: ``--enc-g-method IDENTITY``

Specify the method used to build the generator matrix from the parity matrix
when using the ``LDPC_H`` encoder type.

Description of the allowed values:

+--------------+-------------------------------+
| Value        | Description                   |
+==============+===============================+
| ``IDENTITY`` | |enc-g-method_descr_identity| |
+--------------+-------------------------------+
| ``LU_DEC``   | |enc-g-method_descr_lu_dec|   |
+--------------+-------------------------------+

.. _LU decomposition: https://en.wikipedia.org/wiki/LU_decomposition

.. |enc-g-method_descr_identity| replace:: Generate an identity on H to get the
   parity part
.. |enc-g-method_descr_lu_dec|   replace:: Generate a hollow G thanks to the
   `LU decomposition`_ with a guarantee to have the systematic identity.
   Do not work with irregular matrices.

``LU_DEC`` method is faster than ``IDENTITY``.

.. _enc-ldpc-enc-save-g:

``--enc-save-g``
""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-save-g example/path/to/the/generated/G_matrix.alist``

Give the filename where the built generator matrix with the ``LDPC_H`` encoder
is saved in an AList format.

.. hint:: It is useful to not have to generate again and again this matrix
   simulation after simulation. Use this option the first time but pay attention
   to generate it only on a single thread with :ref:`sim-sim-threads`
   to not write the file several times.
