#include <functional>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <mutex>
#ifdef AFF3CT_HWLOC
#include <hwloc.h>
#endif

#include "Tools/Exception/exception.hpp"
#include "Tools/Thread_pinning/Thread_pinning.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

#ifdef AFF3CT_HWLOC
static hwloc_topology_t g_topology;
static int g_topodepth = 0;
#endif
static bool g_is_init = false;
static std::mutex g_mtx;
static bool g_enable_logs = false;

void Thread_pinning
::init()
{
	if (!g_is_init)
	{
		g_mtx.lock();
		if (!g_is_init)
		{
			g_is_init = true;

#ifdef AFF3CT_HWLOC
			/* Allocate and initialize topology object. */
			hwloc_topology_init(&g_topology);

			/* ... Optionally, put detection configuration here to ignore
			 some objects types, define a synthetic topology, etc....
			 The default is to detect all the objects of the machine that
			 the caller is allowed to access.  See Configure Topology
			 Detection. */

			/* Perform the topology detection. */
			hwloc_topology_load(g_topology);

			/* Optionally, get some additional topology information
			 in case we need the topology depth later. */
			g_topodepth = hwloc_topology_get_depth(g_topology);
#endif
		}
		g_mtx.unlock();
	}
}

void Thread_pinning
::destroy()
{
	if (g_is_init)
	{
		g_mtx.lock();
		if (g_is_init)
		{
#ifdef AFF3CT_HWLOC
			/* Destroy topology object. */
			hwloc_topology_destroy(g_topology);
			g_topodepth = 0;
#endif
			g_is_init = false;
		}
		g_mtx.unlock();
	}

}

void Thread_pinning
::set_logs(const bool enable_logs)
{
	g_mtx.lock();
	g_enable_logs = enable_logs;
	g_mtx.unlock();
}

bool Thread_pinning
::is_logs()
{
	return g_enable_logs;
}

void Thread_pinning
::pin(const size_t puid)
{
	g_mtx.lock();
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		int pu_depth = hwloc_get_type_or_below_depth(g_topology, HWLOC_OBJ_PU);
		hwloc_obj_t pu_obj = hwloc_get_obj_by_depth(g_topology, pu_depth, puid);

		if (pu_obj == nullptr)
		{
			std::stringstream message;
			message << "'pu_obj' is nullptr ('puid' = " << puid << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		/* Get a copy of its cpuset that we may modify. */
		hwloc_cpuset_t cpuset = hwloc_bitmap_dup(pu_obj->cpuset);

		/* Get only one logical processor (in case the core is
		SMT/hyper-threaded). */
		hwloc_bitmap_singlify(cpuset);

		if (g_enable_logs)
		{
			char c[128];
			hwloc_bitmap_snprintf(c, 128, cpuset);

			std::cerr << "Thread pinning info -- "
			          << "PU logical index (hwloc): " << pu_obj->logical_index << " -- "
			          << "P OS index: " << pu_obj->os_index << " -- "
			          << "bitmap: " << c << std::endl;
		}

		/* And try to bind ourself there. */
		if (hwloc_set_cpubind(g_topology, cpuset, HWLOC_CPUBIND_THREAD))
		{
			char *str;
			int error = errno;
			hwloc_bitmap_asprintf(&str, pu_obj->cpuset);
			printf("Couldn't bind to cpuset %s: %s\n", str, strerror(error));
			free(str);
		}

		/* Free our cpuset copy */
		hwloc_bitmap_free(cpuset);
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'pin' method if you have not call the 'init' method before, nothing will "
			          << "be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'pin' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
	g_mtx.unlock();
}

void Thread_pinning
::unpin()
{
	g_mtx.lock();
#ifdef AFF3CT_HWLOC
	if (!g_is_init)
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'unpin' method if you have not call the 'init' method before, nothing "
			          << "will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'unpin' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
	g_mtx.unlock();
}

std::string Thread_pinning
::get_cur_cpuset_str()
{
#ifdef AFF3CT_HWLOC
	hwloc_cpuset_t cur_cpuset = hwloc_bitmap_alloc();

	hwloc_get_cpubind(g_topology, cur_cpuset, HWLOC_CPUBIND_THREAD);

	char c[128];
	hwloc_bitmap_snprintf(c, 128, cur_cpuset);

	/* Free our cpuset copy */
	hwloc_bitmap_free(cur_cpuset);

	return std::string(c);
#else
	std::stringstream message;
	message << "'get_cur_cpuset_str' method can be called only if AFF3CT is linked with the 'hwloc' library.";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
#endif
}
