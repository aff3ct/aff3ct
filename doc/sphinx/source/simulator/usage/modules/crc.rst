.. _crc-crc-parameters:

CRC parameters
--------------

.. _crc-crc-implem:

``--crc-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``INTER`` ``STD`` 
   :Examples: ``--crc-implem FAST``

Select the CRC implementation you want to use.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``FAST``  | |crc-implem_descr_fast|  |
+-----------+--------------------------+
| ``INTER`` | |crc-implem_descr_inter| |
+-----------+--------------------------+
| ``STD``   | |crc-implem_descr_std|   |
+-----------+--------------------------+

.. |crc-implem_descr_fast| replace:: TODO VALUE FAST
.. |crc-implem_descr_inter| replace:: TODO VALUE INTER
.. |crc-implem_descr_std| replace:: TODO VALUE STD


.. _crc-crc-size:

``--crc-size``
""""""""""""""

   :type: integer
   :Examples: ``--crc-size 1``

Size of the CRC (divisor size in bit -1), required if you selected an unknown CRC.

.. _crc- --crc-poly:

``--crc-type, --crc-poly``
""""""""""""""""""""""""""

   :type: text
   :Examples: `` --crc-poly "TODO CHECK VALUE"``

Select the CRC type/polynomial you want to use (ex: "8-DVB-S2": 0xD5, "16-IBM": 0x8005, "24-LTEA": 0x864CFB, "32-GZIP": 0x04C11DB7).

