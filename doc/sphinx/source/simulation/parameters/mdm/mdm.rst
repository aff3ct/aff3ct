.. _mdm-modem-parameters:

Modem parameters
----------------

.. _mdm-mdm-bps:

``--mdm-bps``
"""""""""""""

   :Type: integer
   :Examples: ``--mdm-bps 1``

Select the number of bits per symbol (default is 1).

.. _mdm-mdm-const-path:

``--mdm-const-path``
""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Examples: ``--mdm-const-path example/path/to/the/right/file``

Path to the ordered modulation symbols (constellation), to use with "\\-\\-mod-type USER".

.. _mdm-mdm-cpm-k:

``--mdm-cpm-k``
"""""""""""""""

   :Type: integer
   :Examples: ``--mdm-cpm-k 1``

Modulation index numerator (default is 1).

.. _mdm-mdm-cpm-L:

``--mdm-cpm-L``
"""""""""""""""

   :Type: integer
   :Examples: ``--mdm-cpm-L 1``

CPM pulse width or CPM memory (default is 2).

.. _mdm-mdm-cpm-map:

``--mdm-cpm-map``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GRAY`` ``NATURAL``
   :Examples: ``--mdm-cpm-map GRAY``

Symbols mapping layout (default is NATURAL).

Description of the allowed values:

+-------------+-----------------------------+
| Value       | Description                 |
+=============+=============================+
| ``GRAY``    | |mdm-cpm-map_descr_gray|    |
+-------------+-----------------------------+
| ``NATURAL`` | |mdm-cpm-map_descr_natural| |
+-------------+-----------------------------+

.. |mdm-cpm-map_descr_gray| replace:: TODO VALUE GRAY
.. |mdm-cpm-map_descr_natural| replace:: TODO VALUE NATURAL


.. _mdm-mdm-cpm-p:

``--mdm-cpm-p``
"""""""""""""""

   :Type: integer
   :Examples: ``--mdm-cpm-p 1``

Modulation index denominator (default is 2).

.. _mdm-mdm-cpm-std:

``--mdm-cpm-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GSM``
   :Examples: ``--mdm-cpm-std GSM``

The selection of a default CPM standard hardly implemented (any of those parameters is overwritten if the argument is given by the user).

Description of the allowed values:

+---------+-------------------------+
| Value   | Description             |
+=========+=========================+
| ``GSM`` | |mdm-cpm-std_descr_gsm| |
+---------+-------------------------+

.. |mdm-cpm-std_descr_gsm| replace:: TODO VALUE GSM


.. _mdm-mdm-cpm-ws:

``--mdm-cpm-ws``
""""""""""""""""

   :Type: text
   :Allowed values: ``GMSK`` ``RCOS`` ``REC``
   :Examples: ``--mdm-cpm-ws GMSK``

Wave shape (default is GMSK).

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``GMSK`` | |mdm-cpm-ws_descr_gmsk| |
+----------+-------------------------+
| ``RCOS`` | |mdm-cpm-ws_descr_rcos| |
+----------+-------------------------+
| ``REC``  | |mdm-cpm-ws_descr_rec|  |
+----------+-------------------------+

.. |mdm-cpm-ws_descr_gmsk| replace:: TODO VALUE GMSK
.. |mdm-cpm-ws_descr_rcos| replace:: TODO VALUE RCOS
.. |mdm-cpm-ws_descr_rec| replace:: TODO VALUE REC


.. _mdm-mdm-implem:

``--mdm-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Examples: ``--mdm-implem FAST``

Select the implementation of the modem.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``FAST`` | |mdm-implem_descr_fast| |
+----------+-------------------------+
| ``STD``  | |mdm-implem_descr_std|  |
+----------+-------------------------+

.. |mdm-implem_descr_fast| replace:: TODO VALUE FAST
.. |mdm-implem_descr_std| replace:: TODO VALUE STD


.. _mdm-mdm-ite:

``--mdm-ite``
"""""""""""""

   :Type: integer
   :Examples: ``--mdm-ite 1``

Number of iteration in the demodulator.

.. _mdm-mdm-max:

``--mdm-max``
"""""""""""""

   :Type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS`` ``MAXSS``
   :Examples: ``--mdm-max MAX``

Select the type of the max operation to use in the demodulator.

Description of the allowed values:

+-----------+-----------------------+
| Value     | Description           |
+===========+=======================+
| ``MAX``   | |mdm-max_descr_max|   |
+-----------+-----------------------+
| ``MAXL``  | |mdm-max_descr_maxl|  |
+-----------+-----------------------+
| ``MAXS``  | |mdm-max_descr_maxs|  |
+-----------+-----------------------+
| ``MAXSS`` | |mdm-max_descr_maxss| |
+-----------+-----------------------+

.. |mdm-max_descr_max| replace:: TODO VALUE MAX
.. |mdm-max_descr_maxl| replace:: TODO VALUE MAXL
.. |mdm-max_descr_maxs| replace:: TODO VALUE MAXS
.. |mdm-max_descr_maxss| replace:: TODO VALUE MAXSS


.. _mdm-mdm-no-sig2:

``--mdm-no-sig2``
"""""""""""""""""


Turn off the division by sigma square in the demodulator.

.. _mdm-mdm-psi:

``--mdm-psi``
"""""""""""""

   :Type: text
   :Allowed values: ``PSI0`` ``PSI1`` ``PSI2`` ``PSI3``
   :Examples: ``--mdm-psi PSI0``

Select the type of the psi function to use in the SCMA demodulator.

Description of the allowed values:

+----------+----------------------+
| Value    | Description          |
+==========+======================+
| ``PSI0`` | |mdm-psi_descr_psi0| |
+----------+----------------------+
| ``PSI1`` | |mdm-psi_descr_psi1| |
+----------+----------------------+
| ``PSI2`` | |mdm-psi_descr_psi2| |
+----------+----------------------+
| ``PSI3`` | |mdm-psi_descr_psi3| |
+----------+----------------------+

.. |mdm-psi_descr_psi0| replace:: TODO VALUE PSI0
.. |mdm-psi_descr_psi1| replace:: TODO VALUE PSI1
.. |mdm-psi_descr_psi2| replace:: TODO VALUE PSI2
.. |mdm-psi_descr_psi3| replace:: TODO VALUE PSI3


.. _mdm-mdm-type:

``--mdm-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BPSK`` ``CPM`` ``OOK`` ``PAM`` ``PSK`` ``QAM`` ``SCMA`` ``USER``
   :Examples: ``--mdm-type BPSK``

Type of the modulation to use in the simulation.

Description of the allowed values:

+----------+-----------------------+
| Value    | Description           |
+==========+=======================+
| ``BPSK`` | |mdm-type_descr_bpsk| |
+----------+-----------------------+
| ``CPM``  | |mdm-type_descr_cpm|  |
+----------+-----------------------+
| ``OOK``  | |mdm-type_descr_ook|  |
+----------+-----------------------+
| ``PAM``  | |mdm-type_descr_pam|  |
+----------+-----------------------+
| ``PSK``  | |mdm-type_descr_psk|  |
+----------+-----------------------+
| ``QAM``  | |mdm-type_descr_qam|  |
+----------+-----------------------+
| ``SCMA`` | |mdm-type_descr_scma| |
+----------+-----------------------+
| ``USER`` | |mdm-type_descr_user| |
+----------+-----------------------+

.. |mdm-type_descr_bpsk| replace:: TODO VALUE BPSK
.. |mdm-type_descr_cpm| replace:: TODO VALUE CPM
.. |mdm-type_descr_ook| replace:: TODO VALUE OOK
.. |mdm-type_descr_pam| replace:: TODO VALUE PAM
.. |mdm-type_descr_psk| replace:: TODO VALUE PSK
.. |mdm-type_descr_qam| replace:: TODO VALUE QAM
.. |mdm-type_descr_scma| replace:: TODO VALUE SCMA
.. |mdm-type_descr_user| replace:: TODO VALUE USER


.. _mdm-mdm-ups:

``--mdm-ups``
"""""""""""""

   :Type: integer
   :Examples: ``--mdm-ups 1``

Select the symbol sampling factor (default is 1).

