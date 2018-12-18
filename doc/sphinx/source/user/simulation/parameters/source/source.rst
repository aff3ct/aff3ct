.. _src-source-parameters:

Source parameters
-----------------

The source generates :math:`K` information bits: it is the simulation starting
point.

.. _src-src-type:

``--src-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AZCW`` ``RAND`` ``USER``
   :Default: ``RAND``
   :Examples: ``--src-type AZCW``

|factory::Source::parameters::p+type|

Description of the allowed values:

+----------+-------------------------------------------------------------------+
| Value    | Description                                                       |
+==========+===================================================================+
| ``AZCW`` | Set all the information bits to 0.                                |
+----------+-------------------------------------------------------------------+
| ``RAND`` | Generate randomly the information bits based on the |MT 19937|    |
|          | |PRNG| :cite:`Matsumoto1998`.                                     |
+----------+-------------------------------------------------------------------+
| ``USER`` | Read the information bits from a given file, the path can be set  |
|          | with the :ref:`src-src-path` parameter.                           |
+----------+-------------------------------------------------------------------+

.. note:: For the ``USER`` type, when the number of simulated frames exceeds the
   number of frames contained in the files, the frames are replayed from the
   beginning of the file and this is repeated until the end of the simulation.

.. _src-src-implem:

``--src-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Default: ``STD``
   :Examples: ``--src-implem FAST``

|factory::Source::parameters::p+implem|

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |src-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |src-implem_descr_fast| |
+----------+-------------------------+

.. |src-implem_descr_std|  replace:: Standard implementation working for any
   source type.
.. |src-implem_descr_fast| replace:: Fast implementation, only available for the
   ``RAND`` source type.

.. _src-src-fra:

``--src-fra, -F``
"""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--src-fra 8``

|factory::Source::parameters::p+fra,F|

The default behavior is to generate one frame at a time. This parameter enables
to process more than one frame when the *generate* task (from the source module)
is called.

The number of frames consumed and produced when a task is executed is called the
**inter frame level** or |IFL|. Setting the |IFL| in the source module will
automatically affect the |IFL| level in all the other simulation modules (c.f.
:numref:`fig_src_fra_inter`).

.. _fig_src_fra_inter:

.. figure:: images/src_fra_inter.png
   :figwidth: 70 %
   :align: center

   3-way inter frame level in the communication chain.

The |IFL| also allows multi-user configurations to be simulated (see
:numref:`fig_src_fra_multi_user`). This configurations is used when using |SCMA|
modulation (see the :ref:`mdm-mdm-type` ``SCMA`` parameter).

.. _fig_src_fra_multi_user:

.. figure:: images/src_fra_multi_user.png
   :figwidth: 70 %
   :align: center

   3-way inter frame level with multi-user channel in the communication chain.

.. note:: **For short frames**, increase the |IFL| can **increase the
  simulation throughput**, it can hide task call overheads.

.. note:: **For large frames**, increase the |IFL| can **decrease the
  simulation throughput** due the CPU cache size limitation.

.. _src-src-path:

``--src-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--src-path conf/src/GSM-LDPC_2112.src``

|factory::Source::parameters::p+path|

An |ASCII| file is expected:

.. code-block:: bash

   # 'F' has to be replaced by the number of contained frames.
   F

   # 'K' has to be replaced by the number of information bits.
   K

   # a sequence of 'F * K' bits (separated by spaces)
   B_0 B_1 B_2 B_3 B_4 B_5 [...] B_{(F*K)-1}

.. _src-src-start-idx:

``--src-start-idx``
"""""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--src-start-idx 42``

|factory::Source::parameters::p+start-idx|

References
""""""""""

.. bibliography:: references.bib
