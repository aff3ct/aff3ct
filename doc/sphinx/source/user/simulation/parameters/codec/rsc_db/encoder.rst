.. _enc-rsc_db-encoder-parameters:

|RSC| |DB| Encoder parameters
-----------------------------

.. _enc-rsc_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Useful number of bit transmitted (information bits).

.. _enc-rsc_db-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``RSC_DB`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

Type of the encoder to use in the simulation.

Description of the allowed values:

+------------+-------------------------+
| Value      | Description             |
+============+=========================+
| ``RSC_DB`` | |enc-type_descr_rsc_db| |
+------------+-------------------------+
| ``AZCW``   | |enc-type_descr_azcw|   |
+------------+-------------------------+
| ``COSET``  | |enc-type_descr_coset|  |
+------------+-------------------------+
| ``USER``   | |enc-type_descr_user|   |
+------------+-------------------------+

.. |enc-type_descr_rsc_db| replace:: TODO VALUE RSC_DB
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-rsc_db-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""

Disable the buffered encoding.

.. _enc-rsc_db-enc-std:

``--enc-std``
"""""""""""""

   :Type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2``
   :Examples: ``--enc-std DVB-RCS1``

Select a standard and set automatically some parameters (overwritten with user
given arguments).

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
