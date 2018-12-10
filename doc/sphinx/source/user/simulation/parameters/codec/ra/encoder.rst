.. _enc-ra-encoder-parameters:

|RA| Encoder parameters
-----------------------

.. _enc-ra-enc-cw-size:

``--enc-cw-size, -N`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-cw-size 1``

Set the codeword size :math:`N`.

.. _enc-ra-enc-info-bits:

``--enc-info-bits, -K`` |image_required_argument|
"""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-info-bits 1``

Set the nulber of information bits :math:`K`.

.. _enc-ra-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``RA`` ``AZCW`` ``COSET`` ``USER``
   :Default: ``RA``
   :Examples: ``--enc-type AZCW``

Select the encoder type.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``RA``    | |enc-type_descr_ra|    |
+-----------+------------------------+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_ra| replace:: Select the standard |RA| encoder.
.. |enc-type_descr_azcw| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_coset| replace:: See the common :ref:`enc-common-enc-type`
   parameter.
.. |enc-type_descr_user| replace:: See the common :ref:`enc-common-enc-type`
   parameter.