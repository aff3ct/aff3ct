.. _pct-ldpc-puncturer-parameters:

|LDPC| Puncturer parameters
---------------------------

.. _pct-ldpc-pct-fra-size:

``--pct-fra-size, -N`` |image_required_argument|
""""""""""""""""""""""""""""""""""""""""""""""""

   :Type: integer
   :Examples: ``--pct-fra-size 1``

Useful number of bit transmitted (information bits).

.. _pct-ldpc-pct-type:

``--pct-type``
""""""""""""""

   :Type: text
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


.. _pct-ldpc-pct-pattern:

``--pct-pattern``
"""""""""""""""""

   :Type: text
   :Examples: ``--pct-pattern "TODO CHECK VALUE"``

Puncturing pattern for the LDPC encoder/decoder (size = N_Code/Z) (ex: "1,1,1,0").