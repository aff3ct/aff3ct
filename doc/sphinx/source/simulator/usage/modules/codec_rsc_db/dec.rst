.. _dec-rsc_db-decoder-parameters:

Decoder parameters
------------------

.. _dec-rsc_db-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-rsc_db-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-rsc_db-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2`` ``GENERIC`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-implem DVB-RCS1``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+--------------+-----------------------------+
| Value        | Description                 |
+==============+=============================+
| ``DVB-RCS1`` | |dec-implem_descr_dvb-rcs1| |
+--------------+-----------------------------+
| ``DVB-RCS2`` | |dec-implem_descr_dvb-rcs2| |
+--------------+-----------------------------+
| ``GENERIC``  | |dec-implem_descr_generic|  |
+--------------+-----------------------------+
| ``NAIVE``    | |dec-implem_descr_naive|    |
+--------------+-----------------------------+
| ``STD``      | |dec-implem_descr_std|      |
+--------------+-----------------------------+

.. |dec-implem_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |dec-implem_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2
.. |dec-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-rsc_db-dec-max:

``--dec-max``
"""""""""""""

   :type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS`` 
   :Examples: ``--dec-max MAX``

The MAX implementation for the nodes.

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``MAX``  | |dec-max_descr_max|  |
+----------+----------------------+
| ``MAXL`` | |dec-max_descr_maxl| |
+----------+----------------------+
| ``MAXS`` | |dec-max_descr_maxs| |
+----------+----------------------+

.. |dec-max_descr_max| replace:: TODO VALUE MAX
.. |dec-max_descr_maxl| replace:: TODO VALUE MAXL
.. |dec-max_descr_maxs| replace:: TODO VALUE MAXS


.. _dec-rsc_db-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-type BCJR``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``BCJR``  | |dec-type_descr_bcjr|  |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_bcjr| replace:: TODO VALUE BCJR
.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML


