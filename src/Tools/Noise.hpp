#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <utility>
#include <string>

namespace aff3ct
{
namespace tools
{

enum class Noise_type : uint8_t{ SIGMA, ROP, EP }; // Sigma (SNR variance), Received optical power, Erasure Probability


template <typename R = float>
class Noise
{
public:
	Noise();
	explicit Noise(const R noise, const Noise_type t = Noise_type::SIGMA);
	Noise(const Noise<R>& other);
	Noise(Noise<R>&& other);
	~Noise();

	bool is_set   () const throw();
	bool has_type () const throw();
	bool has_noise() const throw();

	Noise_type get_type () const; // return the noise type,                           throw if not set
	R          get_noise() const; // return the stocked noise,                        throw if not set
	R          get_sigma() const; // return the noise as SIGMA type and has been set, throw if it is not
	R          get_rop  () const; // return the noise as ROP   type and has been set, throw if it is not
	R          get_ep   () const; // return the noise as EP    type and has been set, throw if it is not

	const R get_ebn0() const; // return EB/N0 when the noise is of SIGMA type and has been set, throw if it is not
	const R get_esn0() const; // return ES/N0 when the noise is of SIGMA type and has been set, throw if it is not

	void set_noise(const R noise, const Noise_type t = Noise_type::SIGMA);
	void set_rop  (const R rop  ); // set the noise and its type as ROP
	void set_ep   (const R ep   ); // set the noise and its type as EP
	void set_sigma(const R sigma); // set the noise and its type as SIGMA
	void set_sigma(const R sigma, const R ebn0, const R esn0); // set the noise and its type as SIGMA
	                                                           // set also EB/N0 and ES/N0

	static Noise_type str2type(const std::string& str);
	static std::string type2str(const Noise_type& t);

	Noise& operator=(const Noise<R>& other);
	Noise& operator=(Noise<R>&& other);

protected:
	std::pair<Noise_type, bool> _t;
	std::pair<R,          bool> _n;
	std::pair<R,          bool> _ebn0;
	std::pair<R,          bool> _esn0;

	void check();

	void set_n(const R          n);
	void set_t(const Noise_type t);

	void init();
};

}
}

#endif // NOISE_HPP__