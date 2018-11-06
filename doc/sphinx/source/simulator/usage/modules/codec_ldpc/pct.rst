.. _pct-ldpc-puncturer-parameters:

Puncturer parameters
--------------------

.. _pct-ldpc-pct-fra-size:

``--pct-fra-size, -N`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :type: integer
   :Examples: ``--pct-fra-size 1``

Useful number of bit transmitted (information bits).

.. _pct-ldpc-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :type: text
   :Examples: ``--pct-pattern "TODO CHECK VALUE"``

Puncturing pattern for the LDPC encoder/decoder (size = N_Code/Z) (ex: "1,1,1,0").

.. _pct-ldpc-pct-type:

``--pct-type``
""""""""""""""

   :type: text
   :Allowed values: ``LDPC`` ``NO`` 
   :Examples: ``--pct-type LDPC``

Code puncturer type.

Description of the allowed values:

+----------+-----------------------+
| Value    | Description           |
+==========+=======================+
| ``LDPC`` | |pct-type_descr_ldpc| |
+----------+-----------------------+
| ``NO``   | |pct-type_descr_no|   |
+----------+-----------------------+

.. |pct-type_descr_ldpc| replace:: TODO VALUE LDPC
.. |pct-type_descr_no| replace:: TODO VALUE NO


