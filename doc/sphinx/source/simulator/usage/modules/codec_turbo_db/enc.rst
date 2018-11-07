.. _enc-turbo_db-encoder-parameters:

Encoder parameters
------------------

.. _enc-turbo_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-turbo_db-enc-json-path:

``--enc-json-path``
"""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-json-path example/path/to/the/right/file``

Path to store the encoder and decoder traces formated in JSON.

.. _enc-turbo_db-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-turbo_db-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo_db-enc-sub-start-idx:

``--enc-sub-start-idx``
"""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-sub-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-turbo_db-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2``
   :Examples: ``--enc-sub-std DVB-RCS1``

Select a standard and set automatically some parameters (overwritten with user given arguments).

Description of the allowed values:

+--------------+------------------------------+
| Value        | Description                  |
+==============+==============================+
| ``DVB-RCS1`` | |enc-sub-std_descr_dvb-rcs1| |
+--------------+------------------------------+
| ``DVB-RCS2`` | |enc-sub-std_descr_dvb-rcs2| |
+--------------+------------------------------+

.. |enc-sub-std_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |enc-sub-std_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2


.. _enc-turbo_db-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``RSC_DB`` ``USER``
   :Examples: ``--enc-sub-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+------------+-----------------------------+
| Value      | Description                 |
+============+=============================+
| ``AZCW``   | |enc-sub-type_descr_azcw|   |
+------------+-----------------------------+
| ``COSET``  | |enc-sub-type_descr_coset|  |
+------------+-----------------------------+
| ``NO``     | |enc-sub-type_descr_no|     |
+------------+-----------------------------+
| ``RSC_DB`` | |enc-sub-type_descr_rsc_db| |
+------------+-----------------------------+
| ``USER``   | |enc-sub-type_descr_user|   |
+------------+-----------------------------+

.. |enc-sub-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-sub-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-sub-type_descr_no| replace:: TODO VALUE NO
.. |enc-sub-type_descr_rsc_db| replace:: TODO VALUE RSC_DB
.. |enc-sub-type_descr_user| replace:: TODO VALUE USER


.. _enc-turbo_db-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``TURBO_DB`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``AZCW``     | |enc-type_descr_azcw|     |
+--------------+---------------------------+
| ``COSET``    | |enc-type_descr_coset|    |
+--------------+---------------------------+
| ``NO``       | |enc-type_descr_no|       |
+--------------+---------------------------+
| ``TURBO_DB`` | |enc-type_descr_turbo_db| |
+--------------+---------------------------+
| ``USER``     | |enc-type_descr_user|     |
+--------------+---------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_turbo_db| replace:: TODO VALUE TURBO_DB
.. |enc-type_descr_user| replace:: TODO VALUE USER


