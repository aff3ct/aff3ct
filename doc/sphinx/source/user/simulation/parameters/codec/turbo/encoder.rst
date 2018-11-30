.. _enc-turbo-encoder-parameters:

Turbo Encoder parameters
------------------------

.. _enc-turbo-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-turbo-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``TURBO`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``TURBO`` | |enc-type_descr_turbo| |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_turbo| replace:: TODO VALUE TURBO
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :Type: text
   :Allowed values: ``RSC`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-sub-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``RSC``   | |enc-sub-type_descr_rsc|   |
+-----------+----------------------------+
| ``AZCW``  | |enc-sub-type_descr_azcw|  |
+-----------+----------------------------+
| ``COSET`` | |enc-sub-type_descr_coset| |
+-----------+----------------------------+
| ``USER``  | |enc-sub-type_descr_user|  |
+-----------+----------------------------+

.. |enc-sub-type_descr_rsc| replace:: TODO VALUE RSC
.. |enc-sub-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-sub-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-sub-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo-enc-json-path:

``--enc-json-path``
"""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-json-path example/path/to/the/right/file``

Path to store the encoder and decoder traces formated in JSON.

.. _enc-turbo-enc-sub-no-buff:

``--enc-sub-no-buff``
"""""""""""""""""""""

Disable the buffered encoding.

.. _enc-turbo-enc-sub-poly:

``--enc-sub-poly``
""""""""""""""""""

   :Type: text
   :Examples: ``--enc-sub-poly "TODO CHECK VALUE"``

The polynomials describing RSC code, should be of the form "{A,B}".

.. _enc-turbo-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``CCSDS`` ``LTE``
   :Examples: ``--enc-sub-std CCSDS``

Select a standard and set automatically some parameters (overwritten with user
given arguments).

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