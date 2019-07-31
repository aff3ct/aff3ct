.. _user_introduction:

************
Introduction
************

.. _Forward Error Correction: https://en.wikipedia.org/wiki/Forward_error_correction
.. _Monte Carlo method: https://en.wikipedia.org/wiki/Monte_Carlo_method

|AFF3CT| is a toolbox dedicated to the `Forward Error Correction`_ (FEC or
**channel coding**). It is written in **C++** and it supports a large range of
codes: from the well-spread **Turbo codes** to the new **Polar codes** including
the **Low-Density Parity-Check (LDPC) codes**. |AFF3CT| includes many tools but
the most important ones are:

.. hlist::
   :columns: 2

   * a **standalone simulator** to simulate communication chains (c.f.
     :numref:`fig_comm_chain`) based on a `Monte Carlo method`_,
   * a **toolbox** or a **library** that can be used through a well-documented
     |API|.

.. _fig_comm_chain:

.. figure:: images/comm_chain.svg
   :figwidth: 80 %
   :align: center

   The communication chain.

**The simulator targets high speed simulations** and extensively uses parallel
techniques like |SIMD|, multi-threading and multi-nodes programming models.
Below, a list of the features that motivated the creation of the simulator:

   #. **reproduce state-of-the-art decoding performances**,
   #. **explore various channel code configurations**, find new trade-offs,
   #. **prototype hardware implementation** (fixed-point receivers, hardware in
      the loop tools),
   #. **reuse tried and tested modules** and add yours,
   #. **alternative to MATLAB**, if you seek to reduce simulations time.

|AFF3CT| was first intended to be a simulator but as it developed, the need to
reuse sub-parts of the code intensified: **the library was born**. Below is a
list of possible applications for the library:

   #. **build custom communication chains** that are not possible with the
      simulator,
   #. **facilitate hardware prototyping**,
   #. enable various modules to be used in |SDR| contexts.