#include <type_traits>
#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"
#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hpp"

namespace aff3ct
{
namespace factory
{

template <class E1, class E2>
Encoder_turbo<E1,E2>
::Encoder_turbo(const std::string &prefix)
: Encoder(Encoder_turbo_name, prefix),
  itl(new Interleaver("itl")),
  sub1(new E1(std::is_same<E1,E2>() ? prefix+"-sub" : prefix+"-sub1")),
  sub2(new E2(std::is_same<E1,E2>() ? prefix+"-sub" : prefix+"-sub2"))
{
	this->type = "TURBO";
}

template <class E1, class E2>
Encoder_turbo<E1,E2>* Encoder_turbo<E1,E2>
::clone() const
{
	return new Encoder_turbo<E1,E2>(*this);
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo<E1,E2>
::get_names() const
{
	auto n = Encoder::get_names();
	if (sub1 != nullptr) { auto nn = sub1->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo<E1,E2>
::get_short_names() const
{
	auto sn = Encoder::get_short_names();
	if (sub1 != nullptr) { auto nn = sub1->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

template <class E1, class E2>
std::vector<std::string> Encoder_turbo<E1,E2>
::get_prefixes() const
{
	auto p = Encoder::get_prefixes();
	if (sub1 != nullptr) { auto nn = sub1->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sub2 != nullptr) { auto nn = sub2->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl  != nullptr) { auto nn = itl ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

template <class E1, class E2>
void Encoder_turbo<E1,E2>
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_turbo::";

	args.erase({p+"-cw-size", "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"});
	}

	Encoder_turbo_common::add_args_and_options(args, p, class_name);

	sub1->get_description(args);

	auto ps1 = sub1->get_prefix();

	args.erase({ps1+"-info-bits", "K"});
	args.erase({ps1+"-cw-size",   "N"});
	args.erase({ps1+"-seed",      "S"});
	args.erase({ps1+"-path"          });

	if (!std::is_same<E1,E2>())
	{
		sub2->get_description(args);

		auto ps2 = sub2->get_prefix();

		args.erase({ps2+"-info-bits", "K"});
		args.erase({ps2+"-cw-size",   "N"});
		args.erase({ps2+"-seed",      "S"});
		args.erase({ps2+"-path"          });
	}
}

template <class E1, class E2>
void Encoder_turbo<E1,E2>
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-json-path"})) this->json_path = vals.at({p+"-json-path"});

	this->sub1->K    = this->K;
	this->sub2->K    = this->K;
	this->sub1->seed = this->seed;
	this->sub2->seed = this->seed;

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

	if (itl != nullptr)
	{
		this->itl->core->size = this->K;

		itl->store(vals);

		if (this->sub1->standard == "LTE" && !vals.exist({"itl-type"}))
			this->itl->core->type = "LTE";

		if (this->sub1->standard == "CCSDS" && !vals.exist({"itl-type"}))
			this->itl->core->type = "CCSDS";
	}
}

template <class E1, class E2>
void Encoder_turbo<E1,E2>
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

	if (itl != nullptr)
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
module::Encoder_turbo<B>* Encoder_turbo<E1,E2>
::build(const module::Encoder<B> &enc_n,
        const module::Encoder<B> &enc_i,
        const module::Interleaver<B> &itl) const
{
	if (this->sub1->buffered)
	{
		if (this->type == "TURBO") return new module::Encoder_turbo       <B>(this->K, this->N_cw, enc_n, enc_i, itl);
	}
	else if (&enc_n == &enc_i)
	{
		if (this->type == "TURBO") return new module::Encoder_turbo_legacy<B>(this->K, this->N_cw, enc_n, itl);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

}
}
