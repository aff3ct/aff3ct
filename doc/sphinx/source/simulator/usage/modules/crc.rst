.. _crc-crc-parameters:

CRC parameters
--------------

The following parameters concern the Cyclic Redundancy Check (CRC) module.
CRC bits can be concatenated to the information bits in order to help the decoding process to know if the
decoded codeword is valid or not.

.. note:: The CRC is only available for some specific decoders that have been designed to take advantage of the CRC like
          in :cite:`Li2012,Tonnellier2016b`.

.. warning:: Using a CRC does not guarantee to know if the decoded codeword is the good one, it can be a
             *false positive*. It is important to adapt the size of the CRC with the frame size and the targeted FER.

.. _crc- --crc-poly:

``--crc-type, --crc-poly``
""""""""""""""""""""""""""

   :Type: text
   :Examples: | ``--crc-type "32-GZIP"``
              | ``--crc-poly "0x04C11DB7" --crc-size 32``

Select the CRC type you want to use among the predefined polynomials in the following table.
If you want a specific polynomial that it is not available in the table you can directly put the polynomial in hexadecimal.
In this case you have to specify explicitly the size of the polynomial with the :ref:`crc-crc-size` parameter.

+-----------------+------------+------+
| Type            | Polynomial | Size |
+=================+============+======+
| 32-GZIP         | 0x04C11DB7 | 32   |
+-----------------+------------+------+
| 32-CASTAGNOLI   | 0x1EDC6F41 | 32   |
+-----------------+------------+------+
| 32-AIXM         | 0x814141AB | 32   |
+-----------------+------------+------+
| 32-KOOPMAN      | 0x32583499 | 32   |
+-----------------+------------+------+
| 30-CDMA         | 0x2030B9C7 | 30   |
+-----------------+------------+------+
| 24-LTEA         | 0x864CFB   | 24   |
+-----------------+------------+------+
| 24-RADIX-64     | 0x864CFB   | 24   |
+-----------------+------------+------+
| 24-FLEXRAY      | 0x5D6DCB   | 24   |
+-----------------+------------+------+
| 21-CAN          | 0x102899   | 21   |
+-----------------+------------+------+
| 17-CAN          | 0x1685B    | 17   |
+-----------------+------------+------+
| 16-IBM          | 0x8005     | 16   |
+-----------------+------------+------+
| 16-CCITT        | 0x1021     | 16   |
+-----------------+------------+------+
| 16-PROFIBUS     | 0x1DCF     | 16   |
+-----------------+------------+------+
| 16-OPENSAFETY-B | 0x755B     | 16   |
+-----------------+------------+------+
| 16-OPENSAFETY-A | 0x5935     | 16   |
+-----------------+------------+------+
| 16-DNP          | 0x3D65     | 16   |
+-----------------+------------+------+
| 16-T10-DIF      | 0x8BB7     | 16   |
+-----------------+------------+------+
| 16-DECT         | 0x0589     | 16   |
+-----------------+------------+------+
| 16-CDMA2000     | 0xC867     | 16   |
+-----------------+------------+------+
| 16-ARINC        | 0xA02B     | 16   |
+-----------------+------------+------+
| 16-CHAKRAVARTY  | 0x2F15     | 16   |
+-----------------+------------+------+
| 15-MPT1327      | 0x6815     | 15   |
+-----------------+------------+------+
| 15-CAN          | 0x4599     | 15   |
+-----------------+------------+------+
| 14-DARC         | 0x0805     | 14   |
+-----------------+------------+------+
| 13-BBC          | 0x1CF5     | 13   |
+-----------------+------------+------+
| 12-CDMA2000     | 0xF13      | 12   |
+-----------------+------------+------+
| 12-TELECOM      | 0x80F      | 12   |
+-----------------+------------+------+
| 11-FLEXRAY      | 0x385      | 11   |
+-----------------+------------+------+
| 10-CDMA2000     | 0x3D9      | 10   |
+-----------------+------------+------+
| 10-ATM          | 0x233      | 10   |
+-----------------+------------+------+
| 8-WCDMA         | 0x9B       |  8   |
+-----------------+------------+------+
| 8-SAE-J1850     | 0x1D       |  8   |
+-----------------+------------+------+
| 8-DARC          | 0x39       |  8   |
+-----------------+------------+------+
| 8-DALLAS        | 0x31       |  8   |
+-----------------+------------+------+
| 8-CCITT         | 0x07       |  8   |
+-----------------+------------+------+
| 8-AUTOSAR       | 0x2F       |  8   |
+-----------------+------------+------+
| 8-DVB-S2        | 0xD5       |  8   |
+-----------------+------------+------+
| 7-MVB           | 0x65       |  7   |
+-----------------+------------+------+
| 7-MMC           | 0x09       |  7   |
+-----------------+------------+------+
| 6-CDMA2000-A    | 0x27       |  6   |
+-----------------+------------+------+
| 6-CDMA2000-B    | 0x07       |  6   |
+-----------------+------------+------+
| 6-DARC          | 0x19       |  6   |
+-----------------+------------+------+
| 6-ITU           | 0x03       |  6   |
+-----------------+------------+------+
| 5-ITU           | 0x15       |  5   |
+-----------------+------------+------+
| 5-EPC           | 0x09       |  5   |
+-----------------+------------+------+
| 5-USB           | 0x05       |  5   |
+-----------------+------------+------+
| 4-ITU           | 0x3        |  4   |
+-----------------+------------+------+
| 1-PAR           | 0x1        |  1   |
+-----------------+------------+------+

.. _crc-crc-size:

``--crc-size``
""""""""""""""

   :Type: integer
   :Range: :math:`]0 \to \infty[`
   :Examples: ``--crc-size 8``

Size of the CRC (divisor size in bit -1), required if you selected an unknown CRC.

.. _crc-crc-implem:

``--crc-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST`` ``INTER``
   :Examples: ``--crc-implem FAST``

Select the CRC implementation you want to use.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``STD``   | |crc-implem_descr_std|   |
+-----------+--------------------------+
| ``FAST``  | |crc-implem_descr_fast|  |
+-----------+--------------------------+
| ``INTER`` | |crc-implem_descr_inter| |
+-----------+--------------------------+

.. |crc-implem_descr_std| replace:: The standard implementation is generic and support any size of CRCs.
                                    On the other hand the throughput is limited.
.. |crc-implem_descr_fast| replace:: This implementation is much faster than the standard one. This speedup is
                                     achieved thanks to the bit packing technique : up to 32 bits can be computed in
                                     parallel. This implementation does not support polynomials higher than 32 bits.
.. |crc-implem_descr_inter| replace:: The inter-frame implementation should not be used in general cases. It allow to
                                      compute the CRC on many frames in parallel that have been reordered.

References
""""""""""

.. bibliography:: ../../../references.bib

