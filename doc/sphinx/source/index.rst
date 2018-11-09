####################
AFF3CT Documentation
####################

.. _Forward Error Correction: https://en.wikipedia.org/wiki/Forward_error_correction
.. _Monte Carlo: https://en.wikipedia.org/wiki/Monte_Carlo_method
.. _BFER: https://en.wikipedia.org/wiki/Bit_error_rate
.. _EXIT chart: https://en.wikipedia.org/wiki/EXIT_chart

**AFF3CT** (A Fast Forward Error Correction Toolbox!) is a library dedicated to
the `Forward Error Correction`_ (FEC or channel coding). It is written in
**C++11** and it supports a large range of codes: from the well-spread
**Turbo codes** to the new **Polar codes** including the
**Low-Density Parity-Check (LDPC) codes**. AFF3CT can be used in two different
ways, as:

.. hlist::
   :columns: 1

   * a **standalone application** for `Monte Carlo`_ `BFER`_ and `EXIT chart`_
     simulations (see the :ref:`simulation` section),
   * a dedicated **toolbox** or **library** for your projects, many modules
     (like codecs, modems, channels, ...) are available and easy to use (see
     the :ref:`library` section).

.. toctree::
   :caption: Contents
   :maxdepth: 2
   :numbered:

   source_code/source_code
   compilation/compilation
   installation/installation
   simulation/simulation
   library/library
   developer/developer
   license/license

.. Indices and tables
.. ==================

.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`
