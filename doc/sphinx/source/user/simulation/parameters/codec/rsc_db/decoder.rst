.. _dec-rsc_db-decoder-parameters:

|RSC| |DB| Decoder parameters
-----------------------------

.. _dec-rsc_db-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
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
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-rsc_db-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``GENERIC`` ``DVB-RCS1`` ``DVB-RCS2``
   :Examples: ``--dec-implem DVB-RCS1``

Select the implementation of the algorithm to decode.

Description of the allowed values:

+--------------+-----------------------------+
| Value        | Description                 |
+==============+=============================+
| ``GENERIC``  | |dec-implem_descr_generic|  |
+--------------+-----------------------------+
| ``DVB-RCS1`` | |dec-implem_descr_dvb-rcs1| |
+--------------+-----------------------------+
| ``DVB-RCS2`` | |dec-implem_descr_dvb-rcs2| |
+--------------+-----------------------------+

.. |dec-implem_descr_generic| replace:: TODO VALUE GENERIC
.. |dec-implem_descr_dvb-rcs1| replace:: TODO VALUE DVB-RCS1
.. |dec-implem_descr_dvb-rcs2| replace:: TODO VALUE DVB-RCS2

.. _dec-rsc_db-dec-max:

``--dec-max``
"""""""""""""

   :Type: text
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