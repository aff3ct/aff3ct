.. _enc-common-encoder-parameters:

Common Encoder parameters
-------------------------

This section describes the parameters common to all encoders.

.. _enc-common-enc-type:

``--enc-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``AZCW`` ``COSET`` ``USER``
   :Examples: ``--enc-type AZCW``

|factory::Encoder::p+type|

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``AZCW``  | |enc-type_descr_azcw|  |
+-----------+------------------------+
| ``COSET`` | |enc-type_descr_coset| |
+-----------+------------------------+
| ``USER``  | |enc-type_descr_user|  |
+-----------+------------------------+

.. |enc-type_descr_azcw| replace:: Select the |AZCW| encoder which is optimized
   to encode :math:`K` information bits all set to 0.
.. |enc-type_descr_coset| replace:: Select the coset encoder (see the
   :ref:`sim-sim-coset` parameter), this encoder add random bits from
   :math:`X_K` to :math:`X_N`.
.. |enc-type_descr_user| replace:: Read the codewords from a given file, the
   path can be set with the :ref:`enc-common-enc-path` parameter.

.. tip:: The |AZCW| encoder allows to have a working communication chain
   without implementing an encoder. This technique can also reduce the
   simulation time especially when the *encode* task is time consuming.

.. danger:: Be careful, the |AZCW| technique can lead to unexpected behaviors
   with broken decoders.

.. note:: Only use the ``COSET`` encoder if know what you are doing. This
  encoder is set by default when the simulation is run with the
  :ref:`sim-sim-coset` parameter.

.. note:: For the ``USER`` type, when the number of simulated frames exceeds the
   number of codewords contained in the files, the codewords are replayed from
   the beginning of the file and this is repeated until the end of the
   simulation.

.. _enc-common-enc-path:

``--enc-path``
""""""""""""""

   :Type: file
   :Rights: read only
   :Examples: ``--enc-path example/path/to/the/right/file``

|factory::Encoder::p+path|

An |ASCII| file is expected:

.. code-block:: bash

   # 'F' has to be replaced by the number of contained frames.
   F

   # 'N' has to be replaced by the codeword size.
   N

   # a sequence of 'F * N' bits (separated by spaces)
   B_0 B_1 B_2 B_3 B_4 B_5 [...] B_{(F*N)-1}

.. _enc-common-enc-start-idx:

``--enc-start-idx``
"""""""""""""""""""

   :Type: integer
   :Examples: ``--enc-start-idx 1``

|factory::Encoder::p+start-idx|