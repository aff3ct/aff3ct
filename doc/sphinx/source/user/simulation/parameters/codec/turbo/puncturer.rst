.. _pct-turbo-puncturer-parameters:

Turbo Puncturer parameters
--------------------------

.. _pct-turbo-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``TURBO``
   :Default: ``NO``
   :Examples: ``--pct-type NO``

Select the puncturer type.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``NO``    | |pct-type_descr_no|    |
+-----------+------------------------+
| ``TURBO`` | |pct-type_descr_turbo| |
+-----------+------------------------+

.. |pct-type_descr_no| replace:: Disable the puncturer.
.. |pct-type_descr_turbo| replace:: Enable the puncturing patterns.

.. note:: The frame size will be automatically set from the given puncturing
   pattern (c.f. the :ref:`pct-turbo-pct-pattern` parameter).

.. _pct-turbo-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :Type: list of (list of (boolean:including set={0|1}):limited length [1;inf]):limited length [3;3], elements of same length
   :Examples: ``--pct-pattern "11,10,01"``

Define the puncturing pattern. Considering the ``"11,10,01"`` puncturing
pattern, the first sub-pattern ``11`` defines the emitted systematic bits,
the second sub-pattern ``10`` defines the emitted parity bits in the natural
domain and the third sub-pattern ``01`` defines the emitted parity bits in
the interleaved domain. 1 means that the bit has to be transmitted and 0 means
that the bit transmission has to be erased.

Given the following frame:
:math:`X_0^{sn},X_1^{pn},\underline{X_2^{pi}},X_3^{sn},\underline{X_4^{pn}},X_5^{pi},X_6^{sn},X_7^{pn},\underline{X_8^{pi}}`,
with the ``"11,10,01"`` puncturing pattern, the underlined bits will not be
emitted. In the previous example, tail bits are not taken into account but in
reality they are always emitted.


