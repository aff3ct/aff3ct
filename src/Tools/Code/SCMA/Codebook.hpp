#ifndef CODEBOOK_HPP_
#define CODEBOOK_HPP_

#include <string>
#include <complex>
#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Codebook
{
public:
	explicit Codebook(const std::string& codebook_path);
	~Codebook() = default;

	const std::complex<R>& operator() (int u, int o, int c) const;

	int get_number_of_users()                const;
	int get_number_of_resources_per_user()   const;
	int get_number_of_users_per_resource()   const;
	int get_number_of_orthogonal_resources() const;
	int get_codebook_size()                  const;
	int get_resource_to_user(int r, int idx) const;
	int get_user_to_resource(int u, int idx) const;

private:
	int number_of_users;
	int number_of_orthogonal_resources;
	int number_of_resources_per_user;
	int number_of_users_per_resource;
	int codebook_size;

	std::vector<std::vector<std::vector<std::complex<R>>>> data;
	std::vector<std::vector<int>> resource_to_user;
	std::vector<std::vector<int>> user_to_resource;

	void read_codebook(const std::string& codebook_path);
};

}
}

#include "Codebook.hxx"

#endif // CODEBOOK_HPP_
