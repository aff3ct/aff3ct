#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_std.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace factory
{
template <class D1, class D2>
Decoder_turbo::parameters<D1,D2>
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_turbo_name, prefix),
  sub1(new typename D1::parameters(std::is_same<D1,D2>() ? prefix+"-sub" : prefix+"-sub1")),
  sub2(new typename D2::parameters(std::is_same<D1,D2>() ? prefix+"-sub" : prefix+"-sub2")),
  itl (new Interleaver::parameters("itl")),
  sf  (new Scaling_factor::parameters(prefix+"-sf")),
  fnc (new Flip_and_check::parameters(prefix+"-fnc"))
{
	this->type   = "TURBO";
	this->implem = "FAST";
}

template <class D1, class D2>
Decoder_turbo::parameters<D1,D2>* Decoder_turbo::parameters<D1,D2>
::clone() const
{
	return new Decoder_turbo::parameters<D1,D2>(*this);
}

template <class D1, class D2>
std::vector<std::string> Decoder_turbo::parameters<D1,D2>
::get_names() const
{
	auto n = Decoder::parameters::get_names();
	if (sf   != nullptr) { auto nn = sf  ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (fnc  != nullptr) { auto nn = fnc ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sub1 != nullptr) { auto nn = sub1->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

template <class D1, class D2>
std::vector<std::string> Decoder_turbo::parameters<D1,D2>
::get_short_names() const
{
	auto sn = Decoder::parameters::get_short_names();
	if (sf   != nullptr) { auto nn = sf  ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (fnc  != nullptr) { auto nn = fnc ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sub1 != nullptr) { auto nn = sub1->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

template <class D1, class D2>
std::vector<std::string> Decoder_turbo::parameters<D1,D2>
::get_prefixes() const
{
	auto p = Decoder::parameters::get_prefixes();
	if (sf   != nullptr) { auto nn = sf  ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (fnc  != nullptr) { auto nn = fnc ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sub1 != nullptr) { auto nn = sub1->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

template <class D1, class D2>
void Decoder_turbo::parameters<D1,D2>
::get_description(tools::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();

	args.erase({p+"-cw-size", "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	tools::add_options(args.at({p+"-type", "D"}), 0, "TURBO"      );
	tools::add_options(args.at({p+"-implem"   }), 0, "STD", "FAST");

	args.add(
		{p+"-ite", "i"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"maximal number of iterations in the turbo.");

	args.add(
		{p+"-sc"},
		tools::None(),
		"enables the self corrected decoder (requires \"--crc-type\").");

	args.add(
		{p+"-json"},
		tools::None(),
		"enable the json output trace.");


	sf->get_description(args);

	auto psf = sf->get_prefix();

	args.erase({psf+"-ite"});

	fnc->get_description(args);

	auto pfnc = fnc->get_prefix();

	args.erase({pfnc+"-size"     });
	args.erase({pfnc+"-fra",  "F"});
	args.erase({pfnc+"-ite",  "i"});

	sub1->get_description(args);

	auto ps1 = sub1->get_prefix();

	args.erase({ps1+"-info-bits", "K"});
	args.erase({ps1+"-cw-size",   "N"});
	args.erase({ps1+"-fra",       "F"});

	if (!std::is_same<D1,D2>())
	{
		auto ps2 = sub2->get_prefix();

		args.erase({ps2+"-info-bits", "K"});
		args.erase({ps2+"-cw-size",   "N"});
		args.erase({ps2+"-fra",       "F"});
	}
}

template <class D1, class D2>
void Decoder_turbo::parameters<D1,D2>
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-ite", "i"})) this->n_ite          = vals.to_int({p+"-ite", "i"});
	if(vals.exist({p+"-sc"      })) this->self_corrected = true;
	if(vals.exist({p+"-json"    })) this->enable_json    = true;

	this->sub1->K        = this->K;
	this->sub2->K        = this->K;
	this->sub1->n_frames = this->n_frames;
	this->sub2->n_frames = this->n_frames;

	sub1->store(vals);
	sub2->store(vals);

	if (this->enable_json)
	{
		this->sub1->implem        = "GENERIC_JSON";
		this->sub2->implem        = "GENERIC_JSON";
		this->sub1->simd_strategy = "";
		this->sub2->simd_strategy = "";
	}

	this->tail_length = this->sub1->tail_length + this->sub2->tail_length;
	this->N_cw        = this->sub1->N_cw + this->sub2->N_cw - this->K;
	this->R           = (float)this->K / (float)this->N_cw;

	if (itl != nullptr)
	{
		this->itl->core->size     = this->K;
		this->itl->core->n_frames = this->n_frames;

		itl->store(vals);

		if (this->sub1->standard == "LTE" && !vals.exist({"itl-type"}))
			this->itl->core->type = "LTE";

		if (this->sub1->standard == "CCSDS" && !vals.exist({"itl-type"}))
			this->itl->core->type = "CCSDS";
	}

	this->sf->n_ite = this->n_ite;

	sf->store(vals);

	this->fnc->size     = this->K;
	this->fnc->n_frames = this->n_frames;
	this->fnc->n_ite    = this->n_ite;

	fnc->store(vals);
}

template <class D1, class D2>
void Decoder_turbo::parameters<D1,D2>
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));
		if (this->tail_length && full)
			headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));
		headers[p].push_back(std::make_pair("Enable json", ((this->enable_json) ? "on" : "off")));
		headers[p].push_back(std::make_pair("Self-corrected", ((this->self_corrected) ? "on" : "off")));

		sf->get_headers(headers, full);
		fnc->get_headers(headers, full);

		this->sub1->get_headers(headers, full);
		if (!std::is_same<D1,D2>())
		{
			this->sub2->get_headers(headers, full);
		}
	}
}

template <class D1, class D2>
template <typename B, typename Q>
module::Decoder_turbo<B,Q>* Decoder_turbo::parameters<D1,D2>
::build(const module::Interleaver<Q>  &itl,
              module::Decoder_SISO<Q> &siso_n,
              module::Decoder_SISO<Q> &siso_i,
              const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (this->type == "TURBO")
	{
		if (this->implem == "STD" ) return new module::Decoder_turbo_std <B,Q>(this->K, this->N_cw, this->n_ite, itl, siso_n, siso_i, this->sub1->buffered);
		if (this->implem == "FAST") return new module::Decoder_turbo_fast<B,Q>(this->K, this->N_cw, this->n_ite, itl, siso_n, siso_i, this->sub1->buffered);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <class D1, class D2>
template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo::parameters<D1,D2>
::build(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	return Decoder::parameters::build<B,Q>(encoder);
}

template <typename B, typename Q, class D1, class D2>
module::Decoder_turbo<B,Q>* Decoder_turbo
::build(const parameters<D1,D2>       &params,
        const module::Interleaver<Q>  &itl,
              module::Decoder_SISO<Q> &siso_n,
              module::Decoder_SISO<Q> &siso_i,
              const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(itl, siso_n, siso_i, encoder);
}

template <typename B, typename Q, class D1, class D2>
module::Decoder_SIHO<B,Q>* Decoder_turbo
::build(const parameters<D1,D2> &params, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(encoder);
}
}
}
