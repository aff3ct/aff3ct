.. _chn-channel-parameters:

Channel parameters
------------------

.. _chn-chn-blk-fad:

``--chn-blk-fad``
"""""""""""""""""

   :type: text
   :Allowed values: ``FRAME`` ``NO`` ``ONETAP`` 
   :Examples: ``--chn-blk-fad FRAME``

Block fading policy for the RAYLEIGH channel.

Description of the allowed values:

+------------+----------------------------+
| Value      | Description                |
+============+============================+
| ``FRAME``  | |chn-blk-fad_descr_frame|  |
+------------+----------------------------+
| ``NO``     | |chn-blk-fad_descr_no|     |
+------------+----------------------------+
| ``ONETAP`` | |chn-blk-fad_descr_onetap| |
+------------+----------------------------+

.. |chn-blk-fad_descr_frame| replace:: TODO VALUE FRAME
.. |chn-blk-fad_descr_no| replace:: TODO VALUE NO
.. |chn-blk-fad_descr_onetap| replace:: TODO VALUE ONETAP


.. _chn-chn-gain-occur:

``--chn-gain-occur``
""""""""""""""""""""

   :type: integer
   :Examples: ``--chn-gain-occur 1``

The number of times a gain is used on consecutive symbols (used with "\\-\\-chn-type RAYLEIGH_USER").

.. _chn-chn-implem:

``--chn-implem``
""""""""""""""""

   :type: text
   :Allowed values: ``FAST`` ``STD`` 
   :Examples: ``--chn-implem FAST``

Select the implementation of the algorithm to generate noise.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``FAST`` | |chn-implem_descr_fast| |
+----------+-------------------------+
| ``STD``  | |chn-implem_descr_std|  |
+----------+-------------------------+

.. |chn-implem_descr_fast| replace:: TODO VALUE FAST
.. |chn-implem_descr_std| replace:: TODO VALUE STD


.. _chn-chn-path:

``--chn-path``
""""""""""""""

   :type: file
   :Rights: read/write
   :Examples: ``--chn-path example/path/to/the/right/file``

Path to a noisy file, to use with "\\-\\-chn-type USER,OPTICAL" or to a gain file (used with "\\-\\-chn-type RAYLEIGH_USER").

.. _chn-chn-type:

``--chn-type``
""""""""""""""

   :type: text
   :Allowed values: ``AWGN`` ``BEC`` ``BSC`` ``NO`` ``OPTICAL`` ``RAYLEIGH`` ``RAYLEIGH_USER`` ``USER`` ``USER_ADD`` 
   :Examples: ``--chn-type AWGN``

Type of the channel to use in the simulation ('USER' has an output got from a file when 'USER_ADD' has an additive noise got from a file).

Description of the allowed values:

+-------------------+--------------------------------+
| Value             | Description                    |
+===================+================================+
| ``AWGN``          | |chn-type_descr_awgn|          |
+-------------------+--------------------------------+
| ``BEC``           | |chn-type_descr_bec|           |
+-------------------+--------------------------------+
| ``BSC``           | |chn-type_descr_bsc|           |
+-------------------+--------------------------------+
| ``NO``            | |chn-type_descr_no|            |
+-------------------+--------------------------------+
| ``OPTICAL``       | |chn-type_descr_optical|       |
+-------------------+--------------------------------+
| ``RAYLEIGH``      | |chn-type_descr_rayleigh|      |
+-------------------+--------------------------------+
| ``RAYLEIGH_USER`` | |chn-type_descr_rayleigh_user| |
+-------------------+--------------------------------+
| ``USER``          | |chn-type_descr_user|          |
+-------------------+--------------------------------+
| ``USER_ADD``      | |chn-type_descr_user_add|      |
+-------------------+--------------------------------+

.. |chn-type_descr_awgn| replace:: TODO VALUE AWGN
.. |chn-type_descr_bec| replace:: TODO VALUE BEC
.. |chn-type_descr_bsc| replace:: TODO VALUE BSC
.. |chn-type_descr_no| replace:: TODO VALUE NO
.. |chn-type_descr_optical| replace:: TODO VALUE OPTICAL
.. |chn-type_descr_rayleigh| replace:: TODO VALUE RAYLEIGH
.. |chn-type_descr_rayleigh_user| replace:: TODO VALUE RAYLEIGH_USER
.. |chn-type_descr_user| replace:: TODO VALUE USER
.. |chn-type_descr_user_add| replace:: TODO VALUE USER_ADD


