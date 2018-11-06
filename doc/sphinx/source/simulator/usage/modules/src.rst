.. _src-source-parameters:

Source parameters
-----------------

.. _src-src-fra:

``--src-fra, -F``
"""""""""""""""""

   :type: integer
   :Examples: ``--src-fra 1``

Set the number of inter frame level to process.

.. _src-src-implem:

``--src-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``STD`` 
   :Examples: ``--src-implem FAST``

Select the implementation of the algorithm to generate the information bits.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``FAST`` | |src-implem_descr_fast| |
+----------+-------------------------+
| ``STD``  | |src-implem_descr_std|  |
+----------+-------------------------+

.. |src-implem_descr_fast| replace:: TODO VALUE FAST
.. |src-implem_descr_std| replace:: TODO VALUE STD


.. _src-src-path:

``--src-path``
""""""""""""""

   :type: file
   :Rights: read only
   :Examples: ``--src-path example/path/to/the/right/file``

Path to a file containing one or a set of pre-computed source bits, to use with "\\-\\-src-type USER".

.. _src-src-start-idx:

``--src-start-idx``
"""""""""""""""""""

   :type: integer
   :Examples: ``--src-start-idx 1``

Start idx to use in the USER type source.

.. _src-src-type:

``--src-type``
""""""""""""""

   :type: text
   :Allowed values: ``AZCW`` ``RAND`` ``USER`` 
   :Examples: ``--src-type AZCW``

Method used to generate the codewords.

Description of the allowed values:

+----------+-----------------------+
| Value    | Description           |
+==========+=======================+
| ``AZCW`` | |src-type_descr_azcw| |
+----------+-----------------------+
| ``RAND`` | |src-type_descr_rand| |
+----------+-----------------------+
| ``USER`` | |src-type_descr_user| |
+----------+-----------------------+

.. |src-type_descr_azcw| replace:: TODO VALUE AZCW
.. |src-type_descr_rand| replace:: TODO VALUE RAND
.. |src-type_descr_user| replace:: TODO VALUE USER


