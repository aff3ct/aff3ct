.. _dec-polar-decoder-parameters:

Polar Decoder parameters
------------------------

.. _dec-polar-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``SC`` ``SCAN`` ``SCF`` ``SCL`` ``SCL_MEM`` ``ASCL``
                    ``ASCL_MEM`` ``CHASE`` ``ML``
   :Default: ``SC``
   :Examples: ``--dec-type ASCL``

|factory::Decoder::p+type,D|

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``SC``       | Select the original |SC| algorithm from :cite:`Arikan2009`.   |
+--------------+---------------------------------------------------------------+
| ``SCAN``     | Select the |SCAN| algorithm from :cite:`Fayyaz2014`.          |
+--------------+---------------------------------------------------------------+
| ``SCF``      | Select the |SCF| algorithm from :cite:`Afisiadis2014`.        |
+--------------+---------------------------------------------------------------+
| ``SCL``      | Select the |SCL| algorithm from :cite:`Tal2011`, also support |
|              | the improved |CA|-|SCL| algorithm.                            |
+--------------+---------------------------------------------------------------+
| ``SCL_MEM``  | Select the |SCL| algorithm, same as the previous one but with |
|              | an implementation optimized to reduce the memory footprint.   |
+--------------+---------------------------------------------------------------+
| ``ASCL``     | Select the |A-SCL| algorithm from :cite:`Li2012`, |PA-SCL|    |
|              | and |FA-SCL| variants from :cite:`Leonardon2017` are          |
|              | available (see the :ref:`dec-polar-dec-partial-adaptive`      |
|              | parameter).                                                   |
+--------------+---------------------------------------------------------------+
| ``ASCL_MEM`` | Select the |A-SCL| algorithm, same as the previous one but    |
|              | with an implementation optimized to reduce the memory         |
|              | footprint.                                                    |
+--------------+---------------------------------------------------------------+
| ``CHASE``    | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+
| ``ML``       | See the common :ref:`dec-common-dec-type` parameter.          |
+--------------+---------------------------------------------------------------+

.. _dec-polar-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``NAIVE`` ``FAST``
   :Default: ``FAST``
   :Examples: ``--dec-implem FAST``

|factory::Decoder::p+implem|

Description of the allowed values:

+-----------+--------------------------+
| Value     | Description              |
+===========+==========================+
| ``NAIVE`` | |dec-implem_descr_naive| |
+-----------+--------------------------+
| ``FAST``  | |dec-implem_descr_fast|  |
+-----------+--------------------------+

.. |dec-implem_descr_naive| replace:: Select the naive implementation which is
   typically slow (not supported by the |A-SCL| decoders).
.. |dec-implem_descr_fast| replace:: Select the fast implementation, available
   only for the |SC|, |SCL|, |SCL|-MEM, |A-SCL| and |A-SCL|-MEM decoders.

.. warning:: ``FAST`` implementations only support systematic encoding of Polar
   codes.

.. note:: The |SC| ``FAST`` implementation has been presented in
   :cite:`LeGal2015a,Cassagne2015c,Cassagne2016b`.

.. note:: The |SCL|, |CA|-|SCL| and |A-SCL| ``FAST`` implementations
   have been presented in :cite:`Leonardon2017`.

.. _dec-polar-dec-simd:

``--dec-simd``
""""""""""""""

   :Type: text
   :Allowed values: ``INTER`` ``INTRA``
   :Examples: ``--dec-simd INTER``

|factory::Decoder_polar::p+simd|

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``INTER`` | Select the inter-frame strategy, only available for the |SC|     |
|           | ``FAST`` decoder (see                                            |
|           | :cite:`LeGal2015a,Cassagne2015c,Cassagne2016b`).                 |
+-----------+------------------------------------------------------------------+
| ``INTRA`` | Select the intra-frame strategy, only available for the |SC|     |
|           | (see :cite:`Cassagne2015c,Cassagne2016b`),                       |
|           | |SCL| and |A-SCL| decoders (see in :cite:`Leonardon2017`).       |
+-----------+------------------------------------------------------------------+

.. note:: In **the intra-frame strategy**, |SIMD| units process several LLRs in
   parallel within a single frame decoding. This approach is efficient in the
   upper layers of the tree and in the specialized nodes, but more limited in
   the lowest layers where the computation becomes more sequential. In **the
   inter-frame strategy**, SIMD units decodes several independent frames in
   parallel in order to saturate the |SIMD| unit. This approach improves the
   throughput of the decoder but requires to load several frames before starting
   to decode, increasing both the decoding latency and the decoder memory
   footprint.

.. note:: When the inter-frame |SIMD| strategy is set, the simulator will run
   with the right number of frames depending on the |SIMD| length. This number
   of frames can be manually set with the :ref:`sim-sim-inter-fra` parameter. Be
   aware that running the simulator with the :ref:`sim-sim-inter-fra` parameter
   set to 1 and the :ref:`dec-polar-dec-simd` parameter set to ``INTER`` will
   completely be counterproductive and will lead to no throughput improvements.

.. _dec-polar-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Examples: ``--dec-ite 1``

|factory::Decoder_polar::p+ite,i|

.. _dec-polar-dec-flips:

``--dec-flips``
"""""""""""""""

   :Type: integer
   :Examples: ``--dec-flips 1``

|factory::Decoder::p+flips|

Corresponds to the ``T`` parameter of the |SCF| decoding alogorithm
:cite:`Afisiadis2014`.

.. _dec-polar-dec-lists:

``--dec-lists, -L``
"""""""""""""""""""

   :Type: integer
   :Default: ``8``
   :Examples: ``--dec-lists 1``

|factory::Decoder_polar::p+lists,L|

.. _dec-polar-dec-partial-adaptive:

``--dec-partial-adaptive``
""""""""""""""""""""""""""

|factory::Decoder_polar::p+partial-adaptive|

.. _dec-polar-dec-polar-nodes:

``--dec-polar-nodes``
"""""""""""""""""""""

   :Type: text
   :Default: ``"{R0,R1,R0L,REP,REPL,SPC}"``
   :Examples: ``--dec-polar-nodes "{R0,R1}"``

|factory::Decoder_polar::p+polar-nodes|

Here are the available node types (or rules):

   * ``R0``:   Rate 0, all the bits are frozen,
   * ``R0L``:  Rate 0 left, the next left node in the tree is a ``R0``,
   * ``R1``:   Rate 1, all the bits are information bits,
   * ``REP``:  Repetition code,
   * ``REPL``: Repetition left, the next left node in the tree is ``REP``,
   * ``SPC``:  |SPC| code.

Those node types are well explained in :cite:`Sarkis2014a,Cassagne2015c`.
It is also possible to specify the level in the tree where the node type will
be recognized. For instance, the following value
``"{R0,R1,R0L,REP_2-8,REPL,SPC_4+}"`` matches:

   * ``R0``: all the Rate 0 nodes,
   * ``R0L``: all the Rate 0 left nodes,
   * ``R1``: all the Rate 1 nodes,
   * ``REP_2-8``: the repetition nodes with a size between 2 and 8 (including 2
     and 8),
   * ``REPL``: all the repetition left nodes (will be automatically limited by
     the ``REP_2-8`` rule),
   * ``SPC_4+``: the |SPC| nodes with a size equal or higher than 4.

To disable the tree cuts you can use the following value: ``"{R0_1,R1_1}"``.

References
""""""""""

.. bibliography:: references_dec.bib
   :labelprefix: Plrd-