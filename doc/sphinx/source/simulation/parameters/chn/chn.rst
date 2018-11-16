.. _chn-channel-parameters:

Channel parameters
------------------

The channel is the representation of the physical space in which data are
transmitted. This operation corrupts the information, hence the origin of the
error-correcting codes.

.. TODO : Add_user on all channels ? For complex numbers, real / imag part independent ?

.. _chn-chn-type:

``--chn-type``
""""""""""""""

   :Type: text
   :Allowed values: ``AWGN`` ``BEC`` ``BSC`` ``NO`` ``OPTICAL`` ``RAYLEIGH``
                    ``RAYLEIGH_USER`` ``USER`` ``USER_ADD``
   :Default: ``AWGN``
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
| ``USER_BEC``      | |chn-type_descr_user_bec|      |
+-------------------+--------------------------------+
| ``USER_BSC``      | |chn-type_descr_user_bsc|      |
+-------------------+--------------------------------+

.. _Additive White Gaussian Noise: https://en.wikipedia.org/wiki/Additive_white_Gaussian_noise
.. _Binary Erasure Channel: https://en.wikipedia.org/wiki/Binary_erasure_channel
.. _Binary Symmetric Channel: https://en.wikipedia.org/wiki/Binary_symmetric_channel
.. _Rayleigh Fading: https://en.wikipedia.org/wiki/Rayleigh_fading


.. |chn-type_descr_awgn|          replace:: The `Additive White Gaussian Noise`_
   channel: :math:`Y = X + Z \text{ with } Z \sim \mathcal{N}(0,\sigma)`

.. |chn-type_descr_bec|           replace:: The `Binary Erasure Channel`_
   :math:`Y_i = \begin{cases}
   erased & \quad \text{if } e = 1 \\
   X_i    & \quad \text{else}
   \end{cases} \text{with } P(e = 1) = p_e`

.. |chn-type_descr_bsc|           replace:: The `Binary Symmetric Channel`_
   :math:`Y_i = \begin{cases}
   !X_i & \quad \text{if } e = 1 \\
   X_i    & \quad \text{else}
   \end{cases} \text{with } P(e = 1) = p_e`

.. |chn-type_descr_no|            replace:: The **perfect** channel:
   :math:`Y = X`

.. |chn-type_descr_optical|       replace:: The **optical** channel:
   :math:`Y_i = \begin{cases}
   CDF_0(x) & \quad \text{ when } X_i = 0 \\
   CDF_1(x) & \quad \text{ when } X_i = 1
   \end{cases} \text{ with } x \sim \mathcal{U}(0,1)`
   and the CDFs given by user.

.. |chn-type_descr_rayleigh|      replace:: The `Rayleigh Fading`_ channel with
   an AWGN gain: :math:`Y = X.H + Z \text{ with }
   Z \sim \mathcal{N}(0,\sigma) \text{ and }
   H \sim \mathcal{N}(0,\frac{1}{\sqrt 2})`

.. |chn-type_descr_rayleigh_user| replace:: The `Rayleigh Fading`_ channel with
   the gain given in a file: :math:`Y = X.H + Z
   \text{ with } Z \sim \mathcal{N}(0,\sigma) \text{ and }
   H \text{ given by the user}`

.. |chn-type_descr_user|          replace:: A **user defined output** channel
   from a file: :math:`Y = Z \text{ with } Z \text{ given by user}`

.. |chn-type_descr_user_add|      replace:: An **additive** channel with a user
   defined noise given in a file: :math:`Y = X +
   Z \text{ with } Z \text{ given by the user}`

.. |chn-type_descr_user_bec|      replace:: The `Binary Erasure Channel`_ with a
   user defined event draw given in a file:
   :math:`Y_i = \begin{cases}
   erased & \quad \text{if } e = 1 \\
   X_i    & \quad \text{else}
   \end{cases} \text{ with } e \text{ given by the user}`

.. |chn-type_descr_user_bsc|      replace:: The `Binary Symmetric Channel`_ with
   a user defined event draw given in a file:
   :math:`Y_i = \begin{cases}
   !X_i & \quad \text{if } e = 1 \\
   X_i    & \quad \text{else}
   \end{cases} \text{ with } e \text{ given by the user}`

Where :math:`\sigma, p_e` are the simulated noise points and are given by the
user through the :ref:`sim-sim-noise-range` argument.

The :abbr:`CDF (Cumulative Distribution Function)` for the ``OPTICAL``
channel is given with the :ref:`sim-sim-pdf-path` argument. This file
describes the :abbr:`PDF (Probability Density Function)` of the
:abbr:`ROP (Received Optical Power)` at different value for a bit transmitted
at 0 and another for a bit transmitted at 1. So this file contains twice as
many PDF as ROP to test.


.. note:: The ``AWGN``, ``NO`` and ``RAYLEIGH`` channels handle
   complex modulations.

.. note:: The ``BEC``, ``BSC`` and ``OPTICAL`` channels work only with ``OOK``
   modulation.


.. _chn-chn-implem:

``--chn-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``FAST`` ``STD`` ``GSL`` ``MKL``
   :Default: ``STD``
   :Examples: ``--chn-implem FAST``

Selects the implementation of the algorithm to generate the noise.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |chn-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |chn-implem_descr_fast| |
+----------+-------------------------+
| ``GSL``  | |chn-implem_descr_gsl|  |
+----------+-------------------------+
| ``MKL``  | |chn-implem_descr_mkl|  |
+----------+-------------------------+

.. _GNU Scientific Library: https://www.gnu.org/software/gsl/
.. _Intel Math Kernel Library: https://software.intel.com/en-us/mkl

.. |chn-implem_descr_std|  replace:: A standard implementation using the C++
   standard objects and algorithms.

.. |chn-implem_descr_fast| replace:: A much faster method using |SIMD|.

.. |chn-implem_descr_gsl|  replace:: A method using the
   `GNU Scientific Library`_ but not parallelized.

.. |chn-implem_descr_mkl|  replace:: A parallelized method using the
   `Intel Math Kernel Library`_ optimized on Intel processors.

.. attention:: All channels work with any implementation. However, to activate
   the ``GSL`` or the ``MKL`` implementation you need to
   compile with the good options (see :ref:`compilation_cmake_options`).


Numbers on :numref:`comp_implem_awgn`, :numref:`comp_implem_bec` and
:numref:`comp_implem_optical` show the throughput of the channels on
the different implementations in function of the frame size. The conditions of
the experiment were an ``UNCODED`` simulation on 8 threads during 10 seconds per
measurement on an *Intel(R) Xeon(R) CPU E3-1270 v5 @ 3.60GHz*.
To measure throughput AFF3CT uses an internal tool that can be activated with
the :ref:`sim-sim-stats` option flag. Numbers on those tables were read on the
*Average* column of the *add_noise* task of the *Channel* module.


.. _comp_implem_awgn:

.. csv-table:: Comparison of the throughput in :math:`[Mb/s]` of the methods
   for the ``AWGN`` channel in function of the frame size.
   :delim: ;
   :file: implem_comparison_AWGN.csv

The :numref:`comp_implem_awgn` shows that the standard normal distribution
generator with a Mersenne Twister 19937 is quite fast next to a Ziggurat
algorithm from the GSL. However, the ``FAST`` Box-Muller method implementation
with also a Mersenne Twister is much faster than the ``MKL``'s Box-Muller on
short frames but the last takes some distance on longer ones.


.. _comp_implem_bec:

.. csv-table:: Comparison of the throughput in :math:`[Mb/s]` of the methods
   for the ``BEC` and ``BSC`` channel in function of the frame size.
   :delim: ;
   :file: implem_comparison_BEC.csv

You can see on :numref:`comp_implem_bec` that the ``MKL``'s Bernoulli solution
is up to four times faster than the ``FAST`` implementation using a
Mersenne Twister. The ``GSL``'s Bernoulli implementation is here
twice faster than a ``STD``'s Bernoulli algorithm solution associated with a
Mersenne Twister.


.. _comp_implem_optical:

.. csv-table:: Comparison of the throughput in :math:`[Mb/s]` of the methods
   for the ``OPTICAL`` channel in function of the frame size.
   :delim: ;
   :file: implem_comparison_OPTICAL.csv

You can see on :numref:`comp_implem_optical` that although using same
implementation as for the ``BEC`` to generate an uniform draw, the
throughputs are identical for all. This is due to an unoptimized interpolation
function using standard algorithm applied on each symbol to compute the output
according to the given CDFs.

.. _chn-chn-gain-occur:

``--chn-gain-occur``
""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--chn-gain-occur 10``

The number of times a gain is used on consecutive symbols. It is used in the
``RAYLEIGH_USER`` channel while applying gains read from the given file.

.. _chn-chn-path:

``--chn-path``
""""""""""""""

   :Type: file
   :Rights: read
   :Examples: ``--chn-path example/path/to/the/right/file``

For ``USER`` type, the file must contain noisy output values.
For ``USER_ADD`` type, the file must contain noise values to add to input.
For ``RAYLEIGH_USER`` type, the file must contain gain values.
See :ref:`chn-chn-type` for more details.

.. TODO Block fading is unused !!!
   .. _chn-chn-blk-fad:

   ``--chn-blk-fad``
   """""""""""""""""

      :Type: text
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
