.. _enc-turbo-encoder-parameters:

Encoder parameters
------------------

.. _enc-turbo-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-turbo-enc-json-path:

``--enc-json-path``
"""""""""""""""""""

   :type: file
   :Rights: write only
   :Examples: ``--enc-json-path example/path/to/the/right/file``

Path to store the encoder and decoder traces formated in JSON.

.. _enc-turbo-enc-path:

``--enc-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-turbo-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo-enc-sub-no-buff:

``--enc-sub-no-buff``
"""""""""""""""""""""


Disable the buffered encoding.

.. _enc-turbo-enc-sub-poly:

``--enc-sub-poly``
""""""""""""""""""

   :type: text
   :Examples: ``--enc-sub-poly "TODO CHECK VALUE"``

The polynomials describing RSC code, should be of the form "{A,B}".

.. _enc-turbo-enc-sub-start-idx:

``--enc-sub-start-idx``
"""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-sub-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :type: text
   :Allowed values: ``CCSDS`` ``LTE`` 
   :Examples: ``--enc-sub-std CCSDS``

Select a standard and set automatically some parameters (overwritten with user given arguments).

Description of the allowed values:

+-----------+---------------------------+
| Value     | Description               |
+===========+===========================+
| ``CCSDS`` | |enc-sub-std_descr_ccsds| |
+-----------+---------------------------+
| ``LTE``   | |enc-sub-std_descr_lte|   |
+-----------+---------------------------+

.. |enc-sub-std_descr_ccsds| replace:: TODO VALUE CCSDS
.. |enc-sub-std_descr_lte| replace:: TODO VALUE LTE


.. _enc-turbo-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``RSC`` ``USER`` 
   :Examples: ``--enc-sub-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``AZCW``  | |enc-sub-type_descr_azcw|  |
+-----------+----------------------------+
| ``COSET`` | |enc-sub-type_descr_coset| |
+-----------+----------------------------+
| ``NO``    | |enc-sub-type_descr_no|    |
+-----------+----------------------------+
| ``RSC``   | |enc-sub-type_descr_rsc|   |
+-----------+----------------------------+
| ``USER``  | |enc-sub-type_descr_user|  |
+-----------+----------------------------+

.. |enc-sub-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-sub-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-sub-type_descr_no| replace:: TODO VALUE NO
.. |enc-sub-type_descr_rsc| replace:: TODO VALUE RSC
.. |enc-sub-type_descr_user| replace:: TODO VALUE USER


.. _enc-turbo-enc-type:

``--enc-type``
""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``TURBO`` ``USER`` 
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
| ``TURBO`` | |enc-type_descr_turbo| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_turbo| replace:: TODO VALUE TURBO
.. |enc-type_descr_user| replace:: TODO VALUE USER


