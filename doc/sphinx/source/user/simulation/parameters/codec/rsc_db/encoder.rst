.. _enc-rsc_db-encoder-parameters:

|RSC| |DB| Encoder parameters
-----------------------------

.. _enc-rsc_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

**STRING**: |factory::Encoder::parameters::p+info-bits,K|

Set the number of information bits :math:`K`.
The codeword size :math:`N` is automatically deduced: :math:`N = 2 \times K`.

.. _enc-rsc_db-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``RSC_DB`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``RSC_DB``
   :Examples: ``--enc-type AZCW``

**STRING**: |factory::Encoder::parameters::p+type|

Select the encoder type.

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

.. |enc-type_descr_rsc_db| replace:: Select the standard |RSC| |DB| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-rsc_db-enc-no-buff:

``--enc-no-buff``
"""""""""""""""""

**STRING**: |factory::Encoder_RSC_DB::parameters::p+no-buff|

Disable the buffered encoding.

.. _enc-rsc_db-enc-std:

``--enc-std``
"""""""""""""

   :Type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2``
   :Default: ``DVB-RCS1``
   :Examples: ``--enc-std DVB-RCS2``

**STRING**: |factory::Encoder_RSC_DB::parameters::p+std|

Select a standard.

Description of the allowed values:

+--------------+--------------------------+
| Value        | Description              |
+==============+==========================+
| ``DVB-RCS1`` | |enc-std_descr_dvb-rcs1| |
+--------------+--------------------------+
| ``DVB-RCS2`` | |enc-std_descr_dvb-rcs2| |
+--------------+--------------------------+

.. |enc-std_descr_dvb-rcs1| replace:: Select the configuration of the |DVB-RCS1|
   standard.
.. |enc-std_descr_dvb-rcs2| replace:: Select the configuration of the |DVB-RCS2|
   standard.