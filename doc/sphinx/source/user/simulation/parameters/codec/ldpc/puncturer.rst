.. _pct-ldpc-puncturer-parameters:

|LDPC| Puncturer parameters
---------------------------

.. _pct-ldpc-pct-fra-size:

``--pct-fra-size, -N`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 912``

Set the frame size :math:`N`. This is not necessarily the codeword size if a
puncturing pattern is used.

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
| ``NO``   | |pct-type_descr_no|   |
+----------+-----------------------+
| ``LDPC`` | |pct-type_descr_ldpc| |
+----------+-----------------------+

.. |pct-type_descr_no|   replace:: Disable the puncturer.
.. |pct-type_descr_ldpc| replace:: Puncture the |LDPC| codeword.

.. _pct-ldpc-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :Type: binary vector
   :Examples: ``--pct-pattern "1,1,1,0"``

Give the puncturing pattern following the |LDPC| code. The number :math:`P` of
values given in this pattern must be as :math:`N_{cw} = P \times Z` where
:math:`Z` is the number of bits represented by a single value in the pattern.

This |LDPC| puncturer behavior is such as, for the above example, the first
three quarter bits are kept and the last quarter is removed from the frame.