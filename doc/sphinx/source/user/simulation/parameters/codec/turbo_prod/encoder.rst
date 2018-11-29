.. _enc-turbo_prod-encoder-parameters:

Encoder parameters
------------------

.. _enc-turbo_prod-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``TURBO_PROD`` ``USER``
   :Default: ``TURBO_PROD``
   :Examples: ``--enc-type TURBO_PROD``

Set the type of the encoder to use in the simulation.

Description of the allowed values:

+----------------+-----------------------------+
| Value          | Description                 |
+================+=============================+
| ``TURBO_PROD`` | |enc-type_descr_turbo_prod| |
+----------------+-----------------------------+
| ``AZCW``       | |enc-type_descr_azcw|       |
+----------------+-----------------------------+
| ``COSET``      | |enc-type_descr_coset|      |
+----------------+-----------------------------+
| ``USER``       | |enc-type_descr_user|       |
+----------------+-----------------------------+

.. |enc-type_descr_turbo_prod| replace:: The |TPC| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.


.. _enc-turbo_prod-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``BCH`` ``COSET`` ``NO`` ``USER``
   :Default: ``BCH``
   :Examples: ``--enc-sub-type BCH``

Give the type of the sub-encoder to use to code each column and row.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``BCH``   | |enc-sub-type_descr_bch|   |
+-----------+----------------------------+
| ``AZCW``  | |enc-sub-type_descr_azcw|  |
+-----------+----------------------------+
| ``COSET`` | |enc-sub-type_descr_coset| |
+-----------+----------------------------+
| ``USER``  | |enc-sub-type_descr_user|  |
+-----------+----------------------------+

.. |enc-sub-type_descr_bch| replace:: See the |BCH| code
   :ref:`enc-bch-encoder-parameters` parameters.
.. |enc-sub-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-sub-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-sub-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo_prod-enc-sub-cw-size:

``--enc-sub-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-sub-cw-size 127``

Give the *sub-encoder code* codeword size. You can extend this codeword with a
parity bit with the :ref:`enc-turbo_prod-enc-ext` option.
Then the codeword size of the |TPC| is the square of this value.

.. _enc-turbo_prod-enc-sub-info-bits:

``--enc-sub-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-sub-info-bits 120``

Give the *sub-encoder code* input size (number of information bits).
Then the number of information bits of the |TPC| is the
square of this value.

.. _enc-turbo_prod-enc-ext:

``--enc-ext``
"""""""""""""

Extend the *sub-encoder* codeword with a parity bit in order to increase the
distance of the code.

.. TODO: Need to deactivate those options in the code

.. _enc-turbo_prod-enc-sub-path:

``--enc-sub-path``
""""""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-sub-path example/path/to/the/right/file``

Set the path to a file containing one or a set of pre-computed codewords
for the ``USER`` type sub-encoder.

.. _enc-turbo_prod-enc-sub-seed:

``--enc-sub-seed, -S``
""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-sub-seed 1``

Seed used to initialize the pseudo random generators in the sub-encoder.

.. _enc-turbo_prod-enc-sub-start-idx:

``--enc-sub-start-idx``
"""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-sub-start-idx 1``

Set the first frame index to use when reading the file in the ``USER`` type
sub-encoder.
