.. _pct-ldpc-puncturer-parameters:

Puncturer parameters
--------------------

.. _pct-ldpc-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
   :Allowed values: ``LDPC`` ``NO``
   :Default: ``LDPC``
   :Examples: ``--pct-type LDPC``

Select the puncturer type.

Description of the allowed values:

+----------+-----------------------+
| Value    | Description           |
+==========+=======================+
| ``LDPC`` | |pct-type_descr_ldpc| |
+----------+-----------------------+
| ``NO``   | |pct-type_descr_no|   |
+----------+-----------------------+

.. |pct-type_descr_ldpc| replace:: Puncture the |LDPC| codeword.
.. |pct-type_descr_no|   replace:: Deactivate the puncturer.

.. _pct-ldpc-pct-fra-size:

``--pct-fra-size, -N`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 912``

Set the number of bit transmitted after the puncture process.
If the given size matches with the codeword size, then the puncturer is
deactivated.

.. _pct-ldpc-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :Type: binary vector
   :Examples: ``--pct-pattern "1,1,1,0"``

Give the puncturing pattern following the |LDPC| code.
The number :math:`P` of values given in this pattern must be as
:math:`N_{cw} = P \times Z` where :math:`Z` is the number of bits represented
by a single value in the pattern.

This |LDPC| puncturer behavior is such as, for the above example, the first
three quarter bits are kept and the last quarter is removed from the frame.