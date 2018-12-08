.. _qnt-quantizer-parameters:

Quantizer parameters
--------------------

The quantizer is a module that ensures the real numbers transformation from a
floating-point representation to a fixed-point representation. This module is
enabled only if the receiver part of the communication chain works on a
fixed-point representation (cf. the :ref:`sim-sim-prec` parameter).

.. warning:: Some decoders are not *fixed-point ready*, please refer to the
   decoders documentation for more details.

.. _qnt-qnt-type:

``--qnt-type``
""""""""""""""

   :Type: text
   :Allowed values: ``CUSTOM`` ``POW2``
   :Default: ``POW2``
   :Examples: ``--qnt-type CUSTOM``

**STRING**: |factory::Quantizer::parameters::p+type|

Select the quantizer type.

Description of the allowed values (:math:`Y_i` stands for a floating-point
representation and :math:`Q_i` for the fixed-point representation of a real
number):

+------------+-------------------------+
| Value      | Description             |
+============+=========================+
| ``CUSTOM`` | |qnt-type_descr_custom| |
+------------+-------------------------+
| ``POW2``   | |qnt-type_descr_pow2|   |
+------------+-------------------------+

.. |qnt-type_descr_custom| replace:: :math:`Q_i = \begin{cases}
   +v_{sat} & \text{when } v_i > +v_{sat} \\
   -v_{sat} & \text{when } v_i < -v_{sat} \\
   v_i      & \text{else}
   \end{cases}`, with :math:`v_i = \lfloor \frac{Y_i}{\Delta} \rceil
   \text{ and } v_{sat} = 2^{p_b - 1} - 1
   \text{ and } \Delta = \frac{|p_r|}{v_{sat}}`

.. |qnt-type_descr_pow2| replace:: :math:`Q_i = \begin{cases}
   +v_{sat} & \text{when } v_i > +v_{sat} \\
   -v_{sat} & \text{when } v_i < -v_{sat} \\
   v_i      & \text{else}
   \end{cases}`, with :math:`v_i = \lfloor Y_i * F \rceil
   \text{ and } v_{sat} = 2^{p_b - 1} - 1
   \text{ and } F = 2^{p_d}`

Where :math:`p_r`, :math:`p_b` and :math:`p_d` are respectively given through
the :ref:`qnt-qnt-range`, :ref:`qnt-qnt-bits` and :ref:`qnt-qnt-dec` parameters.

.. _qnt-qnt-implem:

``--qnt-implem``
""""""""""""""""

   :Type: text
   :Allowed values: ``STD`` ``FAST``
   :Default: ``STD``
   :Examples: ``--qnt-implem FAST``

**STRING**: |factory::Quantizer::parameters::p+implem|

Select the implementation of the quantizer.

Description of the allowed values:

+----------+-------------------------+
| Value    | Description             |
+==========+=========================+
| ``STD``  | |qnt-implem_descr_std|  |
+----------+-------------------------+
| ``FAST`` | |qnt-implem_descr_fast| |
+----------+-------------------------+

.. |qnt-implem_descr_std|  replace:: Select a standard implementation.
.. |qnt-implem_descr_fast| replace:: Select a fast implementation, only
   available for the ``POW2`` quantizer.

.. _qnt-qnt-range:

``--qnt-range``
"""""""""""""""

   :Type: real number
   :Examples: ``--qnt-range 1.0``

**STRING**: |factory::Quantizer::parameters::p+range|

The min/max bounds for the ``CUSTOM`` quantizer.

.. _qnt-qnt-bits:

``--qnt-bits``
""""""""""""""

   :Type: integer
   :Default: 8 else see :numref:`bits_default_table`
   :Examples: ``--qnt-bits 1``

**STRING**: |factory::Quantizer::parameters::p+bits|

Set the number of bits used in the fixed-point representation.

.. note:: If the amplitude of the current number exceeds the maximum amplitude
   that can be represented with the current quantization, then a saturation is
   applied (c.f. the :ref:`qnt-qnt-type` parameter).

.. _bits_default_table:

.. table:: Default values of the total number of bits for the different codes.
   :align: center

   +--------------+-----------+
   | Code         | Value     |
   +==============+===========+
   | ``LDPC``     | 6         |
   +--------------+-----------+
   | ``POLAR``    | 6         |
   +--------------+-----------+
   | ``REP``      | 6         |
   +--------------+-----------+
   | ``RSC``      | 6         |
   +--------------+-----------+
   | ``RSC_DB``   | 6         |
   +--------------+-----------+
   | ``TPC``      | |bit_tpc| |
   +--------------+-----------+
   | ``TURBO``    | 6         |
   +--------------+-----------+
   | ``TURBO_DB`` | 6         |
   +--------------+-----------+

.. |bit_tpc| replace:: 6 on 8-bit and 8 on 16-bit

.. _qnt-qnt-dec:

``--qnt-dec``
"""""""""""""

   :Type: integer
   :Default: 3 else see :numref:`dec_default_table`
   :Examples: ``--qnt-dec 1``

**STRING**: |factory::Quantizer::parameters::p+dec|

Set the position of the decimal point in the quantified representation.

.. _dec_default_table:

.. table:: Default values of the decimal point position for the different codes.
   :align: center

   +--------------+---------------+
   | Code         | Value         |
   +==============+===============+
   | ``LDPC``     | 2             |
   +--------------+---------------+
   | ``POLAR``    | 1             |
   +--------------+---------------+
   | ``REP``      | 2             |
   +--------------+---------------+
   | ``RSC``      | |dec_rsc|     |
   +--------------+---------------+
   | ``RSC_DB``   | |dec_rscdb|   |
   +--------------+---------------+
   | ``TPC``      | |dec_tpc|     |
   +--------------+---------------+
   | ``TURBO``    | |dec_turbo|   |
   +--------------+---------------+
   | ``TURBO_DB`` | |dec_turbodb| |
   +--------------+---------------+

.. |dec_rsc|     replace:: 1 on 8-bit and 3 on 16-bit
.. |dec_rscdb|   replace:: 1 on 8-bit and 3 on 16-bit
.. |dec_tpc|     replace:: 2 on 8-bit and 3 on 16-bit
.. |dec_turbo|   replace:: 2 on 8-bit and 3 on 16-bit
.. |dec_turbodb| replace:: 2 on 8-bit and 3 on 16-bit