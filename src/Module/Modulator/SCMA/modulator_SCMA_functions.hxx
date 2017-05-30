namespace aff3ct
{
namespace tools
{
template <typename R>
inline R psi_0(const std::complex<R>& d, const R& n0)
{
	return std::exp(-std::norm(d) / n0);
}

template <typename R>
inline R psi_1(const std::complex<R>& d, const R& n0)
{
	return 1 / (std::norm(d) + n0);
}

template <typename R>
inline R psi_2(const std::complex<R>& d, const R& n0)
{
	return 1 / (8 * std::pow(std::norm(d),2) + n0);
}

template <typename R>
inline R psi_3(const std::complex<R>& d, const R& n0)
{
	return 1 / (4 * std::pow(std::norm(d),2) + n0);
}
}
}