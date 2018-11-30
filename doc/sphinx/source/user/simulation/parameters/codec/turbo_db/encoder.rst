.. _enc-turbo_db-encoder-parameters:

Turbo |DB| Encoder parameters
-----------------------------

.. _enc-turbo_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-turbo_db-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``TURBO_DB`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``TURBO_DB`` | |enc-type_descr_turbo_db| |
+--------------+---------------------------+
| ``AZCW``     | |enc-type_descr_azcw|     |
+--------------+---------------------------+
| ``COSET``    | |enc-type_descr_coset|    |
+--------------+---------------------------+
| ``USER``     | |enc-type_descr_user|     |
+--------------+---------------------------+

.. |enc-type_descr_turbo_db| replace:: TODO VALUE TURBO_DB
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo_db-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

   :Type: text
   :Allowed values: ``RSC_DB`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-sub-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+------------+-----------------------------+
| Value      | Description                 |
+============+=============================+
| ``RSC_DB`` | |enc-sub-type_descr_rsc_db| |
+------------+-----------------------------+
| ``AZCW``   | |enc-sub-type_descr_azcw|   |
+------------+-----------------------------+
| ``COSET``  | |enc-sub-type_descr_coset|  |
+------------+-----------------------------+
| ``USER``   | |enc-sub-type_descr_user|   |
+------------+-----------------------------+

.. |enc-sub-type_descr_rsc_db| replace:: TODO VALUE RSC_DB
.. |enc-sub-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-sub-type_descr_coset| replace:: See the common
   :ref:`enc-common-enc-type` parameter.
.. |enc-sub-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo_db-enc-json-path:

``--enc-json-path``
"""""""""""""""""""

   :Type: file
   :Rights: write only
   :Examples: ``--enc-json-path example/path/to/the/right/file``

Path to store the encoder and decoder traces formated in JSON.

.. _enc-turbo_db-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2``
   :Examples: ``--enc-sub-std DVB-RCS1``

Select a standard and set automatically some parameters (overwritten with user
given arguments).

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