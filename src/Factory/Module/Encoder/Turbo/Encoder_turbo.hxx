#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Encoder_turbo.hpp"

namespace aff3ct
{
namespace factory
{

template <class E1, class E2>
Encoder_turbo::parameters<E1,E2>
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_turbo_name, prefix),
  itl(new Interleaver::parameters("itl")),
  sub1(new typename E1::parameters(std::is_same<E1,E2>() ? prefix+"-sub" : prefix+"-sub1")),
  sub2(new typename E2::parameters(std::is_same<E1,E2>() ? prefix+"-sub" : prefix+"-sub2"))
{
	this->type = "TURBO";
}

template <class E1, class E2>
Encoder_turbo::parameters<E1,E2>* Encoder_turbo::parameters<E1,E2>
::clone() const
{
	auto clone = new Encoder_turbo::parameters<E1,E2>(*this);

	if (itl  != nullptr) { clone->itl  = itl ->clone(); }
	if (sub1 != nullptr) { clone->sub1 = sub1->clone(); }
	if (sub2 != nullptr) { clone->sub2 = sub2->clone(); }

	return clone;
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo::parameters<E1,E2>
::get_names() const
{
	auto n = Encoder::parameters::get_names();
	if (sub1 != nullptr) { auto nn = sub1->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo::parameters<E1,E2>
::get_short_names() const
{
	auto sn = Encoder::parameters::get_short_names();
	if (sub1 != nullptr) { auto nn = sub1->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo::parameters<E1,E2>
::get_prefixes() const
{
	auto p = Encoder::parameters::get_prefixes();
	if (sub1 != nullptr) { auto nn = sub1->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

template <class E1, class E2>
Encoder_turbo::parameters<E1,E2>
::~parameters()
{
	if (itl  != nullptr) { delete itl;  itl  = nullptr; }
	if (sub1 != nullptr) { delete sub1; sub1 = nullptr; }
	if (sub2 != nullptr) { delete sub2; sub2 = nullptr; }
}

template <class E1, class E2>
void Encoder_turbo::parameters<E1,E2>
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	itl->get_description(req_args, opt_args);

	auto pi = itl->get_prefix();

	req_args.erase({pi+"-size"    });
	opt_args.erase({pi+"-fra", "F"});

	tools::add_options(opt_args.at({p+"-type"}), 0, "TURBO");

	opt_args.add(
		{p+"-json-path"},
		tools::File(tools::openmode::write),
		"path to store the encoder and decoder traces formated in JSON.");

	sub1->get_description(req_args, opt_args);

	auto ps1 = sub1->get_prefix();

	req_args.erase({ps1+"-info-bits", "K"});
	req_args.erase({ps1+"-cw-size",   "N"});
	opt_args.erase({ps1+"-fra",       "F"});
	opt_args.erase({ps1+"-seed",      "S"});
	opt_args.erase({ps1+"-path"          });

	if (!std::is_same<E1,E2>())
	{
		sub2->get_description(req_args, opt_args);

		auto ps2 = sub2->get_prefix();

		req_args.erase({ps2+"-info-bits", "K"});
		req_args.erase({ps2+"-cw-size",   "N"});
		opt_args.erase({ps2+"-fra",       "F"});
		opt_args.erase({ps2+"-seed",      "S"});
		opt_args.erase({ps2+"-path"          });
	}
}

template <class E1, class E2>
void Encoder_turbo::parameters<E1,E2>
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-json-path"})) this->json_path = vals.at({p+"-json-path"});

	this->sub1->K        = this->K;
	this->sub2->K        = this->K;
	this->sub1->n_frames = this->n_frames;
	this->sub2->n_frames = this->n_frames;
	this->sub1->seed     = this->seed;
	this->sub2->seed     = this->seed;

	sub1->store(vals);
	sub2->store(vals);

	if (!this->json_path.empty())
	{
		this->sub1->type += "_JSON";
		this->sub2->type += "_JSON";
	}

	this->tail_length = this->sub1->tail_length + this->sub2->tail_length;
	this->N_cw        = this->sub1->N_cw + this->sub2->N_cw - this->K;
	this->R           = (float)this->K / (float)this->N_cw;

	this->itl->core->size     = this->K;
	this->itl->core->n_frames = this->n_frames;

	itl->store(vals);

	if (this->sub1->standard == "LTE" && !vals.exist({"itl-type"}))
		this->itl->core->type = "LTE";

	if (this->sub1->standard == "CCSDS" && !vals.exist({"itl-type"}))
		this->itl->core->type = "CCSDS";
}

template <class E1, class E2>
void Encoder_turbo::parameters<E1,E2>
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	itl->get_headers(headers);

	auto p = this->get_prefix();

	if (this->tail_length)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	if (!this->json_path.empty())
		headers[p].push_back(std::make_pair("Path to the JSON file", this->json_path));

	sub1->get_headers(headers, full);
	if (!std::is_same<E1,E2>())
		sub2->get_headers(headers, full);
}

template <class E1, class E2>
template <typename B>
module::Encoder<B>* Encoder_turbo::parameters<E1,E2>
::build(const module::Interleaver<B> &itl,
              module::Encoder    <B> *enc_n,
              module::Encoder    <B> *enc_i) const
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (this->sub1->buffered)
	{
		if (this->type == "TURBO") return new module::Encoder_turbo       <B>(this->K, this->N_cw, itl, *enc_n, *enc_i, this->n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (this->type == "TURBO") return new module::Encoder_turbo_legacy<B>(this->K, this->N_cw, itl, *enc_n,         this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, class E1, class E2>
module::Encoder<B>* Encoder_turbo
::build(const parameters<E1,E2>      &params,
        const module::Interleaver<B> &itl,
              module::Encoder    <B> *enc_n,
              module::Encoder    <B> *enc_i)
{
	return params.template build<B>(itl, enc_n, enc_i);
}
}
}
