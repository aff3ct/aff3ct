#ifndef HISTOGRAM_H__
#define HISTOGRAM_H__

#include <cmath>
#include <fstream>
#include <vector>

template <typename R>
void histogram(std::ofstream& hist_file, const std::vector<R>& draw, const R hist_min, const R hist_max, const R n_intervals = 100, const bool include_borders = false)
{
	std::vector<R> hist(n_intervals+1, 0);

	unsigned n_over_range = 0;

	R norm = (R)n_intervals / (hist_max - hist_min);
	R _norm = (R)1/norm;


	for (auto d : draw)
	{
		if (d < hist_min || d > hist_max)
		{
			n_over_range++;

			if (include_borders)
				d = (d < hist_min) ? hist_min : hist_max;
			else
				continue;
		}

		auto x = round((d - hist_min) * norm);
		hist[x]++;
	}


	for (unsigned i = 0; i < hist.size(); ++i)
		hist_file << ((R)i * _norm + hist_min) << "; " << hist[i]/(R)draw.size() << std::endl;
}


#endif // HISTOGRAM_H__