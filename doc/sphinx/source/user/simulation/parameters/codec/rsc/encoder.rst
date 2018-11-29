.. _enc-rsc-encoder-parameters:

Encoder parameters
------------------

.. _enc-rsc-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-rsc-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""


Disable the buffered encoding.

.. _enc-rsc-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-rsc-enc-poly:

``--enc-poly``
""""""""""""""

   :Type: text
   :Examples: ``--enc-poly "TODO CHECK VALUE"``

The polynomials describing RSC code, should be of the form "{A,B}".

.. _enc-rsc-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-rsc-enc-std:

``--enc-std``
"""""""""""""

   :Type: text
   :Allowed values: ``CCSDS`` ``LTE``
   :Examples: ``--enc-std CCSDS``

Select a standard and set automatically some parameters (overwritten with user given arguments).

Description of the allowed values:

+-----------+-----------------------+
| Value     | Description           |
+===========+=======================+
| ``CCSDS`` | |enc-std_descr_ccsds| |
+-----------+-----------------------+
| ``LTE``   | |enc-std_descr_lte|   |
+-----------+-----------------------+

.. |enc-std_descr_ccsds| replace:: TODO VALUE CCSDS
.. |enc-std_descr_lte| replace:: TODO VALUE LTE


.. _enc-rsc-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``RSC`` ``USER``
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
| ``RSC``   | |enc-type_descr_rsc|   |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_rsc| replace:: TODO VALUE RSC
.. |enc-type_descr_user| replace:: TODO VALUE USER


