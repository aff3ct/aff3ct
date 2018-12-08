.. _crc-crc-parameters:

CRC parameters
--------------

The following parameters concern the Cyclic Redundancy Check (CRC) module.
|CRC| bits can be concatenated to the information bits in order to help the
decoding process to know if the decoded bit sequence is valid or not.

.. note:: The |CRC| is only available for some specific decoders that have been
          designed to take advantage of the |CRC| like in
          :cite:`Li2012,Tonnellier2016b`.

.. warning:: Using a |CRC| does not guarantee to know if the decoded frame is
             the good one, it can be a *false positive*. It is important to
             adapt the size of the |CRC| with the frame size and the targeted
             |FER|.

.. _crc-crc-poly:

``--crc-type, --crc-poly``
""""""""""""""""""""""""""

   :Type: text
   :Default: ``NO``
   :Examples: | ``--crc-type "32-GZIP"``
              | ``--crc-poly "0x04C11DB7" --crc-size 32``

**STRING**: |factory::CRC::parameters::p+type,p+poly|

Select the |CRC| type you want to use among the predefined polynomials in the
:numref:`tab_crc_polynomials`. If you want a specific polynomial that it is not
available in the table you can directly put the polynomial in hexadecimal. In
this case you have to specify explicitly the size of the polynomial with the
:ref:`crc-crc-size` parameter. The type ``NO`` deactivates the |CRC|.

.. _tab_crc_polynomials:

.. csv-table:: List of the predefined |CRC| polynomials.
   :delim: ;
   :align: center
   :file: crc_polynomials.csv

.. _crc-crc-size:

``--crc-size``
""""""""""""""

   :Type: integer
   :Range: :math:`]0 \to \infty[`
   :Examples: ``--crc-size 8``

**STRING**: |factory::CRC::parameters::p+size|

Size the |CRC| (divisor size in bits minus one), required if you selected an
unknown |CRC|.

.. _crc-crc-implem:

``--crc-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST`` ``INTER``
   :Default: ``FAST``
   :Examples: ``--crc-implem FAST``

**STRING**: |factory::CRC::parameters::p+implem|

Select the |CRC| implementation you want to use.

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

.. |crc-implem_descr_std| replace:: The standard implementation is generic and
   support any size of |CRCs|. On the other hand the throughput is limited.
.. |crc-implem_descr_fast| replace:: This implementation is much faster than the
   standard one. This speedup is achieved thanks to the bit packing technique:
   up to 32 bits can be computed in parallel. This implementation does not
   support polynomials higher than 32 bits.
.. |crc-implem_descr_inter| replace:: The inter-frame implementation should not
   be used in general cases. It allow to compute the |CRC| on many frames in
   parallel that have been reordered.

References
""""""""""

.. bibliography:: references.bib

