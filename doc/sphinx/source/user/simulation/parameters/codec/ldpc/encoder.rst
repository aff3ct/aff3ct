.. _enc-ldpc-encoder-parameters:

|LDPC| Encoder parameters
-------------------------

.. _enc-ldpc-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1024``

|factory::Encoder::parameters::p+cw-size,N|

.. note:: This parameter value is automatically deduced if the :math:`H` parity
   matrix is given with the :ref:`dec-ldpc-dec-h-path` parameter or if the
   :math:`G` generator matrix is given with the :ref:`enc-ldpc-enc-g-path`
   parameter.

.. _enc-ldpc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 512``

|factory::Encoder::parameters::p+info-bits,K|

.. note:: This parameter value is automatically deduced if the :math:`G`
   generator matrix is given with the :ref:`enc-ldpc-enc-g-path` parameter.

.. note:: In some cases, this parameter value can be automatically deduced if
   the :math:`H` parity matrix is given with the :ref:`dec-ldpc-dec-h-path`
   parameter. For regular matrices, :math:`K = N - M` where :math:`N` and
   :math:`M` are the :math:`H` parity matrix dimensions. For *non-regular
   matrices*, :math:`K` has to be given.

.. _enc-ldpc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``LDPC`` ``LDPC_H`` ``LDPC_DVBS2`` ``LDPC_IRA``
                    ``LDPC_QC`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``AZCW``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::parameters::p+type|

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

.. |enc-type_descr_ldpc| replace:: Select the generic encoder that encode
   from a given :math:`G` generator matrix (to use with the
   :ref:`enc-ldpc-enc-g-path` parameter).
.. |enc-type_descr_ldpc_h| replace:: Build the  :math:`G` generator matrix from
   the given :math:`H` parity matrix and then encode with the ``LDPC`` method
   (to use with the :ref:`dec-ldpc-dec-h-path` parameter).
.. |enc-type_descr_ldpc_dvbs2| replace:: Select the optimized encoding process
   for the |DVB-S2| :math:`H` matrices (to use with the
   :ref:`enc-ldpc-enc-cw-size` and :ref:`enc-ldpc-enc-info-bits` parameters).
.. |enc-type_descr_ldpc_ira| replace:: Select the optimized encoding process for
   the |IRA| :math:`H` parity matrices (to use with the
   :ref:`dec-ldpc-dec-h-path` parameter).
.. |enc-type_descr_ldpc_qc| replace:: Select the optimized encoding process for
   the |QC| :math:`H` parity matrices (to use with the
   :ref:`dec-ldpc-dec-h-path` parameter).
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. note:: The ``LDPC_DVBS2`` encoder type allow the simulation of the
   |DVB-S2| standard but without the |BCH| code. All matrices described by
   `the standard (Tables 5a/5b page 22-23) <https://www.etsi.org/deliver/etsi_en/302300_302399/30230701/01.04.01_60/en_30230701v010401p.pdf>`_
   are available. You just need to give to the arguments
   :ref:`enc-ldpc-enc-info-bits` and :ref:`enc-ldpc-enc-cw-size` the real
   :math:`K` and :math:`N` |LDPC| dimensions, respectively.

.. _enc-ldpc-enc-g-path:

``--enc-g-path``
""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-g-path example/path/to/the/G_matrix.alist``

|factory::Encoder_LDPC::parameters::p+g-path|

.. _enc-ldpc-enc-g-method:

``--enc-g-method``
""""""""""""""""""

   :Type: text
   :Allowed values: ``IDENTITY`` ``LU_DEC``
   :Default: ``IDENTITY``
   :Examples: ``--enc-g-method IDENTITY``

|factory::Encoder_LDPC::parameters::p+g-method|

Description of the allowed values:

+--------------+-------------------------------+
| Value        | Description                   |
+==============+===============================+
| ``IDENTITY`` | |enc-g-method_descr_identity| |
+--------------+-------------------------------+
| ``LU_DEC``   | |enc-g-method_descr_lu_dec|   |
+--------------+-------------------------------+

.. _LU decomposition: https://en.wikipedia.org/wiki/LU_decomposition

.. |enc-g-method_descr_identity| replace:: Generate an identity on :math:`H` to
   get the parity part.
.. |enc-g-method_descr_lu_dec|   replace:: Generate a hollow :math:`G` thanks to
   the `LU decomposition`_ with a guarantee to have the systematic identity.
   Do not work with irregular matrices.

``LU_DEC`` method is faster than ``IDENTITY``.

.. _enc-ldpc-enc-g-save-path:

``--enc-g-save-path``
"""""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-g-save-path example/path/to/the/generated/G_matrix.alist``

|factory::Encoder_LDPC::parameters::p+g-save-path|

.. hint:: When running the ``LDPC_H`` encoder, the generation of the :math:`G`
   matrix can take a non-negligible part of the simulation time. With this
   option the :math:`G` matrix can be saved once for all and used in the
   standard ``LDPC`` decoder after.