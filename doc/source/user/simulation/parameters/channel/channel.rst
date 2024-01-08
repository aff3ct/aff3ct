.. _chn-channel-parameters:

Channel parameters
------------------

The channel represents the physical support such as optical fiber, space, water,
air, etc. It is during the passage in the channel that the frames are
altered/noised and errors can occur. The channel coding theory has been invented
to correct errors induced by the channel (or at least reduce the number of
errors to an acceptable rate).

.. TODO : Add_user on all channels ? For complex numbers, real / imag part independent ?

.. _chn-chn-type:

``--chn-type``
""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``BEC`` ``BSC`` ``AWGN`` ``RAYLEIGH``
                    ``RAYLEIGH_USER`` ``OPTICAL`` ``USER`` ``USER_ADD``
                    ``USER_BEC`` ``USER_BSC``
   :Default: ``AWGN``
   :Examples: ``--chn-type AWGN``

|factory::Channel::p+type|

Description of the allowed values:

+-------------------+--------------------------------+
| Value             | Description                    |
+===================+================================+
| ``NO``            | |chn-type_descr_no|            |
+-------------------+--------------------------------+
| ``BEC``           | |chn-type_descr_bec|           |
+-------------------+--------------------------------+
| ``BSC``           | |chn-type_descr_bsc|           |
+-------------------+--------------------------------+
| ``AWGN``          | |chn-type_descr_awgn|          |
+-------------------+--------------------------------+
| ``RAYLEIGH``      | |chn-type_descr_rayleigh|      |
+-------------------+--------------------------------+
| ``RAYLEIGH_USER`` | |chn-type_descr_rayleigh_user| |
+-------------------+--------------------------------+
| ``OPTICAL``       | |chn-type_descr_optical|       |
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
.. _Rayleigh fading: https://en.wikipedia.org/wiki/Rayleigh_fading

.. |chn-type_descr_no| replace:: Disable the channel noise: :math:`Y = X`.

.. |chn-type_descr_bec| replace:: Select the `Binary Erasure Channel`_ (|BEC|):
   :math:`Y_i = \begin{cases}
   erased & \text{if } e = 1 \\
   X_i    & \text{else}
   \end{cases}, \text{with } P(e = 1) = p_e \text{ and } P(e = 0) = 1 - p_e`.

.. |chn-type_descr_bsc| replace:: Select the `Binary Symmetric Channel`_
   (|BSC|):
   :math:`Y_i = \begin{cases}
   !X_i & \text{if } e = 1 \\
   X_i  & \text{else}
   \end{cases}, \text{with } P(e = 1) = p_e \text{ and } P(e = 0) = 1 - p_e`.

.. |chn-type_descr_awgn| replace:: Select the `Additive White Gaussian Noise`_
   (|AWGN|) channel:
   :math:`Y = X + Z \text{ with } Z \sim \mathcal{N}(0,\sigma^2)`.

.. |chn-type_descr_rayleigh| replace:: Select the `Rayleigh fading`_ channel
   with an |AWGN| gain: :math:`Y = X.H + Z \text{ with }
   Z \sim \mathcal{N}(0,\sigma) \text{ and }
   H \sim \mathcal{N}(0,\frac{1}{\sqrt 2})`.

.. |chn-type_descr_rayleigh_user| replace:: Select the `Rayleigh fading`_
   channel with the gain given in a file: :math:`Y = X.H + Z
   \text{ with } Z \sim \mathcal{N}(0,\sigma) \text{ and }
   H \text{ given by the user}` (to use with the :ref:`chn-chn-path` parameter).

.. |chn-type_descr_optical| replace:: Select the optical channel:
   :math:`Y_i = \begin{cases}
   CDF_0(x) & \text{ when } X_i = 0 \\
   CDF_1(x) & \text{ when } X_i = 1
   \end{cases}, \text{ with } x \sim \mathcal{U}(0,1)`,
   and the :math:`CDF_0` and :math:`CDF_1` are given by the user and selected
   in function of the current |ROP| (to use with the :ref:`sim-sim-pdf-path`
   parameter).

.. |chn-type_descr_user| replace:: Select the noised frame from a file:
   :math:`Y = Z \text{ with } Z \text{ given by the user}` (to use with the
   :ref:`chn-chn-path` parameter).

.. |chn-type_descr_user_add| replace:: Select the noise to add to the frame
   from a given file: :math:`Y = X +
   Z \text{ with } Z \text{ given by the user}` (to use with the
   :ref:`chn-chn-path` parameter).

.. |chn-type_descr_user_bec| replace:: Select the event draw of the |BEC| from a
   file:
   :math:`Y_i = \begin{cases}
   erased & \text{if } e = 1 \\
   X_i    & \text{else}
   \end{cases}, \text{ with } e \text{ given by the user}` (to use with the
   :ref:`chn-chn-path` parameter).

.. |chn-type_descr_user_bsc| replace:: Select the event draw of the |BSC| from a
   file:
   :math:`Y_i = \begin{cases}
   !X_i & \text{if } e = 1 \\
   X_i  & \text{else}
   \end{cases}, \text{ with } e \text{ given by the user}` (to use with the
   :ref:`chn-chn-path` parameter).

Where:
   * :math:`\sigma` is the *Gaussian noise variance*, :math:`p_e` is the
     *event probability* and |ROP| is the *Received optical power* of the
     simulated noise points.
     They are given by the user through the :ref:`sim-sim-noise-range` argument.
   * :math:`X` is the original modulated frame and :math:`Y` the noisy output.
   * :math:`\mathcal{N}(\mu,\sigma^2)` is the
     `Normal or Gaussian distribution <https://en.wikipedia.org/wiki/Normal_distribution>`_.
   * :math:`\mathcal{U}(a,b)` is the
     `Uniform distribution <https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)>`_.

For the ``OPTICAL`` channel, the |CDF| are computed from the given |PDF| with
the :ref:`sim-sim-pdf-path` argument. This file describes the latter for the
different |ROP|. There must be a |PDF| for a bit transmitted at 0 and another
for a bit transmitted at 1.

.. note:: The ``NO``, ``AWGN`` and ``RAYLEIGH`` channels handle complex
   modulations.

.. warning:: The ``BEC``, ``BSC`` and ``OPTICAL`` channels work only with the
   ``OOK`` modulation (see the :ref:`mdm-mdm-type` parameter).

.. _chn-chn-implem:

``--chn-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST`` ``GSL`` ``MKL``
   :Default: ``STD``
   :Examples: ``--chn-implem FAST``

|factory::Channel::p+implem|

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

.. |chn-implem_descr_std| replace:: Select the standard implementation based on
   the C++ standard library.

.. |chn-implem_descr_fast| replace:: Select the fast implementation (handwritten
   and optimized for |SIMD| architectures).

.. |chn-implem_descr_gsl| replace:: Select an implementation based of the |GSL|.

.. |chn-implem_descr_mkl| replace:: Select an implementation based of the |MKL|
   (only available for x86 architectures).

.. note:: All the proposed implementations are based on the |MT 19937| |PRNG|
   algorithm :cite:`Matsumoto1998`. The Gaussian distribution
   :math:`\mathcal{N}(\mu,\sigma^2)` is implemented with the Box-Muller method
   :cite:`Box1958` except when using the |GSL| where the Ziggurat method
   :cite:`Marsaglia2000` is used instead.

.. attention:: To enable the |GSL| or the |MKL| implementations, you need to
   have those libraries installed on your system and to turn on specific
   :ref:`compilation_cmake_options`.

The :numref:`tab_chn_implem_awgn`, :numref:`tab_chn_implem_bec` and
:numref:`tab_chn_implem_optical` present the throughputs of the different
channel implementations depending on the frame size. The testbed for the
experiments is an *Intel(R) Xeon(R) CPU E3-1270 v5 @ 3.60GHz* 8 threads |CPU|.

.. _tab_chn_implem_awgn:

.. csv-table:: Comparison of the |AWGN| channel implementations (throughputs are
   in Mb/s).
   :delim: ;
   :file: implem_comparison_AWGN.csv

.. The :numref:`tab_chn_implem_awgn` shows that the standard normal distribution
.. generator with a Mersenne Twister 19937 is quite fast next to a Ziggurat
.. algorithm from the GSL. However, the ``FAST`` Box-Muller method implementation
.. with also a Mersenne Twister is much faster than the ``MKL``'s Box-Muller on
.. short frames but the last takes some distance on longer ones.

.. _tab_chn_implem_bec:

.. csv-table:: Comparison of the |BEC|/|BSC| channel implementations
   (throughputs are in Mb/s).
   :delim: ;
   :file: implem_comparison_BEC.csv

.. You can see on :numref:`tab_chn_implem_bec` that the ``MKL``'s Bernoulli solution
.. is up to four times faster than the ``FAST`` implementation using a
.. Mersenne Twister. The ``GSL``'s Bernoulli implementation is here
.. twice faster than a ``STD``'s Bernoulli algorithm solution associated with a
.. Mersenne Twister.

.. _tab_chn_implem_optical:

.. csv-table:: Comparison of the optical channel implementations (throughputs
   are in Mb/s).
   :delim: ;
   :file: implem_comparison_OPTICAL.csv

.. You can see on :numref:`tab_chn_implem_optical` that although using same
.. implementation as for the ``BEC`` to generate an uniform draw, the
.. throughputs are identical for all. This is due to an unoptimized interpolation
.. function using standard algorithm applied on each symbol to compute the output
.. according to the given CDFs.

.. note:: The reported values are the *average* throughputs given by the
   simulator integrated statistics tool (see the :ref:`sim-sim-stats`
   parameter).

.. _chn-chn-blk-fad:

``--chn-blk-fad``
"""""""""""""""""

   :Type: text
   :Allowed values: ``NO`` ``FRAME`` ``ONETAP``
   :Default: 1
   :Examples: ``--chn-gain-occur 10``

|factory::Channel::p+blk-fad|

.. note:: At this time the ``FRAME`` and ``ONETAP`` block fading are not
   implemented.

.. _chn-chn-gain-occur:

``--chn-gain-occur``
""""""""""""""""""""

   :Type: integer
   :Default: 1
   :Examples: ``--chn-gain-occur 10``

|factory::Channel::p+gain-occur|

.. _chn-chn-path:

``--chn-path``
""""""""""""""

   :Type: file
   :Rights: read
   :Examples: ``--chn-path example/path/to/the/right/file``

|factory::Channel::p+path|

The expected type of noise vary depending of the channel type (see the
:ref:`chn-chn-type` parameter for more details):

   * ``USER``: the file must contain frame with the noise applied on it,
   * ``USER_ADD``: the file must contain only the noise :math:`Z`,
   * ``RAYLEIGH_USER``: the file must contain the gain values :math:`H`.

The expected file format is either |ASCII| or binary (the format is
automatically detected). Here is the file structure expected in |ASCII|:

.. code-block:: bash

    # 'F' has to be replaced by the number of contained frames.
   F

   # 'N' has to be replaced by the frame size.
   N

   # a sequence of 'F * N' floating-point values (separated by spaces)
   Y_0 Y_1 Y_2 Y_3 Y_4 [...] Y_{F*N-1}

In binary mode, :math:`F` and :math:`N` have to be 32-bit unsigned integers.
The next :math:`F \times N` floating-point values can be either in 32-bit or in
64-bit.

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

References
""""""""""

.. bibliography:: references.bib
   :labelprefix: Chn-
