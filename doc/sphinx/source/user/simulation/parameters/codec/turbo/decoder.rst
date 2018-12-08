.. _dec-turbo-decoder-parameters:

Turbo Decoder parameters
------------------------

.. _dec-turbo-dec-type:

``--dec-type, -D``
""""""""""""""""""

   :Type: text
   :Allowed values: ``TURBO`` ``CHASE`` ``ML``
   :Default: ``TURBO``
   :Examples: ``--dec-type CHASE``

**STRING**: |factory::Decoder::parameters::p+type,D|

Select the algorithm you want to decode the codeword.

Description of the allowed values:

+-----------+------------------------+
| Value     | Description            |
+===========+========================+
| ``TURBO`` | |dec-type_descr_turbo| |
+-----------+------------------------+
| ``CHASE`` | |dec-type_descr_chase| |
+-----------+------------------------+
| ``ML``    | |dec-type_descr_ml|    |
+-----------+------------------------+

.. |dec-type_descr_turbo| replace:: Select the Turbo decoder, the two
   sub-decoders are from the |RSC| code family.
.. |dec-type_descr_chase| replace:: See the common :ref:`dec-common-dec-type`
   parameter.
.. |dec-type_descr_ml| replace:: See the common :ref:`dec-common-dec-type`
   parameter.

.. _dec-turbo-dec-implem:

``--dec-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST``
   :Default: ``FAST``
   :Examples: ``--dec-implem FAST``

**STRING**: |factory::Decoder::parameters::p+implem|

Select the decoder implementation.

Description of the allowed values:

+-----------+------------------------------------------------------------------+
| Value     | Description                                                      |
+===========+==================================================================+
| ``STD``   | Select the |STD| implemenation.                                  |
+-----------+------------------------------------------------------------------+
| ``FAST``  | Select the fast implemenation from :cite:`Cassagne2016a`.        |
+-----------+------------------------------------------------------------------+

.. _dec-turbo-dec-sub-type:

``--dec-sub-type, -D``
""""""""""""""""""""""

Please refer to the |RSC| :ref:`dec-rsc-dec-type` parameter.

.. _dec-turbo-dec-sub-implem:

``--dec-sub-implem``
""""""""""""""""""""

Please refer to the |RSC| :ref:`dec-rsc-dec-implem` parameter.

.. _dec-turbo-dec-sub-simd:

``--dec-sub-simd``
""""""""""""""""""

Please refer to the |RSC| :ref:`dec-rsc-dec-simd` parameter.

.. _dec-turbo-dec-crc-start:

``--dec-crc-start``
"""""""""""""""""""

   :Type: integer
   :Default: ``2``
   :Examples: ``--dec-fnc-crc-ite 1``

**STRING**: |factory::Decoder_turbo::parameters::p+crc-start|

Set the first iteration to start the |CRC| checking.

.. note:: This parameter requires the Turbo code to be concatenated with a |CRC|
   to work, see the :ref:`crc-crc-parameters`.

.. _dec-turbo-dec-fnc:

``--dec-fnc``
"""""""""""""

**STRING**: |factory::Flip_and_check::parameters::p+|

Enable the |FNC| post processing technique from :cite:`Tonnellier2016b`.

.. note:: This parameter requires the Turbo code to be concatenated with a |CRC|
   to work, see the :ref:`crc-crc-parameters`.

.. _dec-turbo-dec-fnc-ite-min:

``--dec-fnc-ite-m``
"""""""""""""""""""

   :Type: integer
   :Default: ``3``
   :Examples: ``--dec-fnc-ite-m 2``

**STRING**: |factory::Flip_and_check::parameters::p+ite-m|

Set the first iteration at which the |FNC| is used (c.f the
:ref:`dec-turbo-dec-fnc` parameter).

.. _dec-turbo-dec-fnc-ite-max:

``--dec-fnc-ite-M``
"""""""""""""""""""

   :Type: integer
   :Default: ``10``
   :Examples: ``--dec-fnc-ite-M 6``

**STRING**: |factory::Flip_and_check::parameters::p+ite-M|

Set the last iteration at which the |FNC| is used (c.f the
:ref:`dec-turbo-dec-fnc` parameter).

.. _dec-turbo-dec-fnc-ite-s:

``--dec-fnc-ite-s``
"""""""""""""""""""

   :Type: integer
   :Default: ``1``
   :Examples: ``--dec-fnc-ite-s 2``

**STRING**: |factory::Flip_and_check::parameters::p+ite-s|

Set the iteration step for the |FNC| technique (c.f the
:ref:`dec-turbo-dec-fnc` parameter).

.. _dec-turbo-dec-fnc-q:

``--dec-fnc-q``
"""""""""""""""

   :Type: integer
   :Default: ``10``
   :Examples: ``--dec-fnc-q 6``

**STRING**: |factory::Flip_and_check::parameters::p+q|

Set the search space for the |FNC| technique (c.f the
:ref:`dec-turbo-dec-fnc` parameter).

.. _dec-turbo-dec-ite:

``--dec-ite, -i``
"""""""""""""""""

   :Type: integer
   :Default: ``6``
   :Examples: ``--dec-ite 8``

**STRING**: |factory::Decoder_turbo::parameters::p+ite,i|

Set the maximal number of iterations in the Turbo decoder. If the Turbo code
is concatenated with a |CRC| and if the |CRC| is checked, the decoder can stop
before making all the iterations.

.. _dec-turbo-dec-sc:

``--dec-sc``
""""""""""""

**STRING**: |factory::Decoder_turbo::parameters::p+sc|

Enables the |SCo| decoder from :cite:`Tonnellier2017` (in French).

.. note:: This parameter requires the Turbo code to be concatenated with a |CRC|
   to work, see the :ref:`crc-crc-parameters`.

.. _dec-turbo-dec-sf-type:

``--dec-sf-type``
"""""""""""""""""

   :Type: text
   :Allowed values: ``ADAPTIVE`` ``ARRAY`` ``CST`` ``LTE`` ``LTE_VEC``
   :Examples: | ``--dec-sf-type ADAPTIVE``
              | ``--dec-sf-type CST 0.5``

**STRING**: |factory::Scaling_factor::parameters::p+type|

Select a |SF| to be applied to the extrinsic values after each half iteration
:cite:`Vogt2000`. This is especially useful with the max-log-|MAP| sub-decoders
(|BCJR| with the :math:`\max` approximation): the |SF| helps to recover a part
of the decoding performance loss compare to the |MAP| algorithm (|BCJR| with the
:math:`\max^*` operator).

Description of the allowed values:

+--------------+---------------------------------------------------------------+
| Value        | Description                                                   |
+==============+===============================================================+
| ``ADAPTIVE`` | Select the adaptive |SF|, for the first and second iterations |
|              | a |SF| of 0.5 is applied, for the other iterations the |SF|   |
|              | is 0.85.                                                      |
+--------------+---------------------------------------------------------------+
| ``ARRAY``    | Select an hard-coded array of |SFs| (c.f.                     |
|              | :numref:`tab_turbo_dec_sf_array`).                            |
+--------------+---------------------------------------------------------------+
| ``CST``      | Set the same |SF| to be applied for each iterations.          |
+--------------+---------------------------------------------------------------+
| ``LTE``      | Select a 0.75 |SF|.                                           |
+--------------+---------------------------------------------------------------+
| ``LTE_VEC``  | Select a 0.75 vectorized |SF| (faster than ``LTE``), used in  |
|              | :cite:`Cassagne2016a`.                                        |
+--------------+---------------------------------------------------------------+

.. _tab_turbo_dec_sf_array:

.. table:: Hard-coded array of |SFs|.
   :align: center

   +-----------+--------+
   | Iteration | Value  |
   +===========+========+
   | 1         | 0.15   |
   +-----------+--------+
   | 2         | 0.25   |
   +-----------+--------+
   | 3         | 0.30   |
   +-----------+--------+
   | 4         | 0.40   |
   +-----------+--------+
   | 5         | 0.70   |
   +-----------+--------+
   | 6         | 0.80   |
   +-----------+--------+
   | 7         | 0.90   |
   +-----------+--------+
   | 8         | 0.95   |
   +-----------+--------+

.. _dec-turbo-dec-sub-max:

``--dec-sub-max``
"""""""""""""""""

Please refer to the |RSC| :ref:`dec-rsc-dec-max` parameter.

References
""""""""""

.. bibliography:: references.bib