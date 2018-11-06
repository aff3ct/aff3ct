.. _dec-turbo_db-decoder-parameters:

Decoder parameters
------------------

.. _dec-turbo_db-dec-flips:

``--dec-flips``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_db-dec-fnc:

``--dec-fnc``
"""""""""""""


Enables the flip and check decoder (requires "\\-\\-crc-type").

.. _dec-turbo_db-dec-fnc-crc-ite:

``--dec-fnc-crc-ite``
"""""""""""""""""""""

   :type: integer
   :Examples: ``--dec-fnc-crc-ite 1``

Set the iteration to start the CRC checking.

.. _dec-turbo_db-dec-fnc-ite-M:

``--dec-fnc-ite-M``
"""""""""""""""""""

   :type: integer
   :Examples: ``--dec-fnc-ite-M 1``

Set last iteration at which the fnc is used.

.. _dec-turbo_db-dec-fnc-ite-m:

``--dec-fnc-ite-m``
"""""""""""""""""""

   :type: integer
   :Examples: ``--dec-fnc-ite-m 1``

Set first iteration at which the fnc is used.

.. _dec-turbo_db-dec-fnc-ite-s:

``--dec-fnc-ite-s``
"""""""""""""""""""

   :type: integer
   :Examples: ``--dec-fnc-ite-s 1``

Set iteration step for the fnc algorithm.

.. _dec-turbo_db-dec-fnc-q:

``--dec-fnc-q``
"""""""""""""""

   :type: integer
   :Examples: ``--dec-fnc-q 1``

Set the search's space for the fnc algorithm.

.. _dec-turbo_db-dec-hamming:

``--dec-hamming``
"""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-turbo_db-dec-implem:

``--dec-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``NAIVE`` ``STD`` 
   :Examples: ``--dec-implem NAIVE``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``STD``   | |dec-implem_descr_std|   |
+-----------+--------------------------+

.. |dec-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-implem_descr_std| replace:: TODO VALUE STD


.. _dec-turbo_db-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :type: integer
   :Examples: ``--dec-ite 1``

Maximal number of iterations in the turbo.

.. _dec-turbo_db-dec-sf-type:

``--dec-sf-type``
"""""""""""""""""

   :type: text
   :Allowed values: ``ADAPTIVE`` ``ARRAY`` ``CST`` ``LTE`` ``LTE_VEC`` 
   :Examples: ``--dec-sf-type ADAPTIVE``

Scaling factor type.

Description of the allowed values:

+--------------+------------------------------+
| Value        | Description                  |
+==============+==============================+
| ``ADAPTIVE`` | |dec-sf-type_descr_adaptive| |
+--------------+------------------------------+
| ``ARRAY``    | |dec-sf-type_descr_array|    |
+--------------+------------------------------+
| ``CST``      | |dec-sf-type_descr_cst|      |
+--------------+------------------------------+
| ``LTE``      | |dec-sf-type_descr_lte|      |
+--------------+------------------------------+
| ``LTE_VEC``  | |dec-sf-type_descr_lte_vec|  |
+--------------+------------------------------+

.. |dec-sf-type_descr_adaptive| replace:: TODO VALUE ADAPTIVE
.. |dec-sf-type_descr_array| replace:: TODO VALUE ARRAY
.. |dec-sf-type_descr_cst| replace:: TODO VALUE CST
.. |dec-sf-type_descr_lte| replace:: TODO VALUE LTE
.. |dec-sf-type_descr_lte_vec| replace:: TODO VALUE LTE_VEC


.. _dec-turbo_db-dec-sub-flips:

``--dec-sub-flips``
"""""""""""""""""""

   :type: integer
   :Examples: ``--dec-sub-flips 1``

Set the maximum number of flips in the CHASE decoder.

.. _dec-turbo_db-dec-sub-hamming:

``--dec-sub-hamming``
"""""""""""""""""""""


Enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders.

.. _dec-turbo_db-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

   :type: text
   :Allowed values: ``DVB-RCS1`` ``DVB-RCS2`` ``GENERIC`` ``NAIVE`` ``STD`` 
   :Examples: ``--dec-sub-implem DVB-RCS1``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+--------------+---------------------------------+
| Value        | Description                     |
+==============+=================================+
| ``DVB-RCS1`` | |dec-sub-implem_descr_dvb-rcs1| |
+--------------+---------------------------------+
| ``DVB-RCS2`` | |dec-sub-implem_descr_dvb-rcs2| |
+--------------+---------------------------------+
| ``GENERIC``  | |dec-sub-implem_descr_generic|  |
+--------------+---------------------------------+
| ``NAIVE``    | |dec-sub-implem_descr_naive|    |
+--------------+---------------------------------+
| ``STD``      | |dec-sub-implem_descr_std|      |
+--------------+---------------------------------+

.. |dec-sub-implem_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |dec-sub-implem_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2
.. |dec-sub-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-sub-implem_descr_naive| replace:: TODO VALUE NAIVE
.. |dec-sub-implem_descr_std| replace:: TODO VALUE STD


.. _dec-turbo_db-dec-sub-max:

``--dec-sub-max``
"""""""""""""""""

   :type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS`` 
   :Examples: ``--dec-sub-max MAX``

The MAX implementation for the nodes.

Description of the allowed values:

+----------+--------------------------+
| Value    | Description              |
+==========+==========================+
| ``MAX``  | |dec-sub-max_descr_max|  |
+----------+--------------------------+
| ``MAXL`` | |dec-sub-max_descr_maxl| |
+----------+--------------------------+
| ``MAXS`` | |dec-sub-max_descr_maxs| |
+----------+--------------------------+

.. |dec-sub-max_descr_max| replace:: TODO VALUE MAX
.. |dec-sub-max_descr_maxl| replace:: TODO VALUE MAXL
.. |dec-sub-max_descr_maxs| replace:: TODO VALUE MAXS


.. _dec-turbo_db-dec-sub-type:

``--dec-sub-type, -D``
""""""""""""""""""""""

   :type: text
   :Allowed values: ``BCJR`` ``CHASE`` ``ML`` 
   :Examples: ``--dec-sub-type BCJR``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+----------------------------+
| Value     | Description                |
+===========+============================+
| ``BCJR``  | |dec-sub-type_descr_bcjr|  |
+-----------+----------------------------+
| ``CHASE`` | |dec-sub-type_descr_chase| |
+-----------+----------------------------+
| ``ML``    | |dec-sub-type_descr_ml|    |
+-----------+----------------------------+

.. |dec-sub-type_descr_bcjr| replace:: TODO VALUE BCJR
.. |dec-sub-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-sub-type_descr_ml| replace:: TODO VALUE ML


.. _dec-turbo_db-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :type: text
   :Allowed values: ``CHASE`` ``ML`` ``TURBO_DB`` 
   :Examples: ``--dec-type CHASE``

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+--------------+---------------------------+
| Value        | Description               |
+==============+===========================+
| ``CHASE``    | |dec-type_descr_chase|    |
+--------------+---------------------------+
| ``ML``       | |dec-type_descr_ml|       |
+--------------+---------------------------+
| ``TURBO_DB`` | |dec-type_descr_turbo_db| |
+--------------+---------------------------+

.. |dec-type_descr_chase| replace:: TODO VALUE CHASE
.. |dec-type_descr_ml| replace:: TODO VALUE ML
.. |dec-type_descr_turbo_db| replace:: TODO VALUE TURBO_DB


