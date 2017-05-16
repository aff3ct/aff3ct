#ifndef FRAME_DUMPER_HPP_
#define FRAME_DUMPER_HPP_

#include <vector>
#include <fstream>

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Frame_dumper
{
protected:
	const B   *U_K;
	const B   *X_N;
	const R   *Y_N;
	const int *I_N;

	const int K_info;
	const int K;
	const int N;
	const int N_mod;
	const int n_frames;

	std::vector<std::vector<B  >> buff_src;
	std::vector<std::vector<B  >> buff_enc;
	std::vector<std::vector<R  >> buff_chn;
	std::vector<std::vector<int>> buff_itl;

public:
	Frame_dumper(const int K_info, const int K, const int N, const int N_mod, const int n_frames);
	Frame_dumper(const int K_info, const int K, const int N,                  const int n_frames);
	Frame_dumper(                  const int K, const int N,                  const int n_frames);
	Frame_dumper(const int K_info,                                            const int n_frames);

	virtual ~Frame_dumper();

	void set_U_K(const B   *U_K);
	void set_X_N(const B   *X_N);
	void set_Y_N(const R   *Y_N);
	void set_I_N(const int *I_N);

	int get_K_info  ();
	int get_K       ();
	int get_N       ();
	int get_N_mod   ();
	int get_n_frames();

	const std::vector<std::vector<B  >>& get_buff_src();
	const std::vector<std::vector<B  >>& get_buff_enc();
	const std::vector<std::vector<R  >>& get_buff_chn();
	const std::vector<std::vector<int>>& get_buff_itl();

	virtual void dump (const std::string& base_path);
	virtual void add  (const int frame_id = 0      );
	virtual void clear(                            );

protected:
	void write_text_body    (std::ofstream &file, const std::vector<std::vector<B  >> &fra);
	void write_text_body_itl(std::ofstream &file, const std::vector<std::vector<int>> &fra);
	void write_binary_body  (std::ofstream &file, const std::vector<std::vector<R  >> &fra);

private:
	void checks();
};
}
}

#endif /* FRAME_DUMPER_HPP_ */
