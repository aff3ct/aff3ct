#ifndef MONITOR_MI_BFER_HPP_
#define MONITOR_MI_BFER_HPP_

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "../MI/Monitor_MI.hpp"
#include "../BFER/Monitor_BFER.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Monitor_MI_BFER : public Monitor_MI<B,R>, public Monitor_BFER<B>
{
public:
	struct Values_t
	{
		typename Monitor_MI<B,R>::Values_t& mi_vals;
		typename Monitor_BFER<B>::Values_t& bfer_vals;

		Values_t(typename Monitor_MI<B,R>::Values_t& mi_vals, typename Monitor_BFER<B>::Values_t& bfer_vals)
		: mi_vals(mi_vals), bfer_vals(bfer_vals){}

		static constexpr unsigned n_attributes = Monitor_MI<B,R>::Values_t::n_attributes + Monitor_BFER<B>::Values_t::n_attributes;

		Values_t& operator+=(const Values_t& o)
		{
			mi_vals   += o.mi_vals;
			bfer_vals += o.bfer_vals;
			return *this;
		}

		Values_t& operator=(const Values_t& o)
		{
			mi_vals   = o.mi_vals;
			bfer_vals = o.bfer_vals;
			return *this;
		}

		void reset()
		{
			mi_vals  .reset();
			bfer_vals.reset();
		}

	#ifdef ENABLE_MPI
		static void create_MPI_struct(int          blen         [n_attributes],
		                              MPI_Aint     displacements[n_attributes],
		                              MPI_Datatype oldtypes     [n_attributes])
		{
			auto MI_blen            = &blen         [0];
			auto MI_displacements   = &displacements[0];
			auto MI_oldtypes        = &oldtypes     [0];
			auto BFER_blen          = &blen         [Monitor_MI<B,R>::Values_t::n_attributes];
			auto BFER_displacements = &displacements[Monitor_MI<B,R>::Values_t::n_attributes]; // TODO: warning might be wrong displacement next to MI attributes
			auto BFER_oldtypes      = &oldtypes     [Monitor_MI<B,R>::Values_t::n_attributes];

			Monitor_MI<B,R>::Values_t::create_MPI_struct(  MI_blen    MI_displacements,   MI_oldtypes);
			Monitor_BFER<B>::Values_t::create_MPI_struct(BFER_blen, BFER_displacements, BFER_oldtypes);
		}
	#endif
	};

private:
	Values_t vals;

public:
	Monitor_MI_BFER(const int K, const int N, const unsigned max_n_cf,
	                const unsigned max_fe, const bool count_unknown_values = false, const int n_frames = 1);
	virtual ~Monitor_MI_BFER() = default;


	virtual void add_handler_check(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor_MI_BFER<B,R>& m);

protected:
	virtual R _get_mutual_info(const B *X, const R *Y, const int frame_id);
};
}
}

#endif /* MONITOR_MI_BFER_HPP_ */
