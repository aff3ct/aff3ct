.. _mdm-modem-parameters:

Modem parameters
----------------

The Modem is a module with three parts in one:
   * the **modulation** maps the input binary frame into symbols to send the
     data through the channel
   * the **filtering** gives event probabilities to the received waveforms
   * the **demodulation** unmaps symbols or waveforms and computes for every bit
     a :abbr:`LLR (Log-Likelihood Ratio)`

.. note:: The filtering step is used only by the ``CPM`` Modem as it is the only
          |AFF3CT|'s modulation using waveforms.

.. note:: A negative LLR represents a bit at 1, and therefore when positive a
          bit at 0. It is a more convenient choice as the hard decision matches
          directly with the sign bit.


.. _mdm-mdm-type:

``--mdm-type``
""""""""""""""

   :Type: text
   :Allowed values: ``BPSK`` ``CPM`` ``OOK`` ``PAM`` ``PSK`` ``QAM`` ``SCMA``
                    ``USER``
   :Default: ``BPSK``
   :Examples: ``--mdm-type SCMA``

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

.. _Continuous Phase Modulation: https://en.wikipedia.org/wiki/Continuous_phase_modulation
.. _On-Off Keying: https://en.wikipedia.org/wiki/On-off_keying
.. _Pulse-Amplitude Modulation: https://en.wikipedia.org/wiki/Pulse-Amplitude_Modulation
.. _Phase-Shift Keying: https://en.wikipedia.org/wiki/Phase-shift_keying
.. _Quadrature-Amplitude Modulation: https://en.wikipedia.org/wiki/Quadrature_amplitude_modulation
.. _SubCarrier Multiple Access: https://en.wikipedia.org/wiki/Subcarrier_multiplexing

.. |mdm-type_descr_bpsk| replace:: The simple 2-PSK that has an optimized
   implementation
.. |mdm-type_descr_cpm|  replace:: The `Continuous Phase Modulation`_
.. |mdm-type_descr_ook|  replace:: The `On-Off Keying`_
.. |mdm-type_descr_pam|  replace:: The `Pulse-Amplitude Modulation`_
.. |mdm-type_descr_psk|  replace:: The `Phase-Shift Keying`_
.. |mdm-type_descr_qam|  replace:: The Rectangular `Quadrature-Amplitude Modulation`_
.. |mdm-type_descr_scma| replace:: The `SubCarrier Multiple Access`_
.. |mdm-type_descr_user| replace:: A user defined constellation given in a file
   through :ref:`mdm-mdm-const-path`


.. TODO: describe completely the implemented models, such as the constellation
  mapping (ex: PSK has always GRAY mapping) or what is done if sig2 is disabled.
  Describe the different demodulator of OOK when AWGN, optical, BSC, BEC

.. _mdm-mdm-bps:

``--mdm-bps``
"""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-bps 1``

Selects the number of bits per symbol, ie. the number of bits used to generate a
symbol. You can't change this value for ``BPSK`` and ``OOK`` types as it is
forced to 1. Likewise, it is forced to 3 with ``SCMA``.

.. note:: For ``QAM`` type, only even values of bits per symbol are supported.

.. _mdm-mdm-const-path:

``--mdm-const-path``
""""""""""""""""""""

   :Type: file
   :Rights: read/write
   :Examples: ``--mdm-const-path ../conf/mod/16QAM_ANTI_GRAY.mod``

Path to the ordered modulation symbols (constellation), to use with ``USER``
type modulation".

.. _mdm-mdm-ups:

``--mdm-ups``
"""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-ups 1``

Selects the symbol sampling factor.

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
| ``STD``  | |mdm-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |mdm-implem_descr_fast| |
+----------+-------------------------+

.. |mdm-implem_descr_std| replace:: A standard implementation working for any
   modem type.
.. |mdm-implem_descr_fast| replace:: A much faster method using |SIMD| but *only
   for* ``BPSK`` *type*.

.. _mdm-mdm-max:

``--mdm-max``
"""""""""""""

   :Type: text
   :Allowed values: ``MAX`` ``MAXL`` ``MAXS`` ``MAXSS``
   :Examples: ``--mdm-max MAX``

Selects the mathematical form of the maximum operation applied in the ``PAM``'s,
``QAM``'s, ``PSK``'s, ``USER``'s and ``CPM``'s  demodulator.

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

.. |mdm-max_descr_max|   replace:: Standard MAX: :math:`\max(a,b)`
.. |mdm-max_descr_maxl|  replace:: Linear MAX: :math:`\max(a,b) +
   \max(0, 0.301 - (0.5 |a - b|))`
.. |mdm-max_descr_maxs|  replace:: Star MAX: :math:`\max(a,b) +
   \log(1 + \exp(-|a - b|))`
.. |mdm-max_descr_maxss| replace:: Safe Star MAX: :math:`\max(a,b) + d` with
   :math:`d = \begin{cases}
   0                         & \quad \text{if } d >= 37\\
   \exp(-|a - b|)            & \quad \text{if } 9 <= d < 37 \\
   \log(1 + \exp(-|a - b|))  & \quad \text{else}
   \end{cases}`

.. _mdm-mdm-no-sig2:

``--mdm-no-sig2``
"""""""""""""""""


Turn off the division by sigma square in the demodulator. Sigma is the Gaussian
noise variance.

.. _mdm-mdm-cpm-k:

``--mdm-cpm-k``
"""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-cpm-k 1``

Sets the ``CPM``'s modulation index numerator.

.. _mdm-mdm-cpm-p:

``--mdm-cpm-p``
"""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--mdm-cpm-p 1``

Sets the ``CPM``'s modulation index denominator.

.. _mdm-mdm-cpm-L:

``--mdm-cpm-L``
"""""""""""""""

   :Type: integer
   :Default: 2
   :Examples: ``--mdm-cpm-L 1``

Sets the ``CPM``'s *pulse width* (also called *memory depth*).

.. _mdm-mdm-cpm-map:

``--mdm-cpm-map``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GRAY`` ``NATURAL``
   :Default: ``NATURAL``
   :Examples: ``--mdm-cpm-map GRAY``

Selects the ``CPM``'s symbols mapping layout.

Description of the allowed values:

+-------------+-----------------------------+
| Value       | Description                 |
+=============+=============================+
| ``GRAY``    | |mdm-cpm-map_descr_gray|    |
+-------------+-----------------------------+
| ``NATURAL`` | |mdm-cpm-map_descr_natural| |
+-------------+-----------------------------+

.. |mdm-cpm-map_descr_gray|    replace:: Gray code that switches only one bit at
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

Selects the ``CPM``'s wave shape.

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

.. |mdm-cpm-ws_descr_gmsk| replace:: `Gaussian Minimum Shift Keying`_
.. |mdm-cpm-ws_descr_rcos| replace:: Raised COSinus
.. |mdm-cpm-ws_descr_rec|  replace:: RECtangular


.. _mdm-mdm-cpm-std:

``--mdm-cpm-std``
"""""""""""""""""

   :Type: text
   :Allowed values: ``GSM``
   :Examples: ``--mdm-cpm-std GSM``

The selection of a default ``CPM`` standard implemented in hard in the code (any
of these parameters can be overwritten by the other given arguments).

Description of the allowed values:

+---------+------------------------------------+
| Value   | Description                        |
+=========+====================================+
| ``GSM`` | | Modulation index numerator = 1   |
|         | | Modulation index denominator = 2 |
|         | | Memory depth = 3                 |
|         | | Bit per symbol = 1               |
|         | | Sampling factor = 5              |
|         | | Mapping layout = ``NATURAL``     |
|         | | Wave shape = ``GMSK``            |
+---------+------------------------------------+

.. _mdm-mdm-ite:

``--mdm-ite``
"""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--mdm-ite 5``

Sets the number of iteration in the ``SCMA``'s demodulator.

.. _mdm-mdm-psi:

``--mdm-psi``
"""""""""""""

   :Type: text
   :Allowed values: ``PSI0`` ``PSI1`` ``PSI2`` ``PSI3``
   :Examples: ``--mdm-psi PSI0``

Selects the type of the psi function to use in the ``SCMA``'s demodulator.

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

.. |mdm-psi_descr_psi0| replace:: :math:`\exp\left(-\frac{|d|}{n0}\right)`
.. |mdm-psi_descr_psi1| replace:: :math:`\frac{1}{|d| + n0}`
.. |mdm-psi_descr_psi2| replace:: :math:`\frac{1}{8. |d|^2 + n0}`
.. |mdm-psi_descr_psi3| replace:: :math:`\frac{1}{4. |d|^2 + n0}`

Where |n0_equation|

.. |n0_equation| replace:: :math:`n0 =
  \begin{cases}
  1       & \quad \text{if sig2 disabled}\\
  4 \sigma^2  & \quad \text{else}
  \end{cases}`