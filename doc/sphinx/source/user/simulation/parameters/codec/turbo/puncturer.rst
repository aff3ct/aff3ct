.. _pct-turbo-puncturer-parameters:

Turbo Puncturer parameters
--------------------------

.. _pct-turbo-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``TURBO``
   :Examples: ``--pct-type NO``

Code puncturer type.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``NO``    | |pct-type_descr_no|    |
+-----------+------------------------+
| ``TURBO`` | |pct-type_descr_turbo| |
+-----------+------------------------+

.. |pct-type_descr_no| replace:: TODO VALUE NO
.. |pct-type_descr_turbo| replace:: TODO VALUE TURBO

.. _pct-turbo-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :Type: list of (list of (boolean:including set={0|1}):limited length [1;inf]):limited length [3;3], elements of same length
   :Examples: ``--pct-pattern TODO``

Puncturing pattern for the turbo encoder (ex: "11,10,01").