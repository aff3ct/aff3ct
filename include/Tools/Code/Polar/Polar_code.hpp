/*!
 * \file
 * \brief Class tools::Polar_code.
 */
#ifndef POLAR_CODE_HPP_
#define POLAR_CODE_HPP_

#include <string>
#include <vector>
#include <cstdint>

namespace aff3ct
{
namespace tools
{
std::string display_kernel(const std::vector<std::vector<bool>>& pattern_bits);
void read_polar_MK_code(const std::string                                 &code_path,
                              std::vector<std::vector<std::vector<bool>>> &kernel_matrices,
                              std::vector<uint32_t>                       &stages);

class Polar_code
{
private:
	int N;
	std::vector<std::vector<std::vector<bool>>> kernel_matrices;
	std::vector<uint32_t> stages;

public:
	Polar_code(const std::string &code_path);
	Polar_code(const int N, const std::vector<std::vector<bool>> &kernel_matrix);
	Polar_code(const std::vector<std::vector<std::vector<bool>>> &kernel_matrices,
	           const std::vector<uint32_t> &stages);

	virtual ~Polar_code() = default;

	bool can_be_systematic() const;
	size_t is_mono_kernel() const;
	int get_codeword_size() const;
	const std::vector<std::vector<std::vector<bool>>>& get_kernel_matrices() const;
	const std::vector<uint32_t>& get_stages() const;

	size_t get_biggest_kernel_size() const;

private:
	void verif_MK();
	void verif();
};
}
}

#endif