.. _qnt-quantizer-parameters:

Quantizer parameters
--------------------

.. _qnt-qnt-bits:

``--qnt-bits``
""""""""""""""

   :type: integer
   :Examples: ``--qnt-bits 1``

The number of bits used for the quantizer.

.. _qnt-qnt-dec:

``--qnt-dec``
"""""""""""""

   :type: integer
   :Examples: ``--qnt-dec 1``

The position of the fixed point in the quantified representation.

.. _qnt-qnt-implem:

``--qnt-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``STD`` 
   :Examples: ``--qnt-implem FAST``

Select the implementation of quantizer.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``FAST`` | |qnt-implem_descr_fast| |
+----------+-------------------------+
| ``STD``  | |qnt-implem_descr_std|  |
+----------+-------------------------+

.. |qnt-implem_descr_fast| replace:: TODO VALUE FAST
.. |qnt-implem_descr_std| replace:: TODO VALUE STD


.. _qnt-qnt-range:

``--qnt-range``
"""""""""""""""

   :type: real number
   :Examples: ``--qnt-range 1.0``

The min/max bound for the tricky quantizer.

.. _qnt-qnt-type:

``--qnt-type``
""""""""""""""

   :type: text
   :Allowed values: ``CUSTOM`` ``POW2`` 
   :Examples: ``--qnt-type CUSTOM``

Type of the quantizer to use in the simulation.

Description of the allowed values:

+------------+-------------------------+
| Value      | Description             |
+============+=========================+
| ``CUSTOM`` | |qnt-type_descr_custom| |
+------------+-------------------------+
| ``POW2``   | |qnt-type_descr_pow2|   |
+------------+-------------------------+

.. |qnt-type_descr_custom| replace:: TODO VALUE CUSTOM
.. |qnt-type_descr_pow2| replace:: TODO VALUE POW2


