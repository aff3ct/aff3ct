.. _enc-turbo_db-encoder-parameters:

Turbo |DB| Encoder parameters
-----------------------------

.. _enc-turbo_db-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 40``

|factory::Encoder::p+info-bits,K|

The codeword size :math:`N` is automatically deduced: :math:`N = 3 \times K`.

.. _enc-turbo_db-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``TURBO_DB`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``TURBO_DB``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::p+type|

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

.. |enc-type_descr_turbo_db| replace:: Select the standard Turbo |DB| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.

.. _enc-turbo_db-enc-sub-type:

``--enc-sub-type``
""""""""""""""""""

Please refer to the |RSC| |DB| :ref:`enc-rsc_db-enc-type` parameter.

.. _enc-turbo_db-enc-sub-std:

``--enc-sub-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2``
   :Default: ``DVB-RCS1``
   :Examples: ``--enc-sub-std DVB-RCS2``

|factory::Encoder_RSC_DB::p+std|

Description of the allowed values:

+--------------+------------------------------+
| Value        | Description                  |
+==============+==============================+
| ``DVB-RCS1`` | |enc-sub-std_descr_dvb-rcs1| |
+--------------+------------------------------+
| ``DVB-RCS2`` | |enc-sub-std_descr_dvb-rcs2| |
+--------------+------------------------------+

.. |enc-sub-std_descr_dvb-rcs1| replace:: Set the |DVB-RCS1| trellis and select
   the |DVB-RCS1| interleaver (see the :ref:`itl-itl-type` parameter).
.. |enc-sub-std_descr_dvb-rcs2| replace:: Set the |DVB-RCS2| trellis and select
   the |DVB-RCS2| interleaver (see the :ref:`itl-itl-type` parameter).