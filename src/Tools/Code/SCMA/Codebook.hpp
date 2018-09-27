#ifndef CODEBOOK_HPP_
#define CODEBOOK_HPP_

#include <string>
#include <complex>
#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename R>
class Codebook
{
public:
	explicit Codebook(const std::string& codebook_path);
	~Codebook() = default;

	const std::complex<R>& operator() (int u, int o, int c) const;

	int get_number_of_users()                const;
	int get_number_of_orthogonal_resources() const;
	int get_codebook_size()                  const;
	int get_re_user(int r, int df)           const;

private:
	std::vector<std::vector<std::vector<std::complex<R>>>> data;

	int number_of_users;
	int number_of_orthogonal_resources;
	int codebook_size;

	int re_user[4][3];

	void read_codebook(const std::string& codebook_path);
};

}
}

#include "Codebook.hxx"

#endif // CODEBOOK_HPP_
