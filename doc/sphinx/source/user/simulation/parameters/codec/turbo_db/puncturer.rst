.. _pct-turbo_db-puncturer-parameters:

Turbo |DB| Puncturer parameters
-------------------------------

.. _pct-turbo_db-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``TURBO_DB``
   :Examples: ``--pct-type NO``

Code puncturer type.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``NO``       | |pct-type_descr_no|       |
+--------------+---------------------------+
| ``TURBO_DB`` | |pct-type_descr_turbo_db| |
+--------------+---------------------------+

.. |pct-type_descr_no| replace:: TODO VALUE NO
.. |pct-type_descr_turbo_db| replace:: TODO VALUE TURBO_DB

.. _pct-turbo_db-pct-fra-size:

``--pct-fra-size, -N``
""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 1``

Useful number of bit transmitted (information bits).