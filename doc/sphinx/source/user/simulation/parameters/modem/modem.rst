.. _mdm-modem-parameters:

Modem parameters
----------------

|AFF3CT| comes with a set of predefined |modems|. A |modem| transforms a
sequence of bits into a suitable form for the transmission on a physical medium.
In the |AFF3CT| "philosophy", the |modem| is a **module** containing three
**tasks**: *modulate*, *filter* and *demodulate* (read the
:ref:`user_simulation_overview_philosophy` section for more information about
modules and tasks).

.. _mdm-mdm-type:

``--mdm-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BPSK`` ``CPM`` ``OOK`` ``PAM`` ``PSK`` ``QAM`` ``SCMA``
                    ``USER``
   :Default: ``BPSK``
   :Examples: ``--mdm-type SCMA``

Select the modulation type.

Description of the allowed values:

.. _Continuous Phase Modulation: https://en.wikipedia.org/wiki/Continuous_phase_modulation
.. _On-Off Keying: https://en.wikipedia.org/wiki/On-off_keying
.. _Pulse-Amplitude Modulation: https://en.wikipedia.org/wiki/Pulse-Amplitude_Modulation
.. _Phase-Shift Keying: https://en.wikipedia.org/wiki/Phase-shift_keying
.. _Quadrature-Amplitude Modulation: https://en.wikipedia.org/wiki/Quadrature_amplitude_modulation

+----------+-------------------------------------------------------------------+
| Value    | Description                                                       |
+==========+===================================================================+
| ``BPSK`` | Select a |BPSK| modulation.                                       |
+----------+-------------------------------------------------------------------+
| ``CPM``  | Select a `Continuous Phase Modulation`_ (|CPM|)                   |
|          | :cite:`Aulin1981a,Aulin1981b`.                                    |
+----------+-------------------------------------------------------------------+
| ``OOK``  | Select an `On-Off Keying`_ (|OOK|) modulation.                    |
+----------+-------------------------------------------------------------------+
| ``PAM``  | Select a `Pulse-Amplitude Modulation`_ (|PAM|).                   |
+----------+-------------------------------------------------------------------+
| ``PSK``  | Select a `Phase-Shift Keying`_ (|PSK|) modulation.                |
+----------+-------------------------------------------------------------------+
| ``QAM``  | Select a rectangular `Quadrature-Amplitude Modulation`_ (|QAM|).  |
+----------+-------------------------------------------------------------------+
| ``SCMA`` | Select a Sparse Code Multiple Access (|SCMA|) modulation          |
|          | :cite:`Nikopour2013`.                                             |
+----------+-------------------------------------------------------------------+
| ``USER`` | Select a user defined constellation (to use with the              |
|          | :ref:`mdm-mdm-const-path` parameter).                             |
+----------+-------------------------------------------------------------------+

.. TODO: describe completely the implemented models, such as the constellation
  mapping (ex: PSK has always GRAY mapping) or what is done if sig2 is disabled.
  Describe the different demodulator of OOK when AWGN, optical, BSC, BEC

.. _mdm-mdm-implem:

``--mdm-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD``
   :Default: ``STD``
   :Examples: ``--mdm-implem FAST``

Select the implementation of the |modem|.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |mdm-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |mdm-implem_descr_fast| |
+----------+-------------------------+

.. |mdm-implem_descr_std|  replace:: Select a standard implementation working
   for any |modem|.
.. |mdm-implem_descr_fast| replace:: Select a fast implementation, only
   available for the |BPSK| |modem| at this time.

.. _mdm-mdm-bps:

``--mdm-bps``
"""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-bps 1``

Set the number of bits used to generate a symbol (|BPS|). This parameter has no
effect on the |BPSK| and |OOK| |modems| where the |BPS| is forced to 1. This is
the same for the |SCMA| |modem| where the |BPS| is forced to 3.

.. note:: For the |QAM| |modem|, only even |BPS| values are supported.

.. _mdm-mdm-const-path:

``--mdm-const-path``
""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Examples: ``--mdm-const-path ../conf/mod/16QAM_ANTI_GRAY.mod``

Give the path to the ordered modulation symbols (constellation), to use with
``USER`` |modem|.

An |ASCII| file is expected, for instance here is the definition of a 16-|QAM|
with an anti-Gray mapping (the lines starting with a ``#`` are ignored):

.. code-block:: bash

   # 0000
    3  3
   # 0001
   -3 -3
   # 0010
   -1  3
   # 0011
    1 -3
   # 0100
   -3  1
   # 0101
    3 -1
   # 0110
    1  1
   # 0111
   -1 -1
   # 1000
    1 -1
   # 1001
   -1  1
   # 1010
   -3 -1
   # 1011
    3  1
   # 1100
   -1 -3
   # 1101
    1  3
   # 1110
    3 -3
   # 1111
   -3  3

.. warning:: The :ref:`mdm-mdm-bps` parameter has to be set accordingly to the
   given constellation. In the previous example, ``--mdm-bps 4`` has be added
   to the command line.

.. _mdm-mdm-max:

``--mdm-max``
"""""""""""""

   :Type: text
   :Allowed values: ``MAXS`` ``MAXSS`` ``MAXL`` ``MAX``
   :Examples: ``--mdm-max MAX``

Select the approximation of the :math:`\max^*` operator used in the |PAM|,
|QAM|, |PSK|, |CPM| and user demodulators.

Description of the allowed values:

+-----------+-----------------------+
| Value     | Description           |
+===========+=======================+
| ``MAXS``  | |mdm-max_descr_maxs|  |
+-----------+-----------------------+
| ``MAXSS`` | |mdm-max_descr_maxss| |
+-----------+-----------------------+
| ``MAXL``  | |mdm-max_descr_maxl|  |
+-----------+-----------------------+
| ``MAX``   | |mdm-max_descr_max|   |
+-----------+-----------------------+

.. |mdm-max_descr_maxs|  replace:: :math:`\max^*(a,b) = \max(a,b) +
   \log(1 + \exp(-|a - b|))`.
.. |mdm-max_descr_maxss| replace:: :math:`\max^*(a,b) \approx \max(a,b) + d`
   with :math:`d = \begin{cases}
   0                         & \text{if } d >= 37\\
   \exp(-|a - b|)            & \text{if } 9 <= d < 37 \\
   \log(1 + \exp(-|a - b|))  & \text{else}
   \end{cases}`.
.. |mdm-max_descr_maxl|  replace:: :math:`\max^*(a,b) \approx \max(a,b) +
   \max(0, 0.301 - (0.5 |a - b|))`.
.. |mdm-max_descr_max|   replace:: :math:`\max^*(a,b) \approx \max(a,b)`.

``MAXS`` for *Max Star* is the exact :math:`\max^*` operator. ``MAXSS`` for
*Max Star Safe* allows to avoid numeric instabilities due the exponential
operation and the limited precision of the floating-point representation.
``MAXL`` for *Max Linear* is a linear approximation of the :math:`\max^*`
function. ``MAX`` for *Max* is the simplest :math:`\max^*` approximation with
only a :math:`\max` function.

.. _mdm-mdm-no-sig2:

``--mdm-no-sig2``
"""""""""""""""""

Turn off the division by :math:`\sigma^2` in the demodulator where
:math:`\sigma` is the Gaussian noise variance.

.. _mdm-mdm-cpm-k:

``--mdm-cpm-k``
"""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-cpm-k 1``

Set the |CPM| *index numerator*.

.. _mdm-mdm-cpm-p:

``--mdm-cpm-p``
"""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--mdm-cpm-p 1``

Set the |CPM| *index denominator*.

.. _mdm-mdm-cpm-L:

``--mdm-cpm-L``
"""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--mdm-cpm-L 1``

Set the |CPM| *pulse width* (also called *memory depth*).

.. _mdm-mdm-cpm-upf:

``--mdm-cpm-upf``
"""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-cpm-upf 1``

Select the symbol upsampling factor in the |CPM|.

.. _mdm-mdm-cpm-map:

``--mdm-cpm-map``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GRAY`` ``NATURAL``
   :Default: ``NATURAL``
   :Examples: ``--mdm-cpm-map GRAY``

Select the |CPM| *symbols mapping layout*.

Description of the allowed values:

+-------------+-----------------------------+
| Value       | Description                 |
+=============+=============================+
| ``GRAY``    | |mdm-cpm-map_descr_gray|    |
+-------------+-----------------------------+
| ``NATURAL`` | |mdm-cpm-map_descr_natural| |
+-------------+-----------------------------+

.. |mdm-cpm-map_descr_gray|    replace:: Gray code switching only one bit at
   a time from a symbol to the following.
.. |mdm-cpm-map_descr_natural| replace:: The natural binary code incrementing
   the value from a symbol to the next one.

.. _mdm-mdm-cpm-ws:

``--mdm-cpm-ws``
""""""""""""""""

   :Type: text
   :Allowed values: ``GMSK`` ``RCOS`` ``REC``
   :Default: ``GMSK``
   :Examples: ``--mdm-cpm-ws GMSK``

Select the |CPM| *wave shape*.

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

.. _Gaussian Minimum Shift Keying: https://en.wikipedia.org/wiki/Minimum-shift_keying#Gaussian_minimum-shift_keying

.. |mdm-cpm-ws_descr_gmsk| replace:: `Gaussian Minimum Shift Keying`_.
.. |mdm-cpm-ws_descr_rcos| replace:: Raised COSinus.
.. |mdm-cpm-ws_descr_rec|  replace:: RECtangular.

.. _mdm-mdm-cpm-std:

``--mdm-cpm-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GSM``
   :Examples: ``--mdm-cpm-std GSM``

Set the |CPM| parameters according to a standard.

Description of the allowed values:

+---------+--------------------------+---------------+---------------------------------+
| Value   | Parameter                | Value         | Description                     |
+=========+==========================+===============+=================================+
| ``GSM`` | | :ref:`mdm-mdm-bps`     | | 1           | | Bit per symbol.               |
|         | | :ref:`mdm-mdm-cpm-upf` | | 5           | | Upsampling factor.            |
|         | | :ref:`mdm-mdm-cpm-k`   | | 1           | | Modulation index numerator.   |
|         | | :ref:`mdm-mdm-cpm-p`   | | 2           | | Modulation index denominator. |
|         | | :ref:`mdm-mdm-cpm-L`   | | 3           | | Memory depth.                 |
|         | | :ref:`mdm-mdm-cpm-map` | | ``NATURAL`` | | Mapping layout.               |
|         | | :ref:`mdm-mdm-cpm-ws`  | | ``GMSK``    | | Wave shape.                   |
+---------+--------------------------+---------------+---------------------------------+

.. note:: When this parameter is used, if you set any of the other |modem|
   parameters, it will override the configuration from the standard.

.. _mdm-mdm-ite:

``--mdm-ite``
"""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-ite 5``

Set the number of iterations in the |SCMA| demodulator.

.. _mdm-mdm-psi:

``--mdm-psi``
"""""""""""""

   :Type: text
   :Allowed values: ``PSI0`` ``PSI1`` ``PSI2`` ``PSI3``
   :Examples: ``--mdm-psi PSI0``

Select the :math:`\psi` function used in the |SCMA| demodulator.

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

.. |mdm-psi_descr_psi0| replace:: :math:`\psi_0 = \exp\left(-\frac{|d|}{n_0}\right)`
.. |mdm-psi_descr_psi1| replace:: :math:`\psi_1 \approx \psi_0 \approx \frac{1}{|d| + n_0}`
.. |mdm-psi_descr_psi2| replace:: :math:`\psi_2 \approx \psi_0 \approx \frac{1}{8. |d|^2 + n_0}`
.. |mdm-psi_descr_psi3| replace:: :math:`\psi_3 \approx \psi_0 \approx \frac{1}{4. |d|^2 + n_0}`

Where |n0_equation|

.. |n0_equation| replace:: :math:`n_0 =
  \begin{cases}
  1           & \text{if } \sigma^2 \text{ is disabled}\\
  4 \sigma^2  & \text{else}
  \end{cases}`.

See the :ref:`mdm-mdm-no-sig2` parameter to disable the division by
:math:`\sigma^2`.

.. _mdm-mdm-rop-est:

``--mdm-rop-est``
"""""""""""""""""

   :Type: integer
   :Default: 0
   :Examples: ``--mdm-rop-est 256``

Set the number of known bits for the |ROP| estimation in the |OOK| demodulator
on an optical channel. The estimation is done from a known set of bits
that is the output of the modulation.

If left to 0, the demodulation is done with the exact applied |ROP| in the
channel.

References
""""""""""

.. bibliography:: references.bib