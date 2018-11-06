.. _enc-rsc_db-encoder-parameters:

Encoder parameters
------------------

.. _enc-rsc_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-rsc_db-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""


Disable the buffered encoding.

.. _enc-rsc_db-enc-path:

``--enc-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed codewords, to use with "\\-\\-enc-type USER".

.. _enc-rsc_db-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :type: integer
   :Examples: ``--enc-start-idx 1``

Start idx to use in the USER type encoder.

.. _enc-rsc_db-enc-std:

``--enc-std``
"""""""""""""

   :type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2`` 
   :Examples: ``--enc-std DVB-RCS1``

Select a standard and set automatically some parameters (overwritten with user given arguments).

Description of the allowed values:

+--------------+--------------------------+
| Value        | Description              |
+==============+==========================+
| ``DVB-RCS1`` | |enc-std_descr_dvb-rcs1| |
+--------------+--------------------------+
| ``DVB-RCS2`` | |enc-std_descr_dvb-rcs2| |
+--------------+--------------------------+

.. |enc-std_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |enc-std_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2


.. _enc-rsc_db-enc-type:

``--enc-type``
""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``COSET`` ``NO`` ``RSC_DB`` ``USER`` 
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+------------+-------------------------+
| Value      | Description             |
+============+=========================+
| ``AZCW``   | |enc-type_descr_azcw|   |
+------------+-------------------------+
| ``COSET``  | |enc-type_descr_coset|  |
+------------+-------------------------+
| ``NO``     | |enc-type_descr_no|     |
+------------+-------------------------+
| ``RSC_DB`` | |enc-type_descr_rsc_db| |
+------------+-------------------------+
| ``USER``   | |enc-type_descr_user|   |
+------------+-------------------------+

.. |enc-type_descr_azcw| replace:: TODO VALUE AZCW
.. |enc-type_descr_coset| replace:: TODO VALUE COSET
.. |enc-type_descr_no| replace:: TODO VALUE NO
.. |enc-type_descr_rsc_db| replace:: TODO VALUE RSC_DB
.. |enc-type_descr_user| replace:: TODO VALUE USER


