.. _pct-turbo_db-puncturer-parameters:

Turbo |DB| Puncturer parameters
-------------------------------

.. _pct-turbo_db-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``TURBO_DB``
   :Default: ``NO``
   :Examples: ``--pct-type NO``

**STRING**: |factory::Puncturer::parameters::p+type|

Select the puncturer type.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``NO``       | |pct-type_descr_no|       |
+--------------+---------------------------+
| ``TURBO_DB`` | |pct-type_descr_turbo_db| |
+--------------+---------------------------+

.. |pct-type_descr_no| replace:: Disable the puncturer.
.. |pct-type_descr_turbo_db| replace:: Enable the puncturer.

.. _pct-turbo_db-pct-fra-size:

``--pct-fra-size, -N``
""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 1``

**STRING**: |factory::Puncturer::parameters::p+fra-size,N|

Set the frame size :math:`N`.
The puncturer supports :math:`R = 2/5`, :math:`R = 1/2`, :math:`R = 2/3` and
:math:`R = 4/5` with :math:`R = K/N`.