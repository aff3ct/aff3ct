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
static std::vector<hwloc_uint64_t> g_pinned_threads;
static hwloc_obj_t g_cur_core_obj;
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

			int core_depth = hwloc_get_type_or_below_depth(g_topology, HWLOC_OBJ_CORE);
			/* Get last core. */
			int depth_core = hwloc_get_type_or_below_depth(g_topology, HWLOC_OBJ_CORE);
			int core_id = hwloc_get_nbobjs_by_depth(g_topology, depth_core) -1;
			g_cur_core_obj = hwloc_get_obj_by_depth(g_topology, core_depth, core_id);
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
::pin()
{
	g_mtx.lock();
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		if (g_cur_core_obj)
		{
			/* Get the first PU of the core */
			if (g_cur_core_obj->arity == 0)
			{
				std::stringstream message;
				message << "Unsupported architecture, a core should have at least one PU.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			hwloc_obj_t cur_core_pu0_obj = g_cur_core_obj->children[0];

			if (g_enable_logs)
				std::cerr << "Thread pinning info -- "
			              << "CORE logical index (hwloc): " << g_cur_core_obj->logical_index << " -- "
				          << "PU logical index (hwloc): " << cur_core_pu0_obj->logical_index << " -- "
				          << "P OS index: " << cur_core_pu0_obj->os_index << std::endl;

			/* Get a copy of its cpuset that we may modify. */
			hwloc_cpuset_t cpuset = hwloc_bitmap_dup(cur_core_pu0_obj->cpuset);
			/* Get only one logical processor (in case the core is
			SMT/hyper-threaded). */
			hwloc_bitmap_singlify(cpuset);
			/* And try to bind ourself there. */
			if (hwloc_set_cpubind(g_topology, cpuset, HWLOC_CPUBIND_THREAD))
			{
				char *str;
				int error = errno;
				hwloc_bitmap_asprintf(&str, cur_core_pu0_obj->cpuset);
				printf("Couldn't bind to cpuset %s: %s\n", str, strerror(error));
				free(str);
			}
			/* Free our cpuset copy */
			hwloc_bitmap_free(cpuset);
		}

		/* Get next core. */
		g_cur_core_obj = g_cur_core_obj->prev_cousin;

		if (g_cur_core_obj == nullptr)
		{
			std::stringstream message;
			message << "There is no more available cores.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
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

void Thread_pinning
::example1()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * First example:
		 * Walk the topology with an array style, from level 0 (always
		 * the system level) to the lowest level (always the proc level).
		 *****************************************************************/

		char string[128];
		for (int depth = 0; depth < g_topodepth; depth++)
		{
			printf("*** Objects at level %d\n", depth);
			for (unsigned i = 0; i < hwloc_get_nbobjs_by_depth(g_topology, depth); i++)
			{
				hwloc_obj_type_snprintf(string, sizeof(string),
					hwloc_get_obj_by_depth(g_topology, depth, i), 0);
				printf("Index %u: %s\n", i, string);
			}
		}
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example1' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example1' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}

void Thread_pinning
::example2()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * Second example:
		 * Walk the topology with a tree style.
		 *****************************************************************/

		std::function<void(hwloc_topology_t, hwloc_obj_t, int)> print_children =
			[&print_children](hwloc_topology_t topology, hwloc_obj_t obj, int depth)
			{
				char type[32], attr[1024];
				unsigned i;
				hwloc_obj_type_snprintf(type, sizeof(type), obj, 0);
				printf("%*s%s", 2*depth, "", type);
				if (obj->os_index != (unsigned) -1)
					printf("#%u", obj->os_index);
				hwloc_obj_attr_snprintf(attr, sizeof(attr), obj, " ", 0);
				if (*attr)
					printf("(%s)", attr);

				printf("\n");
				for (i = 0; i < obj->arity; i++) {
					print_children(topology, obj->children[i], depth + 1);
				}
			};

		printf("*** Printing overall tree\n");
		print_children(g_topology, hwloc_get_root_obj(g_topology), 0);
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example2' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example2' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}

void Thread_pinning
::example3()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * Third example:
		 * Print the number of packages.
		 *****************************************************************/

		int depth = hwloc_get_type_depth(g_topology, HWLOC_OBJ_PACKAGE);
		if (depth == HWLOC_TYPE_DEPTH_UNKNOWN)
		{
			printf("*** The number of packages is unknown\n");
		}
		else
		{
			printf("*** %u package(s)\n",
			hwloc_get_nbobjs_by_depth(g_topology, depth));
		}
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example3' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example3' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}

void Thread_pinning
::example4()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * Fourth example:
		 * Compute the amount of cache that the first logical processor
		 * has above it.
		 *****************************************************************/

		// This block has been commented to compile with hwloc v2 (HWLOC_OBJ_CACHE does not exist with this version)
		// int levels = 0;
		// unsigned long size = 0;
		// for (hwloc_obj_t obj = hwloc_get_obj_by_type(g_topology, HWLOC_OBJ_PU, 0); obj; obj = obj->parent)
		// {
		// 	if (obj->type == HWLOC_OBJ_CACHE)
		// 	{
		// 		levels++;
		// 		size += obj->attr->cache.size;
		// 	}
		// }
		// printf("*** Logical processor 0 has %d caches totaling %luKB\n", levels, size / 1024);
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example4' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example4' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}

void Thread_pinning
::example5()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * Fifth example:
		 * Bind to only one thread of the last core of the machine.
		 *
		 * First find out where cores are, or else smaller sets of CPUs if
		 * the OS doesn't have the notion of a "core".
		 *****************************************************************/

		int depth = hwloc_get_type_or_below_depth(g_topology, HWLOC_OBJ_CORE);
		/* Get last core. */
		hwloc_obj_t obj = hwloc_get_obj_by_depth(g_topology, depth,
			hwloc_get_nbobjs_by_depth(g_topology, depth) - 1);
		if (obj)
		{
			/* Get a copy of its cpuset that we may modify. */
			hwloc_cpuset_t cpuset = hwloc_bitmap_dup(obj->cpuset);
			/* Get only one logical processor (in case the core is
			SMT/hyper-threaded). */
			hwloc_bitmap_singlify(cpuset);
			/* And try to bind ourself there. */
			if (hwloc_set_cpubind(g_topology, cpuset, 0))
			{
				char *str;
				int error = errno;
				hwloc_bitmap_asprintf(&str, obj->cpuset);
				printf("Couldn't bind to cpuset %s: %s\n", str, strerror(error));
				free(str);
			}
			/* Free our cpuset copy */
			hwloc_bitmap_free(cpuset);
		}
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example5' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example5' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}

void Thread_pinning
::example6()
{
#ifdef AFF3CT_HWLOC
	if (g_is_init)
	{
		/*****************************************************************
		 * Sixth example:
		 * Allocate some memory on the last NUMA node, bind some existing
		 * memory to the last NUMA node.
		 *****************************************************************/

		/* Get last node. */
		int n = hwloc_get_nbobjs_by_type(g_topology, HWLOC_OBJ_NUMANODE);
		if (n)
		{
			void *m;
			int size = 1024*1024;
			hwloc_obj_t obj = hwloc_get_obj_by_type(g_topology, HWLOC_OBJ_NUMANODE, n - 1);
			m = hwloc_alloc_membind_nodeset(g_topology, size, obj->nodeset, HWLOC_MEMBIND_BIND, 0);
			hwloc_free(g_topology, m, size);
			m = malloc(size);
			hwloc_set_area_membind_nodeset(g_topology, m, size, obj->nodeset, HWLOC_MEMBIND_BIND, 0);
			free(m);
		}
	}
	else
	{
		if (g_enable_logs)
		{
			std::clog << "You can't call the 'example6' method if you have not call the 'init' method before, "
			          << "nothing will be done." << std::endl;
		}
	}
#else
	if (g_enable_logs)
	{
		std::clog << "'example6' method do nothing as AFF3CT has not been linked with the 'hwloc' library." << std::endl;
	}
#endif
}
