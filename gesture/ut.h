#ifndef _ut_H
#define _ut_H


#include <iostream>
#include <list>
#include <vector>
//#include "TSdist.h"
#include "ts.h"
#include "dtw.h"
using namespace std;

// Univariate time series class
class UnivariateTimeSeries : public TSdist::TimeSeriesBase
{
public:

	UnivariateTimeSeries(std::vector<double> singleSeries) :
		series(std::move(singleSeries))
	{
		if (series.empty()) throw("Series cannot be empty.");
	}

	int length() const override {
		return series.size();
	}

	int numVars() const override {
		return 1;
	}

	const double& indexSeries(int time_index, int var_index) const override {
		return series.at(time_index);
	}

	double& indexSeries(int time_index, int var_index) override {
		return series.at(time_index);
	}

	std::vector<double>::iterator begin() { return series.begin(); }
	std::vector<double>::iterator end() { return series.end(); }
	std::vector<double>::const_iterator begin() const { return series.begin(); }
	std::vector<double>::const_iterator end() const { return series.end(); }

private:
	std::vector<double> series;
};






#endif // _ut_H
