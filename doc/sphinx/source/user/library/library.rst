.. _user_library:

****************
Library Examples
****************

All the following examples simulate a simple communication chain with a |BPSK|
modem and a repetition code over an |AWGN| channel. The |BFER| results are
calculated from 0.0 dB to 10.0 dB with a 1.0 dB step.

.. _fig_simple_chain:

.. figure:: images/simple_chain.svg
   :align: center

   Simulated communication chain.

Bootstrap
=========

The `bootstrap` example is the most simple way to use |AFF3CT|, it is based on
``C++`` classes and methods that operate on buffers: it is a good point to
start. Keep in mind that this is the most simple way to use |AFF3CT| but not the
most powerful. More advanced features like benchmarking, debugging, command line
interface and more are available in the ``tasks`` and ``factory`` examples.
The :numref:`fig_simple_chain_code` illustrates the source code: the
green boxes are the classes, the blue boxes are the methods and the arrows are
the buffers.

.. _fig_simple_chain_code:

.. figure:: images/simple_chain_code.svg
   :align: center

   Simulated communication chain: classes and methods.

.. code-block:: cpp
	:caption: Bootstrap: main function
	:name: lst_bootstrap_main
	:linenos:

	#include <aff3ct.hpp>
	using namespace aff3ct;

	int main(int argc, char** argv)
	{
		auto p = init_params ( ); // get parameters defined by the user
		auto m = init_modules(p); // create the AFF3CT modules
		auto b = init_buffers(p); // create the buffers required by the AFF3CT modules
		auto u = init_utils  (m); // create the AFF3CT utils

		// display the legend in the terminal
		u.terminal->legend();

		// a loop over the various SNRs
		for (auto ebn0 = p.ebn0_min; ebn0 < p.ebn0_max; ebn0 += p.ebn0_step)
		{
			// compute the current sigma for the channel noise
			const auto esn0  = tools::ebn0_to_esn0 (ebn0, p.R);
			const auto sigma = tools::esn0_to_sigma(esn0     );

			u.noise->set_noise(sigma, ebn0, esn0);

			// update the sigma of the modem and the channel
			m.modem  ->set_noise(*u.noise);
			m.channel->set_noise(*u.noise);

			// display the performance (BER and FER) in real time (in a separate thread)
			u.terminal->start_temp_report();

			// run the simulation chain
			while (!m.monitor->fe_limit_achieved() && !u.terminal->is_interrupt())
			{
				m.source ->generate    (                 b.ref_bits     );
				m.encoder->encode      (b.ref_bits,      b.enc_bits     );
				m.modem  ->modulate    (b.enc_bits,      b.symbols      );
				m.channel->add_noise   (b.symbols,       b.noisy_symbols);
				m.modem  ->demodulate  (b.noisy_symbols, b.LLRs         );
				m.decoder->decode_siho (b.LLRs,          b.dec_bits     );
				m.monitor->check_errors(b.dec_bits,      b.ref_bits     );
			}

			// display the performance (BER and FER) in the terminal
			u.terminal->final_report();

			// reset the monitor for the next SNR
			m.monitor->reset();
			u.terminal->reset();
		}

		return 0;
	}

The :numref:`lst_bootstrap_main` gives an overview of what can be achieved with
the |AFF3CT| library. The firsts lines ``6-9`` are dedicated to the objects
instantiations and buffers allocation and will be detailed after. Just note that
``p`` contains the simulation parameters, ``b`` contains the buffers required by
the modules, ``m`` contains the modules of the communication chain and ``u`` is
a set of tools.

Line ``15`` is the loop over the various |SNRs|. Lines ``31-40`` is a ``while``
loop that stops when 100 frame errors are detected by the monitor. The |AFF3CT|
communication chain methods are called inside this loop. Each |AFF3CT| method
works on input(s) and/or output(s) buffer(s) that have been declared line ``8``.
Those buffers can be ``std::vector`` or pointer. The sizes and the types of
those buffers have to be set accordingly to the sizes and types of the |AFF3CT|
modules declared line ``7``. If there is a mismatch with the sizes or the types
then |AFF3CT| will throw an exception and stop. The |AFF3CT| modules are classes
that uses the C++ meta-programing technique (``C++ templates``). By default
those templates are ``int32_t`` or ``float``.

.. code-block:: cpp
	:caption: Bootstrap: parameters
	:name: lst_bootstrap_params
	:linenos:

	struct params
	{
		int   K;         // the number of information bits
		int   N;         // the codeword size
		int   fe;        // the number of frame errors
		int   seed;      // the PRNG seed for AWGN channel
		float R;         // the code rate (R=K/N)
		float ebn0_min;  // the minimum SNR value
		float ebn0_max;  // the maximum SNR value
		float ebn0_step; // the SNR step
	};

	params init_params()
	{
		params p;

		p.K         =  32;
		p.N         = 128;
		p.fe        = 100;
		p.seed      =   0;
		p.ebn0_min  =   0.00f;
		p.ebn0_max  =  10.01f;
		p.ebn0_step =   1.00f;

		p.R = (float)p.K / (float)p.N;

		return p;
	}

The :numref:`lst_bootstrap_params` describes the simulation parameters.

.. code-block:: cpp
	:caption: Bootstrap: modules
	:name: lst_bootstrap_modules
	:linenos:

	struct modules
	{
		std::unique_ptr<module::Source_random<>>          source;
		std::unique_ptr<module::Encoder_repetition_sys<>> encoder;
		std::unique_ptr<module::Modem_BPSK<>>             modem;
		std::unique_ptr<module::Channel_AWGN_LLR<>>       channel;
		std::unique_ptr<module::Decoder_repetition_std<>> decoder;
		std::unique_ptr<module::Monitor_BFER<>>           monitor;
	};

	modules init_modules(const params &p)
	{
		modules m;

		m.source  = std::unique_ptr<module::Source_random         <>>(new module::Source_random         <>(p.K        ));
		m.encoder = std::unique_ptr<module::Encoder_repetition_sys<>>(new module::Encoder_repetition_sys<>(p.K, p.N   ));
		m.modem   = std::unique_ptr<module::Modem_BPSK            <>>(new module::Modem_BPSK            <>(p.N        ));
		m.channel = std::unique_ptr<module::Channel_AWGN_LLR      <>>(new module::Channel_AWGN_LLR      <>(p.N, p.seed));
		m.decoder = std::unique_ptr<module::Decoder_repetition_std<>>(new module::Decoder_repetition_std<>(p.K, p.N   ));
		m.monitor = std::unique_ptr<module::Monitor_BFER          <>>(new module::Monitor_BFER          <>(p.K, p.fe  ));

		return m;
	};

The :numref:`lst_bootstrap_modules` allocates the modules of the communication
chain. Those modules are allocated on the heap and manage by smart pointers
(``std::unique_ptr``). Not that the ``init_modules`` function takes a ``params``
structure in parameters. These parameters are used to build the modules.

.. code-block:: cpp
	:caption: Bootstrap: buffers
	:name: lst_bootstrap_buffers
	:linenos:

	struct buffers
	{
		std::vector<int  > ref_bits;
		std::vector<int  > enc_bits;
		std::vector<float> symbols;
		std::vector<float> noisy_symbols;
		std::vector<float> LLRs;
		std::vector<int  > dec_bits;
	};

	buffers init_buffers(const params &p)
	{
		buffers b;

		b.ref_bits      = std::vector<int  >(p.K);
		b.enc_bits      = std::vector<int  >(p.N);
		b.symbols       = std::vector<float>(p.N);
		b.noisy_symbols = std::vector<float>(p.N);
		b.LLRs          = std::vector<float>(p.N);
		b.dec_bits      = std::vector<int  >(p.K);

		return b;
	}

The :numref:`lst_bootstrap_buffers` allocates the buffers of the communication
chain. Here we chose to allocate the buffers with the ``std::vector`` C++
standard class. Like for the modules in :numref:`lst_bootstrap_modules`, the
size of the buffers comes from the input parameters of the ``init_buffers``
function.

.. code-block:: cpp
	:caption: Bootstrap: utils
	:name: lst_bootstrap_utils
	:linenos:

	struct utils
	{
		            std::unique_ptr<tools::Sigma<>     > noise;     // a sigma noise type
		std::vector<std::unique_ptr<tools::Reporter>   > reporters; // list of reporters dispayed in the terminal
		            std::unique_ptr<tools::Terminal_std> terminal;  // manage the output text in the terminal
	};

	utils init_utils(const modules &m)
	{
		utils u;

		// create a sigma noise type
		u.noise = std::unique_ptr<tools::Sigma<>>(new tools::Sigma<>());
		// report the noise values (Es/N0 and Eb/N0)
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_noise<>(*u.noise)));
		// report the bit/frame error rates
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_BFER<>(*m.monitor)));
		// report the simulation throughputs
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_throughput<>(*m.monitor)));
		// create a terminal that will display the collected data from the reporters
		u.terminal = std::unique_ptr<tools::Terminal_std>(new tools::Terminal_std(u.reporters));

		return u;
	}

|AFF3CT| comes with useful tools, the ``init_utils`` function allocate some of
them. The ``noise`` object contains the type of noise we want to simulate: here
it is a `sigma` kind. In order to be able to easily print the |BFER| in the
console, |AFF3CT| proposes a terminal object, this object is configurable with
reporters. In the :numref:`lst_bootstrap_utils`, three reporters are created,
one for display the |SNR|, an other to display the |BFER| and the last one to
show the simulation throughput.

If you run the `bootstrap` example the expected output is shown by
:numref:`lst_bootstrap_output`.

.. code-block:: bash
	:caption: Bootstrap: output
	:name: lst_bootstrap_output

	# ---------------------||------------------------------------------------------||---------------------
	#  Signal Noise Ratio  ||   Bit Error Rate (BER) and Frame Error Rate (FER)    ||  Global throughput
	#         (SNR)        ||                                                      ||  and elapsed time
	# ---------------------||------------------------------------------------------||---------------------
	# ----------|----------||----------|----------|----------|----------|----------||----------|----------
	#     Es/N0 |    Eb/N0 ||      FRA |       BE |       FE |      BER |      FER ||  SIM_THR |    ET/RT
	#      (dB) |     (dB) ||          |          |          |          |          ||   (Mb/s) | (hhmmss)
	# ----------|----------||----------|----------|----------|----------|----------||----------|----------
	      -6.02 |     0.00 ||      108 |      262 |      100 | 7.58e-02 | 9.26e-01 ||    2.382 | 00h00'00
	      -5.02 |     1.00 ||      125 |      214 |      100 | 5.35e-02 | 8.00e-01 ||    4.813 | 00h00'00
	      -4.02 |     2.00 ||      136 |      179 |      100 | 4.11e-02 | 7.35e-01 ||    3.804 | 00h00'00
	      -3.02 |     3.00 ||      210 |      135 |      100 | 2.01e-02 | 4.76e-01 ||    4.516 | 00h00'00
	      -2.02 |     4.00 ||      327 |      122 |      100 | 1.17e-02 | 3.06e-01 ||    5.157 | 00h00'00
	      -1.02 |     5.00 ||      555 |      112 |      100 | 6.31e-03 | 1.80e-01 ||    4.703 | 00h00'00
	      -0.02 |     6.00 ||     1619 |      108 |      100 | 2.08e-03 | 6.18e-02 ||    4.110 | 00h00'00
	       0.98 |     7.00 ||     4566 |      102 |      100 | 6.98e-04 | 2.19e-02 ||    4.974 | 00h00'00
	       1.98 |     8.00 ||    15998 |      100 |      100 | 1.95e-04 | 6.25e-03 ||    4.980 | 00h00'00
	       2.98 |     9.00 ||    93840 |      100 |      100 | 3.33e-05 | 1.07e-03 ||    5.418 | 00h00'00
	       3.98 |    10.00 ||   866433 |      100 |      100 | 3.61e-06 | 1.15e-04 ||    4.931 | 00h00'05

Tasks
=====

Some of the |AFF3CT| classes inherit from the ``Module`` abstract class.
Generally speaking all the classes that propose methods for a communication
chain is a ``Module``. Inside a module class they can be various methods but
only some of them are used in the communication chain directly. If a method can
be used in the chain then it is a ``Task``. A ``Task`` is characterized by its
behavior and its data: the input and output data are declared via a collection
of ``Socket``.

.. code-block:: cpp
	:linenos:
	:caption: Tasks: main function
	:name: lst_tasks_main
	:emphasize-lines: 13-21,42-48,59-60

	#include <aff3ct.hpp>
	using namespace aff3ct;

	int main(int argc, char** argv)
	{
		auto p = init_params ( ); // get parameters defined by the user
		auto m = init_modules(p); // create the AFF3CT modules
		auto u = init_utils  (m); // create the AFF3CT utils

		// display the legend in the terminal
		u.terminal->legend();

		// sockets binding (connect the sockets of the tasks = fill the input sockets with the output sockets)
		using namespace module;
		(*m.encoder)[enc::sck::encode      ::U_K ].bind((*m.source )[src::sck::generate   ::U_K ]);
		(*m.modem  )[mdm::sck::modulate    ::X_N1].bind((*m.encoder)[enc::sck::encode     ::X_N ]);
		(*m.channel)[chn::sck::add_noise   ::X_N ].bind((*m.modem  )[mdm::sck::modulate   ::X_N2]);
		(*m.modem  )[mdm::sck::demodulate  ::Y_N1].bind((*m.channel)[chn::sck::add_noise  ::Y_N ]);
		(*m.decoder)[dec::sck::decode_siho ::Y_N ].bind((*m.modem  )[mdm::sck::demodulate ::Y_N2]);
		(*m.monitor)[mnt::sck::check_errors::U   ].bind((*m.encoder)[enc::sck::encode     ::U_K ]);
		(*m.monitor)[mnt::sck::check_errors::V   ].bind((*m.decoder)[dec::sck::decode_siho::V_K ]);

		// a loop over the various SNRs
		for (auto ebn0 = p.ebn0_min; ebn0 < p.ebn0_max; ebn0 += p.ebn0_step)
		{
			// compute the current sigma for the channel noise
			const auto esn0  = tools::ebn0_to_esn0 (ebn0, p.R);
			const auto sigma = tools::esn0_to_sigma(esn0     );

			u.noise->set_noise(sigma, ebn0, esn0);

			// update the sigma of the modem and the channel
			m.modem  ->set_noise(*u.noise);
			m.channel->set_noise(*u.noise);

			// display the performance (BER and FER) in real time (in a separate thread)
			u.terminal->start_temp_report();

			// run the simulation chain
			while (!m.monitor->fe_limit_achieved() && !u.terminal->is_interrupt())
			{
				(*m.source )[src::tsk::generate    ].exec();
				(*m.encoder)[enc::tsk::encode      ].exec();
				(*m.modem  )[mdm::tsk::modulate    ].exec();
				(*m.channel)[chn::tsk::add_noise   ].exec();
				(*m.modem  )[mdm::tsk::demodulate  ].exec();
				(*m.decoder)[dec::tsk::decode_siho ].exec();
				(*m.monitor)[mnt::tsk::check_errors].exec();
			}

			// display the performance (BER and FER) in the terminal
			u.terminal->final_report();

			// reset the monitor and the terminal for the next SNR
			m.monitor->reset();
			u.terminal->reset();
		}

		// display the statistics of the tasks (if enabled)
		tools::Stats::show(m.list, true);

		return 0;
	}

.. code-block:: cpp
	:linenos:
	:caption: Tasks: modules
	:name: lst_tasks_modules
	:emphasize-lines: 10,24-39

	struct modules
	{
		std::unique_ptr<module::Source_random<>>          source;
		std::unique_ptr<module::Encoder_repetition_sys<>> encoder;
		std::unique_ptr<module::Modem_BPSK<>>             modem;
		std::unique_ptr<module::Channel_AWGN_LLR<>>       channel;
		std::unique_ptr<module::Decoder_repetition_std<>> decoder;
		std::unique_ptr<module::Monitor_BFER<>>           monitor;

		std::vector<const module::Module*> list;
	};

	modules init_modules(const params &p)
	{
		modules m;

		m.source  = std::unique_ptr<module::Source_random         <>>(new module::Source_random         <>(p.K        ));
		m.encoder = std::unique_ptr<module::Encoder_repetition_sys<>>(new module::Encoder_repetition_sys<>(p.K, p.N   ));
		m.modem   = std::unique_ptr<module::Modem_BPSK            <>>(new module::Modem_BPSK            <>(p.N        ));
		m.channel = std::unique_ptr<module::Channel_AWGN_LLR      <>>(new module::Channel_AWGN_LLR      <>(p.N, p.seed));
		m.decoder = std::unique_ptr<module::Decoder_repetition_std<>>(new module::Decoder_repetition_std<>(p.K, p.N   ));
		m.monitor = std::unique_ptr<module::Monitor_BFER          <>>(new module::Monitor_BFER          <>(p.K, p.fe  ));

		m.list = {m.source.get(), m.encoder.get(), m.modem.get(), m.channel.get(), m.decoder.get(), m.monitor.get()};

		// configuration of the module tasks
		for (auto& mod : m.list)
			for (auto& tsk : mod->tasks)
			{
				tsk->set_autoalloc  (true ); // enable the automatic allocation of the data in the tasks
				tsk->set_autoexec   (false); // disable the auto execution mode of the tasks
				tsk->set_debug      (false); // disable the debug mode
				tsk->set_debug_limit(16   ); // display only the 16 first bits if the debug mode is enabled
				tsk->set_stats      (true ); // enable the statistics

				// enable the fast mode (= disable the useless verifs in the tasks) if there is no debug and stats modes
				if (!tsk->is_debug() && !tsk->is_stats())
					tsk->set_fast(true);
			}

		return m;
	};

Lines ``68-81``, each ``Module`` is parsed to get its tasks, each ``Task`` is
configured to automatically allocate its output memory (line ``72``) and make
statistic on the ``Task`` execution (line ``76``).
It is also possible to print debug information by putting ``true`` value instead
of ``false`` at line ``74``.

Lines ``84-91``, the sockets of the tasks are binded together to set the data
dependencies of the communication chain. For instance, line ``85`` the output
socket ``U_K`` of the ``generate`` task (``Source`` module) is binded to the
input socket ``U_K`` of the ``encode`` task (``Encoder`` module).
Once the binding is made the tasks can be executed lines ``110-119`` with the
``exec`` method.

Factory
=======

In the previous ``tasks`` example the AFF3CT ``Module`` classes were built
manually in the source code. In the ``Factory`` example, the ``factory`` classes
are used to build the ``Module`` from the command line arguments. The
``factory`` classes are declared line ``14-20`` and the command line arguments
are parsed line ``23`` with the ``Command_parser`` class.

Then, lines ``52-60``, each ``factory`` class uses its ``build`` method to
instantiate the corresponding ``Module``.

In this example, the proposed source code use a repetition code, however it is
very easy to switch the code type by replacing ``repetition`` line ``16`` and
line ``54`` by ``polar`` to work with polar code (this is also true for other
code types).

.. code-block:: cpp
	:caption: Factory: main function
	:name: lst_factory_main
	:emphasize-lines: 6
	:linenos:

	#include <aff3ct.hpp>
	using namespace aff3ct;

	int main(int argc, char** argv)
	{
		auto p = init_params (argc, argv); // initialize the params from the command line with factories
		auto m = init_modules(p         ); // create the AFF3CT modules
		auto u = init_utils  (p,    m   ); // create the AFF3CT utils

		// [...]

		return 0;
	}

.. code-block:: cpp
	:caption: Factory: parameters
	:name: lst_factory_params
	:emphasize-lines: 3-8,20-25,31-49
	:linenos:

	struct params
	{
		std::unique_ptr<factory::Source          ::parameters> source;
		std::unique_ptr<factory::Codec_repetition::parameters> codec;
		std::unique_ptr<factory::Modem           ::parameters> modem;
		std::unique_ptr<factory::Channel         ::parameters> channel;
		std::unique_ptr<factory::Monitor_BFER    ::parameters> monitor;
		std::unique_ptr<factory::Terminal        ::parameters> terminal;

		float R;         // the code rate (R=K/N)
		float ebn0_min;  // min SNR value
		float ebn0_max;  // max SNR value
		float ebn0_step; // SNR step
	};

	params init_params(int argc, char** argv)
	{
		params p;

		p.source   = std::unique_ptr<factory::Source          ::parameters>(new factory::Source          ::parameters());
		p.codec    = std::unique_ptr<factory::Codec_repetition::parameters>(new factory::Codec_repetition::parameters());
		p.modem    = std::unique_ptr<factory::Modem           ::parameters>(new factory::Modem           ::parameters());
		p.channel  = std::unique_ptr<factory::Channel         ::parameters>(new factory::Channel         ::parameters());
		p.monitor  = std::unique_ptr<factory::Monitor_BFER    ::parameters>(new factory::Monitor_BFER    ::parameters());
		p.terminal = std::unique_ptr<factory::Terminal        ::parameters>(new factory::Terminal        ::parameters());

		p.ebn0_min  =  0.00f;
		p.ebn0_max  = 10.01f;
		p.ebn0_step =  1.00f;

		std::vector<factory::Factory::parameters*> params_list = { p.source .get(), p.codec  .get(), p.modem   .get(),
		                                                           p.channel.get(), p.monitor.get(), p.terminal.get() };

		// parse the command for the given parameters and fill them
		factory::Command_parser cp(argc, argv, params_list, true);
		if (cp.parsing_failed())
		{
			cp.print_help    ();
			cp.print_warnings();
			cp.print_errors  ();
			std::exit(1);
		}

		std::cout << "# Simulation parameters: " << std::endl;
		factory::Header::print_parameters(params_list); // display the headers (= print the AFF3CT parameters on the screen)
		std::cout << "#" << std::endl;
		cp.print_warnings();

		p.R = (float)p.codec->enc->K / (float)p.codec->enc->N_cw; // compute the code rate

		return p;
	}

.. code-block:: cpp
	:caption: Factory: modules
	:name: lst_factory_modules
	:emphasize-lines: 8-9,18-24
	:linenos:

	struct modules
	{
		std::unique_ptr<module::Source<>>       source;
		std::unique_ptr<module::Codec_SIHO<>>   codec;
		std::unique_ptr<module::Modem<>>        modem;
		std::unique_ptr<module::Channel<>>      channel;
		std::unique_ptr<module::Monitor_BFER<>> monitor;
		                module::Encoder<>*      encoder;
		                module::Decoder_SIHO<>* decoder;

		std::vector<const module::Module*> list;
	};

	modules init_modules(const params &p)
	{
		modules m;

		m.source  = std::unique_ptr<module::Source      <>>(p.source ->build());
		m.codec   = std::unique_ptr<module::Codec_SIHO  <>>(p.codec  ->build());
		m.modem   = std::unique_ptr<module::Modem       <>>(p.modem  ->build());
		m.channel = std::unique_ptr<module::Channel     <>>(p.channel->build());
		m.monitor = std::unique_ptr<module::Monitor_BFER<>>(p.monitor->build());
		m.encoder = m.codec->get_encoder().get();
		m.decoder = m.codec->get_decoder_siho().get();

		m.list = {m.source.get(), m.modem.get(), m.channel.get(), m.monitor.get(), m.encoder, m.decoder };

		// configuration of the module tasks
		for (auto& mod : m.list)
			for (auto& tsk : mod->tasks)
			{
				tsk->set_autoalloc  (true ); // enable the automatic allocation of the data in the tasks
				tsk->set_autoexec   (false); // disable the auto execution mode of the tasks
				tsk->set_debug      (false); // disable the debug mode
				tsk->set_debug_limit(16   ); // display only the 16 first bits if the debug mode is enabled
				tsk->set_stats      (true ); // enable the statistics

				// enable the fast mode (= disable the useless verifs in the tasks) if there is no debug and stats modes
				if (!tsk->is_debug() && !tsk->is_stats())
					tsk->set_fast(true);
			}

		return m;
	};

.. code-block:: cpp
	:caption: Factory: utils
	:name: lst_factory_utils
	:emphasize-lines: 8,21
	:linenos:

	struct utils
	{
		            std::unique_ptr<tools::Sigma<>  > noise;     // a sigma noise type
		std::vector<std::unique_ptr<tools::Reporter>> reporters; // list of reporters dispayed in the terminal
		            std::unique_ptr<tools::Terminal>  terminal;  // manage the output text in the terminal
	};

	utils init_utils(const params &p, const modules &m)
	{
		utils u;

		// create a sigma noise type
		u.noise = std::unique_ptr<tools::Sigma<>>(new tools::Sigma<>());
		// report the noise values (Es/N0 and Eb/N0)
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_noise<>(*u.noise)));
		// report the bit/frame error rates
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_BFER<>(*m.monitor)));
		// report the simulation throughputs
		u.reporters.push_back(std::unique_ptr<tools::Reporter>(new tools::Reporter_throughput<>(*m.monitor)));
		// create a terminal that will display the collected data from the reporters
		u.terminal = std::unique_ptr<tools::Terminal>(p.terminal->build(u.reporters));

		return u;
	}

SystemC/TLM
===========

Alternatively, the AFF3CT modules support TLM interfaces, the ``systemc``
example illustrates this feature.

.. code-block:: cpp
	:caption: SystemC/TLM: main function
	:name: lst_systemc_main
	:emphasize-lines: 13-18,33-54,59-61,70-72
	:linenos:

	#include <aff3ct.hpp>
	using namespace aff3ct;

	int sc_main(int argc, char** argv)
	{
		auto p = init_params ( ); // get parameters defined by the user
		auto m = init_modules(p); // create the AFF3CT modules
		auto u = init_utils  (m); // create the AFF3CT utils

		// display the legend in the terminal
		u.terminal->legend();

		// add a callback to the monitor to call the "sc_core::sc_stop()" function
		m.monitor->add_handler_check([&m, &u]() -> void
		{
			if (m.monitor->fe_limit_achieved() || u.terminal->is_interrupt())
				sc_core::sc_stop();
		});

		// a loop over the various SNRs
		for (auto ebn0 = p.ebn0_min; ebn0 < p.ebn0_max; ebn0 += p.ebn0_step)
		{
			// compute the current sigma for the channel noise
			const auto esn0  = tools::ebn0_to_esn0 (ebn0, p.R);
			const auto sigma = tools::esn0_to_sigma(esn0     );

			u.noise->set_noise(sigma, ebn0, esn0);

			// update the sigma of the modem and the channel
			m.modem  ->set_noise(*u.noise);
			m.channel->set_noise(*u.noise);

			// create "sc_core::sc_module" instances for each task
			using namespace module;
			m.source ->sc.create_module(+src::tsk::generate    );
			m.encoder->sc.create_module(+enc::tsk::encode      );
			m.modem  ->sc.create_module(+mdm::tsk::modulate    );
			m.modem  ->sc.create_module(+mdm::tsk::demodulate  );
			m.channel->sc.create_module(+chn::tsk::add_noise   );
			m.decoder->sc.create_module(+dec::tsk::decode_siho );
			m.monitor->sc.create_module(+mnt::tsk::check_errors);

			// declare a SystemC duplicator to duplicate the source module output
			tools::SC_Duplicator duplicator;

			// bind the sockets between the modules
			m.source ->sc[+src::tsk::generate   ].s_out[+src::sck::generate   ::U_K ](duplicator                            .s_in                               );
			duplicator                           .s_out1                             (m.monitor->sc[+mnt::tsk::check_errors].s_in[+mnt::sck::check_errors::U   ]);
			duplicator                           .s_out2                             (m.encoder->sc[+enc::tsk::encode      ].s_in[+enc::sck::encode      ::U_K ]);
			m.encoder->sc[+enc::tsk::encode     ].s_out[+enc::sck::encode     ::X_N ](m.modem  ->sc[+mdm::tsk::modulate    ].s_in[+mdm::sck::modulate    ::X_N1]);
			m.modem  ->sc[+mdm::tsk::modulate   ].s_out[+mdm::sck::modulate   ::X_N2](m.channel->sc[+chn::tsk::add_noise   ].s_in[+chn::sck::add_noise   ::X_N ]);
			m.channel->sc[+chn::tsk::add_noise  ].s_out[+chn::sck::add_noise  ::Y_N ](m.modem  ->sc[+mdm::tsk::demodulate  ].s_in[+mdm::sck::demodulate  ::Y_N1]);
			m.modem  ->sc[+mdm::tsk::demodulate ].s_out[+mdm::sck::demodulate ::Y_N2](m.decoder->sc[+dec::tsk::decode_siho ].s_in[+dec::sck::decode_siho ::Y_N ]);
			m.decoder->sc[+dec::tsk::decode_siho].s_out[+dec::sck::decode_siho::V_K ](m.monitor->sc[+mnt::tsk::check_errors].s_in[+mnt::sck::check_errors::V   ]);

			// display the performance (BER and FER) in real time (in a separate thread)
			u.terminal->start_temp_report();

			// start the SystemC simulation
			sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
			sc_core::sc_start();

			// display the performance (BER and FER) in the terminal
			u.terminal->final_report();

			// reset the monitor and the terminal for the next SNR
			m.monitor->reset();
			u.terminal->reset();

			// dirty way to create a new SystemC simulation context
			sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
			sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
		}

		// display the statistics of the tasks (if enabled)
		tools::Stats::show(m.list, true);

		return 0;
	}
