/*
 * tally.cpp
 *
 *  Created on: Mar 13, 2012
 *      Author: William Boyd
 *				MIT, Course 22
 *              wboyd@mit.edu
 */

#include "Tally.h"


/**
 * Default Tally constructor
 */
Tally::Tally() {

	_tally_name = (char*)"";

	 /* Sets the default delta between bins to zero */
	_bin_delta = 0;

	/* Sets the default for batch statistics */
	_num_batches = 0;
	_computed_statistics = false;

}


/**
 * Tally destructor deletes memory for tallies, number of tallies,
 * bin centers and bin edges if they have been created
 */
Tally::~Tally() {

	if (_num_bins != 0) {
		delete [] _tallies;
		delete [] _num_tallies;
		delete [] _centers;
		if (_bin_spacing != OTHER)
			delete [] _edges;
	}

	if (_num_batches != 0) {
		delete [] _batch_mu;
		delete [] _batch_variance;
		delete [] _batch_std_dev;
		delete [] _batch_rel_err;
	}
}


/**
 * Returns the of this Tally as specified by the user
 * @return the Tally's name
 */
char* Tally::getTallyName() {
	return _tally_name;
}


/**
 * Returns the number of bins
 * @return the number of bins
 */
int Tally::getNumBins() {
	return _num_bins;
}


/**
 * Returns a double array of bin edge values
 * @return array of bin edge values
 */
float* Tally::getBinEdges() {
	 if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return bin edges for Tally %s since "
				 "the bins have not yet been created", _tally_name);

	 return _edges;
}


/**
 * Returns a double array of bin center values
 * @return array of bin center values
 */
double* Tally::getBinCenters() {
	 if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return bin centers for Tally %s since the "
				 "bins have not yet been created", _tally_name);

	 return _centers;
}


/**
 * Returns the delta spacing between bins. NOTE: this value is only non-zero
 * for EQUAL and LOGARITHMIC bin types
 * @return the spacing between bins
 */
float Tally::getBinDelta() {
	return _bin_delta;
}


float Tally::getBinDelta(float sample) {

	/* If this Tally uses equally spaced bins in linear or logarithmic
	 * space, return the bin delta */
	if (_bin_spacing == EQUAL || _bin_spacing == LOGARITHMIC)
		return _bin_delta;

	/* If instead this Tally uses irregularly spaced bin edges defined
	 * by a user, compute bin delta of the bin around the sample */
	else {
		int bin_index = getBinIndex(sample);
		return (_edges[bin_index] - _edges[bin_index-1]);
	}
}


/**
 * Returns the bin spacing type (EQUAL, LOGARITHMIC, OTHER)
 * @return the bin spacing type
 */
binSpacingType Tally::getBinSpacingType() {
	return _bin_spacing;
}


/**
 * Returns the type of tally for these bins (ISOTOPE, MATERIAL, REGION)
 * @return the tally type
 */
tallyDomainType Tally::getTallyDomainType() {
	return _tally_domain;
}

/**
 * Returns the type of tally for these bins (FLUX, COLLISION_RATE, ABSORPTION_RATE, etc)
 * @return the tally type
 */
tallyType Tally::getTallyType() {
	return _tally_type;
}


/**
 * Returns a double array of the tallies within each bin
 * @return an array of
 */
double** Tally::getTallies() {
	 if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return tallies for Tally %s since the "
				 "bins have not yet been created", _tally_name);

	 return _tallies;
}


/**
 * Returns a specific tally for a specific bin
 * @param bin_index the index for the bin of interest
 * @return the tally within that bin
 */
double Tally::getTally(int batch_num, int bin_index) {

	if (bin_index < 0 || bin_index >= _num_bins)
		log_printf(ERROR, "Tried to get a tally for a bin index for Tally %s"
				"which does not exist: %d, num_bins = %d", _tally_name, bin_index,
				_num_bins);
	if (batch_num < 0 || batch_num >= _num_batches)
		log_printf(ERROR, "Tried to get a tally for a batch for Tally %s"
				"which does not exist: %d, num_batches = %d", _tally_name, batch_num,
				_num_batches);

	return _tallies[batch_num][bin_index];
}


/**
 * Returns an int array of the number of times tallied within each bin
 * @return an array of the number of tallies in each bin
 */
int** Tally::getNumTallies() {
	 if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return tally numbers for Tally %s since "
				 "the bins have not yet been created", _tally_name);

	return _num_tallies;
}


/**
 * Returns the number of times tallied within a specific bin
 * @param bin_index the bin of interest
 * @return the number of tallies in that bin
 */
int Tally::getNumTallies(int batch_num, int bin_index) {

	if (bin_index < 0 || bin_index >= _num_bins)
		log_printf(ERROR, "Tried to get a tally number for Tally %s for "
				"a bin index which does not exist: %d", _tally_name, bin_index);
	if (batch_num < 0 || batch_num >= _num_batches)
		log_printf(ERROR, "Tried to get a tally number for Tally %s for "
				"a batch which does not exist: %d", _tally_name, batch_num);

	return _num_tallies[batch_num][bin_index];
}


/**
 * Returns the maximum tally value over all bins
 * @return the maximum tally value
 */
double Tally::getMaxTally() {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return the maximum tally for Tally %s"
				 "since the bins have not yet been created", _tally_name);

	double max_tally = 0;

	/* Loop over all bins */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++) {
			if (_tallies[i][j] > max_tally)
				max_tally = _tallies[i][j];
		}
	}

	return max_tally;
}


/**
 * Returns the maximum tally value over all bins
 * @return the maximum tally value
 */
double Tally::getMinTally() {
	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return the minimum tally for Tally %s"
				 " since the bins have not yet been created", _tally_name);

	double min_tally = std::numeric_limits<double>::infinity();

	/* Loop over all bins */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++) {
			if (_tallies[i][j] < min_tally)
				min_tally = _tallies[i][j];
		}
	}

	return min_tally;
}


/**
 * Finds the bin index for a sample in a set of bins. If the samples
 * is outside the bounds of all bins, it returns infinity
 * @param sample the sample value of interest
 * @return the bin index for the sample
 */
int Tally::getBinIndex(float sample) {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot return a bin index for Tally %s since "
				 "the bins have not yet been created", _tally_name);

	/* Set index to infinity to begin with */
	int index = std::numeric_limits<float>::infinity();

	/* if the sample is equal to the last bin edge, return the last bin */
	if (sample == _edges[_num_bins])
		return _num_bins-1;

	/* Equally spaced bins */
	if (_bin_spacing == EQUAL)
		index = int((sample - _edges[0]) / _bin_delta);

	/* Logarithmically spaced bins */
	else if (_bin_spacing == LOGARITHMIC)
		index = int((log10(sample) - log10(_edges[0])) / _bin_delta);

	/* If the bin_type == OTHER then the bin edges were not generated by
	 * generateEqualBinEdges, so use a brute force search to find the bin */
	else {

		/* Loop over all bin edges to find the correct bin index */
		for (int i=0; i <= _num_bins; i++) {
			if (sample >= _edges[i] && sample < _edges[i+1]) {
				index = i;
				break;
			}
		}
	}

	/* If this sample was not contained within a bin set index to infinity*/
	if (index > _num_bins)
		index = std::numeric_limits<float>::infinity();

	return index;
}


/**
 * Returns the number of batches for this tally
 * @return the number of batches
 */
int Tally::getNumBatches() {
	return _num_batches;
}


/**
 * Returns a pointer to an array of batch averages if they have been
 * computed
 * @return a double array of batch averages for each bin
 */
double* Tally::getBatchMu() {

	if (!_computed_statistics)
		log_printf(ERROR, "Statistics have not yet been computed for "
				"Tally %s so batch mu cannot be returned", _tally_name);

	return _batch_mu;
}


/**
 * Returns a pointer to an array of batch variances if they have been
 * computed
 * @return a double array of batch variances for each bin
 */
double* Tally::getBatchVariance() {

	if (!_computed_statistics)
		log_printf(ERROR, "Statistics have not yet been computed for "
				"Tally %s so batch variance cannot be returned", _tally_name);

	return _batch_variance;
}


/**
 * Returns a pointer to an array of batch standard deviations if they have
 * been computed
 * @return a double array of batch standard deviations for each bin
 */
double* Tally::getBatchStdDev() {

	if (!_computed_statistics)
		log_printf(ERROR, "Statistics have not yet been computed for "
				"Tally %s so batch std dev cannot be returned", _tally_name);

	return _batch_std_dev;
}


/**
 * Returns a pointer to an array of batch relative errors if they have been
 * computed
 * @return a double array of batch relative errors for each bin
 */
double* Tally::getBatchRelativeError() {

	if (!_computed_statistics)
		log_printf(ERROR, "Statistics have not yet been computed for "
		"Tally %s so batch relative error cannot be returned", _tally_name);

	return _batch_rel_err;
}


/**
 * Sets this Tally's name
 * @param name the name of the Tally
 */
void Tally::setTallyName(char* name) {
	_tally_name = name;
}


/**
 * Set the bin spacing type for this Tally (EQUAL, LOGARITHMIC, OTHER)
 * @param type the bin spacing type
 */
void Tally::setBinSpacingType(binSpacingType type) {
    _bin_spacing = type;
}


/**
 * Set the domain type for this Tally (ISOTOPE, MATERIAL, REGION)
 * @param type the tally type
 */
void Tally::setTallyDomainType(tallyDomainType type) {
	_tally_domain = type;
}


/**
 * Set the type of tally for this Tally (FLUX, COLLISION_RATE, ABSORPTION_RATE)
 * @param type the tally type
 */
void Tally::setTallyType(tallyType type) {
	_tally_type = type;
}


/**
 * Set a user-defined double array of bin edge values
 * @param edges the array of bin edges
 * @param num_bins the number of bins
 */
void Tally::setBinEdges(float* edges, int num_bins) {

	_num_bins = num_bins;
	_edges = edges;
	_bin_spacing = OTHER;

	/* Set all tallies to zero by default */
	_tallies = (double**) malloc(sizeof(double*) * _num_batches);
	_num_tallies = (int**) malloc(sizeof(int*) * _num_batches);
	for (int i=0; i < _num_batches; i++) {
		_tallies[i] = new double[_num_bins];
		_num_tallies[i] = new int[_num_bins];
	}

	/* Loop over tallies and set to zero */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++) {
			_tallies[i][j] = 0.0;
			_num_tallies[i][j] = 0;
		}
	}

	/* Create an array of the center values between bins */
	generateBinCenters();

	/* Allocate memory for batch-based statistical counters */
	_batch_mu = new double[_num_bins];
	_batch_variance = new double[_num_bins];
	_batch_std_dev = new double[_num_bins];
	_batch_rel_err = new double[_num_bins];

}


/**
 * Set the number of batches for this tally
 * @param num_batches the number of batches
 */
void Tally::setNumBatches(int num_batches) {
	_num_batches = num_batches;
}


/**
 * This method clones a given Tally class object by executing a deep
 * copy of all of the Tally's class attributes and giving them to a new
 * Tally class object
 * @return a pointer to the new cloned Tally class object
 */
Tally* Tally::clone() {

	/* Allocate memory for the clone */
	Tally* new_clone = new Tally();

    new_clone->setTallyName(_tally_name);
    new_clone->setBinSpacingType(_bin_spacing);
    new_clone->setTallyDomainType(_tally_domain);
    new_clone->setTallyType(_tally_type);
    new_clone->setNumBatches(_num_batches);

    /* If the bins are regularly spaced, re-generate them for the new Tally */
    if (_bin_spacing == EQUAL || _bin_spacing == LOGARITHMIC)
        new_clone->generateBinEdges(_edges[0], _edges[_num_bins], 
                                    _num_bins, _bin_spacing);
    /* If the bins are not regularly spaced, deep copy them to the new Tally */
    else {
        float* edges = new float[_num_bins+1];
        memcpy(edges, _edges, _num_bins * sizeof(float));
        new_clone->setBinEdges(edges, _num_bins);
    }

	/* Return a pointer to the cloned Isotope class */
	return new_clone;
}


/**
 * Generate edges between bins defined by a start and end point
 * @param start first bin edge value
 * @param end last bin edge value
 * @param num_bins the number of bins to be created
 * @param type the type of bins (EQUAL or LOGARITHMIC)
 */
void Tally::generateBinEdges(float start, float end, int num_bins,
												binSpacingType type) {
	if (start == end)
		log_printf(ERROR, "Unable to create bins for Tally %s between"
				"the same start and end points: %f", _tally_name, start);

	_num_bins = num_bins;
	_bin_spacing = type;

	/* Allocate memory for tallies */
	_tallies = (double**) malloc(sizeof(double*) * _num_batches);
	_num_tallies = (int**) malloc(sizeof(int*) * _num_batches);
	for (int i=0; i < _num_batches; i++) {
		_tallies[i] = new double[_num_bins];
		_num_tallies[i] = new int[_num_bins];
	}


	/* Set all tallies to zero by default */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++) {
			_tallies[i][j] = 0;
			_num_tallies[i][j] = 0;
		}
	}

	/* Equal spacing between bins */
	if (type == EQUAL) {
		_bin_delta = float(end - start) / float(_num_bins);

		/* Generate points from start to end for each bin edge */
		_edges = linspace<float, float>(start, end, num_bins+1);
	}

	/* Logarithmically equal spacing between bins */
	else if (type == LOGARITHMIC) {
		_bin_delta = float(log10(end) - log10(start)) / float(_num_bins);

		/* Generate points from start to end for each bin edge */
		_edges = logspace<float, float>(start, end, num_bins+1);
	}

	else
		log_printf(ERROR, "Bin type %d is not yet implemented for Tally %s",
															_tally_name, type);

	/* Create an array of the center values between bins */
	generateBinCenters();

	/* Allocate memory for batch-based statistical counters */
	_batch_mu = new double[_num_bins];
	_batch_variance = new double[_num_bins];
	_batch_std_dev = new double[_num_bins];
	_batch_rel_err = new double[_num_bins];


	return;
}


/**
 * Compute the center points between bin edges for this Tally's bins
 */
void Tally::generateBinCenters() {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot generate bin centers for Tally %s since "
				 "the bins have not yet been created", _tally_name);

	/* Allocate memory for the bin centers array */
	_centers = new double[_num_bins];

	/* Loop over all bins and find the midpoint between edges */
	for (int i=0; i < _num_bins; i++)
		_centers[i] = (_edges[i] + _edges[i+1]) / 2.0;

	return;
}


/**
 * Tallies unity for each sample in a double array of samples
 * @param samples array of samples to tally
 * @param num_samples the number of samples to tally
 * @param batch_num the batch number for this sample
 */
void Tally::tally(float* samples, int num_samples, int batch_num) {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot tally samples in Tally %s since the "
				 "bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot tally samples in Tally %s since "
				 "batches have not yet been created", _tally_name);

	int bin_index;

	/* Loop over and tally all samples */
	for (int i=0; i < num_samples; i++) {
		bin_index = getBinIndex(samples[i]);
		if (bin_index >= 0 && bin_index < _num_bins) {
			_tallies[batch_num][bin_index]++;
			_num_tallies[batch_num][bin_index]++;
		}
	}

	return;
}


/**
 * Tallies unity for a sample
 * @param samples array of samples to tally
 * @param batch_num the batch number for this sample
 */
void Tally::tally(float sample, int batch_num) {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot tally sample in Tally %s since "
				 "the bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot tally samples in Tally %s since "
				 "batches have not yet been created", _tally_name);

	int bin_index = getBinIndex(sample);

	if (bin_index >= 0 && bin_index < _num_bins) {
		_tallies[batch_num][bin_index]++;
		_num_tallies[batch_num][bin_index]++;
	}

	return;
}


/**
 * Tallies a weight for each sample in a double array of samples
 * @param samples array of samples to tally
 * @param sample_weights array of sample weights to increment tallies by
 * @param num_samples the number of samples to tally
 * @param batch_num the batch number for this sample
 */
void Tally::weightedTally(float* samples, float* sample_weights,
										int num_samples, int batch_num) {
	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot tally weighted samples in Tally %s "
				 "since the bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot tally samples in Tally %s since "
				 "batches have not yet been created", _tally_name);

	int bin_index;

	/* Loop over and tally all samples */
	for (int i=0; i < num_samples; i++) {
		bin_index = getBinIndex(samples[i]);
		if (bin_index >= 0 && bin_index < _num_bins) {
			_tallies[batch_num][bin_index] += sample_weights[i];
			_num_tallies[batch_num][bin_index]++;
		}
	}

	return;
}


/**
 * Tallies a weight for a sample
 * @param sample a sample to tally
 * @param weight the weight to increment tally by
 * @param batch_num the batch number for this sample
 */
void Tally::weightedTally(float sample, float weight, int batch_num) {

	if (_num_bins == 0)
		 log_printf(ERROR, "Cannot tally weighted sample in Tally %s since "
				 "the bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot tally samples in Tally %s since "
				 "batches have not yet been created", _tally_name);

	int bin_index = getBinIndex(sample);

	if (bin_index >= 0 && bin_index < _num_bins) {
		_tallies[batch_num][bin_index] += double(weight);
		_num_tallies[batch_num][bin_index]++;
	}

	return;
}


/**
 * Divide each tally by the maximum tally value
 */
void Tally::normalizeTallies() {

	if (_num_bins == 0)
		log_printf(ERROR, "Cannot normalize tallies for Tally %s since it is"
						"the bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot normalize tallies for Tally %s since "
				 "batches have not yet been created", _tally_name);

	double max_tally = getMaxTally();

	/* Divide each tally by maximum tally value */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++)
			_tallies[i][j] /= max_tally;
	}

	return;
}


/**
 * Divide each tally by a given scaling factor
 * @param scale_factor factor to normalize tallies by
 */
void Tally::normalizeTallies(float scale_factor) {

	if (_num_bins == 0)
		log_printf(ERROR, "Cannot normalize tallies for Tally %s since it is"
						"the bins have not yet been created", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot normalize tallies for Tally %s since "
				 "batches have not yet been created", _tally_name);

	/* Divide each tally by maximum tally value */
	for (int i=0; i < _num_batches; i++) {
		for (int j=0; j < _num_bins; j++)
		_tallies[i][j] /= double(scale_factor);
	}

	return;
}


/**
 * Computes average, variance, standard deviation and relative error for each
 * bin over the set of batches
 */
void Tally::computeBatchStatistics() {

	if (_num_batches == 0)
		log_printf(ERROR, "Cannot compute batch statistics for Tally %s"
				" since it has  have not yet been generated", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot compute batch statistics for Tally %s since"
				 " batches have not yet been created", _tally_name);

	/* Loop over each bin */
	for (int i=0; i < _num_bins; i++) {

		/* Initialize statistics to zero */
		_batch_mu[i] = 0.0;
		_batch_variance[i] = 0.0;
		_batch_std_dev[i] = 0.0;
		_batch_rel_err[i] = 0.0;

		/* Accumulate flux from each batch */
		for (int j=0; j < _num_batches; j++)
			_batch_mu[i] += _tallies[j][i];

		/* Compute average flux for this bin */
		_batch_mu[i] /= double(_num_batches);

		/* Compute the variance for this bin */
		for (int j=0; j < _num_batches; j++) {
			_batch_variance[i] += (_tallies[j][i] - _batch_mu[i])
					* (_tallies[j][i] - _batch_mu[i]);
		}
		_batch_variance[i] /= double(_num_batches);

		/* Compute the standard deviation for this bin */
		_batch_std_dev[i] = sqrt(_batch_variance[i]);

		/* Compute the relative error for this bin */
		_batch_rel_err[i] = _batch_std_dev[i] / _batch_mu[i];
	}

	_computed_statistics = true;

	return;
}


/**
 * Computes average, variance, standard deviation and relative error for each
 * bin over the set of batches. This method first scales each bin value by
 * a scaling factor
 * @param scale_factor the factor to scale each bin value by
 */
void Tally::computeScaledBatchStatistics(float scale_factor) {

	if (_num_batches == 0)
		log_printf(ERROR, "Cannot compute batch statistics for BatchBinSet %s "
				"since the binners have not yet been generated", _tally_name);
	if (_num_batches == 0)
		 log_printf(ERROR, "Cannot compute batch statistics for Tally %s since"
				 " batches have not yet been created", _tally_name);

	/* Loop over each bin */
	for (int i=0; i < _num_bins; i++) {

		/* Initialize statistics to zero */
		_batch_mu[i] = 0.0;
		_batch_variance[i] = 0.0;
		_batch_std_dev[i] = 0.0;
		_batch_rel_err[i] = 0.0;

		/* Accumulate flux from each batch */
		for (int j=0; j < _num_batches; j++)
			_batch_mu[i] += _tallies[j][i] / double(scale_factor);

		/* Compute average flux for this bin */
		_batch_mu[i] /= double(_num_batches);

		/* Compute the variance for this bin */
		for (int j=0; j < _num_batches; j++) {
			_batch_variance[i] += (_tallies[j][i] / scale_factor
			- _batch_mu[i]) * (_tallies[j][i] / scale_factor
												- _batch_mu[i]);
		}
		_batch_variance[i] /= double(_num_batches);

		/* Compute the standard deviation for this bin */
		_batch_std_dev[i] = sqrt(_batch_variance[i]);

		/* Compute the relative error for this bin */
		_batch_rel_err[i] = _batch_std_dev[i] / _batch_mu[i];
	}

	_computed_statistics = true;

	return;
}


/**
 * Outputs the batch statistics (if they have been computed) to an
 * ASCII file
 * @param filename the output filename
 */
void Tally::outputBatchStatistics(const char* filename) {

	if (_num_batches == 0)
		log_printf(ERROR, "Cannot output batch statistics for Tally %s "
				"since the batches have not yet been generated", _tally_name);

	if (!_computed_statistics)
		log_printf(ERROR, "Cannot output batch statistics for Tally %s "
				"since statistics have not yet been computed", _tally_name);

	/* Create output file */
	FILE* output_file;
	output_file = fopen(filename, "w");

	/* Print header to output file */
	fprintf(output_file, "Batch-based tally statistics for PINSPEC\n");

	if (_tally_type == COLLISION_RATE)
		fprintf(output_file, "Tally type: COLLISION_RATE Rate\n");
	else if (_tally_type == FLUX)
		fprintf(output_file, "Tally type: Flux\n");
	else if (_tally_type == ELASTIC_RATE)
		fprintf(output_file, "Tally type: ELASTIC_RATE Scattering Reaction Rate\n");
	else if (_tally_type == ABSORPTION_RATE)
		fprintf(output_file, "Tally type: ABSORPTION_RATE Reaction Rate\n");
	else if (_tally_type == CAPTURE_RATE)
		fprintf(output_file, "Tally type: CAPTURE_RATE Reaction Rate\n");
	else if (_tally_type == FISSION_RATE)
		fprintf(output_file, "Tally type: FISSION_RATE Reaction Rate\n");
	else if (_tally_type == TRANSPORT_RATE)
		fprintf(output_file, "Tally type: TRANSPORT_RATE Reaction Rate\n");
	else if (_tally_type == DIFFUSION_RATE)
		fprintf(output_file, "Tally type: DIFFUSION_RATE Reaction Rate\n");
	else if (_tally_type == LEAKAGE_RATE)
		fprintf(output_file, "Tally type: LEAKAGE_RATE Rate\n");

	if (_tally_domain == ISOTOPE)
		fprintf(output_file, "Tally Domain: Isotope\n");
	else if (_tally_domain == MATERIAL)
		fprintf(output_file, "Tally Domain: Material\n");
	else if (_tally_domain == REGION)
		fprintf(output_file, "Tally Domain: Region\n");

	if (_bin_spacing == EQUAL)
		fprintf(output_file, "Equally spaced bins with width = %d\n", _bin_spacing);
	else if (_bin_spacing == LOGARITHMIC)
		fprintf(output_file, "Logarithmically spaced bins\n");
	else if (_bin_spacing == OTHER)
		fprintf(output_file, "User-defined bins\n");

	fprintf(output_file, "# batches: %d\t, # bins: %d\n", 
									_num_batches, _num_bins);
	fprintf(output_file, "Bin center, Mu, Variance, Std Dev, Rel Err\n");

	/* Loop over each bin and print mu, var, std dev and rel err */
	for (int i=0; i < _num_bins; i++) {
		fprintf(output_file, "%1.10f, %1.10f, %1.10f, %1.10f, %1.10f\n",
				_centers[i], _batch_mu[i], _batch_variance[i], 
						_batch_std_dev[i], _batch_rel_err[i]);
	}

	fclose(output_file);

	return;
}
