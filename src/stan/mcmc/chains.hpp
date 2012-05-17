#ifndef __STAN__MCMC__CHAINS_HPP__
#define __STAN__MCMC__CHAINS_HPP__

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <Eigen/Dense>

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/additive_combine.hpp>

namespace stan {  

  namespace mcmc {


    /**
     * Validate the specified indexes with respect to the
     * specified dimensions.
     *
     * @param dims Dimensions of array.
     * @param idxs Indexes into array.
     * @throw std::invalid_argument If the two arrays are different
     * sizes.
     * @throw std::out_of_range If any of the indexes is greater than
     * or equal to its correpsonding dimension.
     */
    void validate_dims_idxs(const std::vector<size_t>& dims,
                            const std::vector<size_t>& idxs) {
      if (idxs.size() != dims.size()) {
        std::stringstream msg;
        msg << "index vector and dims vector must be same size."
            << "; idxs.size()=" << idxs.size()
            << "; dims.size()=" << dims.size();
        throw std::invalid_argument(msg.str());
      }
      for (size_t i = 0; i < idxs.size(); ++i) {
        if (idxs[i] >= dims[i]) {
          std::stringstream msg;
          msg << "indexes must be within bounds."
              << "; idxs[" << i << "]=" << idxs[i]
              << "; dims[" << i << "]=" << dims[i];
          throw std::out_of_range(msg.str());
        }
      }
    }

    /**
     * Return the offset in last-index major indexing for the
     * specified indexes given the specified number of dimensions.
     * If both sequences are empty, the index returned is 0.
     *
     * @param dims Sequence of dimensions.
     * @param idxs Sequence of inndexes.
     * @return Offset of indexes given dimensions.
     * @throw std::invalid_argument If the sizes of the index
     * and dimension sequences is different.
     * @throw std::out_of_range If one of the indexes is greater
     * than or equal to the corresponding index.
     */
    size_t get_offset(const std::vector<size_t>& dims, 
                      const std::vector<size_t>& idxs) {
      validate_dims_idxs(dims,idxs);
      if (idxs.size() == 0)
        return 0;
      if (idxs.size() == 1)
        return idxs[0];
      size_t pos(0);
      // OK, stop at 1
      for (size_t i = idxs.size(); --i != 0; ) {
        pos += idxs[i];
        pos  *= dims[i-1];
      }
      return pos + idxs[0];
    }

    /**
     * Increments the specified indexes to refer to the next value
     * in an array given by the specified dimensions.  The indexing
     * is in last-index major order, which is column-major for
     * matrices.
     *
     * <p>The first index in the sequence is all zeroes.
     * Incrementing the last index, whose values are the dimensions
     * minus one, returns the all-zero matrix.
     *
     * <p>Given <code>dims == (2,2,2)</code>, the sequence of
     * indexes are 
     *
     * <code>[0 0 0]</code>, 
     * <code>[1 0 0]</code>, 
     * <code>[0 1 0]</code>, 
     * <code>[1 1 0]</code>, 
     * <code>[0 0 1]</code>, 
     * <code>[1 0 1]</code>, 
     * <code>[0 1 1]</code>, 
     * <code>[1 1 1]</code>,
     * <code>[0 0 0]</code>, 
     * <code>[1 0 0]</code>, ...
     *
     * @param dims Dimensions of array.
     * @param idxs Indexes into array.
     * @throws std::invalid_argument If the dimensions and indexes
     * are not the same size.
     * @throws std::out_of_range If an index is greater than or equal
     * to the corresponding dimension.
     */
    void
    increment_indexes(const std::vector<size_t>& dims,
                      std::vector<size_t>& idxs) {
      validate_dims_idxs(dims,idxs);
      for (size_t i = 0; i < dims.size(); ++i) {
        ++idxs[i];
        if (idxs[i] < dims[i]) 
          return;
        idxs[i] = 0;
      }
    }

    /**
     * Write a permutation into the specified vector of the specified
     * size using the specified Boost random number generator.  The
     * vector will be resized to the specified size.
     *
     * @tparam RNG Type of random number geneation engine
     * @param x Vector into which to write the permutation
     * @param n Size of permutation to create
     * @param rng Random-number generator.
     */
    template <class RNG>
    void permutation(std::vector<size_t>& x,
                     size_t n,
                     RNG& rng) {
      x.resize(n);
      for (size_t i = 0; i < n; ++i)
        x[i] = i;
      if (x.size() < 2) return;
      for (int i = x.size(); --i != 0; ) {
        boost::random::uniform_int_distribution<size_t> uid(0,i);
        size_t j = uid(rng);
        size_t temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
    

    /**
     * Write the specified permutation of the first vector into
     * the second vector.  The second vector will be resized to
     * the size of the permutation.
     *
     * @tparam T Type of elements to permute
     * @param pi Permutation.
     * @param x_from Vector of elements to permute
     * @param x_to Vector into which permutation of elements is written
     * @throw std::invalid_argument If the permutation vector and
     * source vector from which to copy are not the same size.
     */
    template <typename T>
    void permute(const std::vector<size_t>& pi,
                 const std::vector<T>& x_from,
                 std::vector<T>& x_to) {
      size_t N = pi.size();
      if (N != x_from.size()) {
        std::stringstream msg;
        msg << "Require permutation to be same size as source vector."
            << "; found pi.size()=" << pi.size()
            << "; x_from.size()=" << x_from.size();
      }
      x_to.resize(N);
      for (size_t i = 0; i < N; ++i)
        x_to[i] = x_from[pi[i]];
    }

    
    /**
     * An <code>mcmc::chains</code> object stores parameter names and
     * dimensionalities along with samples from multiple chains.
     *
     * <p><b>Synchronization</b>: For arbitrary concurrent use, the
     * read and write methods need to be read/write locked.  Multiple
     * writers can be used concurrently if they write to different
     * chains.  Readers for single chains need only be read/write locked
     * with writers of that chain.  For reading across chains, full
     * read/write locking is required.  Thus methods will be classified
     * as global or single-chain read or write methods.
     *
     * <p><b>Storage Order</b>: Storage is column/last-index major.
     */
    template <typename RNG = boost::random::ecuyer1988>
    class chains {
    private:

      size_t _warmup;
      const std::vector<std::string> _names;
      const std::vector<std::vector<size_t> > _dimss;
      const size_t _num_params; // total
      const std::vector<size_t> _starts;
      const std::map<std::string,size_t> _name_to_index;
      // [chain,param,sample]
      std::vector<std::vector<std::vector<double > > > _samples; 
      std::vector<size_t> _permutation;
      RNG _rng; // defaults to time-based init

      static size_t calc_num_params(const std::vector<size_t>& dims) {
        size_t num_params = 1;
        for (size_t i = 0;  i < dims.size(); ++i)
          num_params *= dims[i];
        return num_params;
      }

      static size_t 
      calc_total_num_params(const std::vector<std::vector<size_t> >& dimss) {
        int num_params = 0;
        for (size_t i = 0; i < dimss.size(); ++i)
          num_params += calc_num_params(dimss[i]);
        return num_params;
      }

      static std::vector<size_t> 
      calc_starts(const std::vector<std::vector<size_t> >& dimss) {
        std::vector<size_t> starts(dimss.size());
        starts[0] = 0;
        for (size_t i = 1; i < dimss.size(); ++i)
          starts[i] = starts[i - 1] + calc_num_params(dimss[i - 1]);
        return starts;
      }

      static std::map<std::string,size_t>
      calc_name_to_index(const std::vector<std::string> names) {
        std::map<std::string,size_t> name_to_index;
        for (size_t i = 0; i < names.size(); ++i)
          name_to_index[names[i]] = i;
        return name_to_index;
      }

      inline void validate_param_name_idx(size_t j) {
        if (j < num_param_names()) 
          return;
        std::stringstream msg;
        msg << "parameter name index must be less than number of params"
            << "; found j=" << j;
        throw std::out_of_range(msg.str());
      }

      inline void validate_param_idx(size_t n) {
        if (n < num_params())
          return;
        std::stringstream msg;
        msg << "parameter index must be less than number of params"
            << "; found n=" << n;
        throw std::out_of_range(msg.str());
      }

      inline void validate_chain_idx(size_t k) {
        if (k >= num_chains()) {
          std::stringstream msg;
          msg << "chain must be less than number of chains."
              << "; num chains=" << num_chains()
              << "; chain=" << k;
          throw std::out_of_range(msg.str());
        }
      }

      // k = chain idx, m = iteration
      void validate_iteration(size_t k,    
                              size_t m) {  
        validate_chain_idx(k);
        if (m >= _samples[k][0].size()) {
          std::stringstream msg;
          msg << "require sample index below number of samples"
              << "; sample index m=" << m
              << "; chain index k=" << k
              << "; num samples in chain" << k << "=" 
              << _samples[k][0].size();
          throw std::out_of_range(msg.str());
        }
      }

      void resize_permutation(size_t K) {
        if (_permutation.size() != K) 
          permutation(_permutation,K,_rng);
      }

    public:

      /**
       * Construct a chains object with the specified number of Markov
       * chains, and the specified parameter names and matching
       * parameter dimensions.
       * 
       * <p>The order of the parameter names and dimesnions should match
       * the order in which samples are added to the constructed
       * object.  
       *
       * <p>The total number of parameters is determined by adding the
       * parameters for each name.  The number of parameters for each
       * name is determined by multiplying its dimensionalities.  For
       * example, a 2 x 3 x 4 matrix parameter produces of 24 total
       * parameters.
       *
       * @param num_chains Number of Markov chains.
       * @param names Sequence of paramter names.
       * @param dimss Sequence of parameter dimensionalities.
       * @throws std::invalid_argument If the name and dimensions
       * sequences are not the same size.
       */
      chains(size_t num_chains,
             const std::vector<std::string>& names,
             const std::vector<std::vector<size_t> >& dimss) 
        : _warmup(0),
          _names(names),
          _dimss(dimss),
          _num_params(calc_total_num_params(dimss)),
          _starts(calc_starts(dimss)),               // copy
          _name_to_index(calc_name_to_index(names)), // copy
          _samples(num_chains,std::vector<std::vector<double> >(_num_params))
      {
        if (names.size() != dimss.size()) {
          std::stringstream msg;
          msg << "names and dimss mismatch in size"
              << " names.size()=" << names.size()
              << " dimss.size()=" << dimss.size();
          throw std::invalid_argument(msg.str());
        }
      }
      
      
      /**
       * Return the number of chains.
       *
       * <p><b>Synchronization</b>: Thread safe.
       * 
       * @return The number of chains.
       */
      inline size_t num_chains() {
        return _samples.size();
      }

      /**
       * Return the total number of parameters.  
       *
       * <p>This is not the number of parameter names, but the total
       * number of scalar parameters.
       *
       * <p><b>Synchronization</b>: Thread safe.
       * 
       * @return The total number of parameters.
       */
      inline size_t num_params() { 
        return _num_params;
      }

      /**
       * Return the total number of parameter names.
       *
       * <p><b>Synchronization</b>: Thread safe.
       * 
       * @return The total number of parameter names.
       */
      inline size_t num_param_names() {
        return _names.size();
      }

      /**
       * Return the sequence of parameter names.
       *
       * <p><b>Synchronization</b>: Thread safe after construction.
       * 
       * @return The sequence of parameter names.
       */
      const std::vector<std::string>& param_names() {
        return _names;
      }

      /**
       * Return the name of the parameter with the specified index.
       *
       * <p><b>Synchronization</b>: Thread safe.
       * 
       * @param j Index of parameter.
       * @return The parameter with the specified index.
       * @throw std::out_of_range If the parameter identifier is
       * greater than or equal to the number of parameters.
       */
      const std::string& param_name(size_t j) {
        validate_param_name_idx(j);
        return _names[j];
      }

      /**
       * Return the sequence of named parameter dimensions.  
       *
       * <p><b>Synchronization</b>: Thread safe after construction.
       *
       * @return The sequence of named parameter dimensions.
       */
      const std::vector<std::vector<size_t> >& param_dimss() {
        return _dimss;
      }

      /**
       * Return the dimensions of the parameter name with the
       * specified index.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @param j Index of a parameter name.
       * @return The dimensions of the parameter name with the specified
       * index.
       * @throw std::out_of_range If the index is greater than or equal
       * to the numberof parameter names.
       */
      const std::vector<size_t>& param_dims(size_t j) {
        validate_param_name_idx(j);
        return _dimss[j];
      }

      /**
       * Return the sequence of starting indexes for the named
       * parameters in the underlying sequence of scalar parameters.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @return The sequence of named parameter start indexes.
       */
      const std::vector<size_t>& param_starts() {
        return _starts;
      }

      /**
       * Return the starting position of the named parameter with the
       * specified index in the underlying sequence of scalar parameters.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @param j The parameter name index.
       * @return The start index of the specified parameter.
       * @throw std::out_of_range If the parameter name index is
       * greater than or equal to the number of named parameters.
       */
      size_t param_start(size_t j) {
        validate_param_name_idx(j);
        return _starts[j];
      }

      /**
       * Return a copy of the sequence of named parameter sizes.  The
       * size of a named parameter is the prouct of its dimensions.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @return The sequence of named parameter sizes.
       */
      const std::vector<size_t> param_sizes() {
        std::vector<size_t> s(num_param_names());
        for (unsigned int j = 0; j < num_param_names(); ++j)
          s[j] = param_size(j); // could optimize tests in param_sizes() out
        return s;
      }

      /**
       * Return the size of the named parameter with the specified index.
       * The size of a named parameter is the prouct of its dimensions.
       *
       * <p><b>Synchronization</b>: Thread safe after construction.
       *
       * @param j The index of a named parameter.
       * @return The size of the specified named parameter.
       * @throw std::out_of_range If the index is greater than or
       * equal to the number of named parameters.
       */
      size_t param_size(size_t j) {
        validate_param_name_idx(j);
        if (j + 1 < _starts.size()) 
          return _starts[j+1] - _starts[j];
        return num_params() - _starts[j];
      }

      /**
       * Return the named parameter index for the specified parameter
       * name.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @param name Parameter name.
       * @return Index of parameter name.
       * @throw std::out_of_range If the parameter is not one of the
       * named parameters.
       */
      size_t param_name_to_index(const std::string& name) {
        std::map<std::string,size_t>::const_iterator it
          = _name_to_index.find(name);
        if (it == _name_to_index.end()) {
          std::stringstream ss;
          ss << "unknown parameter name=" << name;
          throw std::out_of_range(ss.str());
        }
        return it->second;
      }
      

      /**
       * Return the index in the underlying sequence of scalar parameters
       * for the parameter with the specified name index and
       * indexes.
       *
       * <p><b>Synchronization</b>: Thread safe.
       *
       * @param j Index of parameter name.
       * @param idxs Indexes into parameter.
       * @return Offset into the underlying sequence of scalar paramters.
       * @throw std::out_of_range If the named parameter index is greater than
       * or equal to the number of named parameters or if any of the indexes
       * is out of range for the named parameter with the specified index.
       */ 
      size_t get_total_param_index(size_t j, // param id
                                   const std::vector<size_t>& idxs) {
        return get_offset(param_dims(j),idxs)
          + param_start(j);
      }


      /**
       * Set the warmup cutoff to the specified number of
       * iterations.  The first samples in each chain up to
       * this number will be treated as warmup samples.
       * 
       * <p><b>Synchronization</b>: Warmup write method. 
       *
       * @param warmup_iterations Number of warmup iterations.
       */
      void set_warmup(size_t warmup_iterations) {
        _warmup = warmup_iterations;
      }


      /**
       * Return the warmup iteration cutoff.
       * 
       * <p><b>Synchronization</b>: Warmup read method.
       *
       * @return Number of warmup iterations.
       */
      inline size_t warmup() {
        return _warmup;
      }


      /**
       * Add the specified sample to the end of the specified chain.
       *
       * <p><b>Synchronization:</b> Chain-specific write.
       *
       * @param chain Markov chain identifier.
       * @param theta Parameter values.
       * @throws std::invalid_argument if the size of the sample
       * vector does not match the number of parameters.
       */
      void add(size_t chain,
               std::vector<double> theta) {
        validate_chain_idx(chain);
        if (theta.size() != _num_params) {
          std::stringstream msg;
          msg << "parameter vector size must match num params"
              << "; num params=" << _num_params
              << "; theta.size()=" << theta.size();
          throw std::invalid_argument(msg.str());
        }
        for (size_t i = 0; i < theta.size(); ++i)
          _samples[chain][i].push_back(theta[i]); // _samples very non-local
      }
      
      



      /**
       * Return the number of warmup samples in the
       * specified chain.
       *
       * <p><b>Synchronization</b>:  Warmup and chain-specific read.
       *
       * @param k Chain index.
       * @return Number of warmup samples availabe in chain.
       */
      size_t num_warmup_samples(size_t k) {
        return std::min<size_t>(num_samples(k), warmup());
      }

      /**
       * Return the total number of warmup samples across chains.
       *
       * <p><b>Synchronization</b>:  Warmup and cross-chain read.
       *
       * @return The total number of warmup samples.
       */
      size_t num_warmup_samples() {
        size_t total = 0;
        for (size_t k = 0; k < num_chains(); ++k)
          total += num_warmup_samples(k);
        return total;
      }

      /**
       * Return the number of samples in the specified chain not
       * including warmup samples.
       *
       * <p><b>Synchronization</b>:  Warmup and chain-specific read.
       *
       * @param k Chain index.
       * @return Number of warmup samples availabe in chain.
       */
      size_t num_kept_samples(size_t k) {
        if (num_samples(k) > warmup())
          return num_samples(k) - warmup();
        return 0U;
      }


      /**
       * Return the total number of samples in all chains not
       * including warmup samples.
       *
       * <p><b>Synchronization</b>:  Warmup and cross-chain read.
       *
       * @return Total number of warmup samples
       */
      size_t num_kept_samples() {
        size_t total = 0;
        for (size_t k = 0; k < num_chains(); ++k)
          total += num_kept_samples(k);
        return total;
      }

      /**
       * Return the total number of samples across chains including
       * warmup and kept samples.
       *
       * <p><b>Synchronization</b>: Cross-chain read.
       *
       * @return Total number of samples.
       */
      size_t num_samples() {
        size_t M = 0;
        for (size_t k = 0; k < num_chains(); ++k)
          M += num_samples(k);
        return M;
      }

      /**
       * Return the number of samples including warmup and kept samples
       * in the specified chain.
       *
       * <p><b>Synchronization</b>: Chain-specific read.
       *
       * @param k Markov chain index.
       * @return Number of samples in the specified chain.
       * @throw std::out_of_range If the identifier is greater than
       * or equal to the number of chains.
       */
      size_t num_samples(size_t k) {
        validate_chain_idx(k);
        return _samples[k][0].size();
      }


      /**
       * Write into the specified vector the warmup and kept samples
       * for the scalar parameter with the specified index.  The order
       * of samples is by chain, then by order in which the sample was
       * added to the chain.
       *
       * <p><b>Synchronization</b>: Cross-chain read.
       * 
       * @param n Index of parameter.
       * @param samples Vector into which samples are written.
       * @throw std::out_of_range If the parameter index is greater
       * than or equal to the total number of scalar parameters.
       */
      void
      get_samples(size_t n,
                  std::vector<double>& samples) {
        validate_param_idx(n);
        samples.resize(0);
        samples.reserve(num_samples());
        for (size_t k = 0; k < num_chains(); ++k)
          samples.insert(samples.end(),
                         _samples[k][n].begin(), 
                         _samples[k][n].end());
      }

      /**
       * Write into the specified vector the warmup and kept samples
       * for the scalar parameter with the specified index in the
       * chain with the specified index.  The order of samples is the
       * order in which they were added.
       *
       * <p><b>Synchronization</b>: Chain-specific read.
       *
       * @param k Index of chain.
       * @param n Index of parameter.
       * @param samples Vector into which to write samples
       * @throw std::out_of_range If the specified chain index is greater
       * than or equal to the number of chains, or if the specified parameter
       * index is greater than or equal to the total number of parameters.
       */
      void get_samples(size_t k, 
                       size_t n, 
                       std::vector<double>& samples) {  
        validate_chain_idx(k);
        validate_param_idx(n); 
        samples.resize(0);
        samples.reserve(num_samples(k));
        samples.insert(samples.end(),
                       _samples[k][n].begin(),
                       _samples[k][n].end());
      }


      /**
       * Write into the specified vector the kept samples for the
       * scalar parameter with the specified index.  The order of
       * samples is permuted, but as long as no samples have been
       * added in the interim, subsequent calls to this method will
       * use the same permutation for all parameter indexes.
       *
       * <p><b>Synchronization</b>: Cross-chain read.
       * 
       * @param n Index of parameter.
       * @param samples Vector into which samples are written.
       * @throw std::out_of_range If the parameter index is greater
       * than or equal to the total number of scalar parameters.
       */
      void
      get_kept_samples_permuted(size_t n,
                                std::vector<double>& samples) {
        validate_param_idx(n);
        size_t M = num_kept_samples();
        samples.resize(M);
        resize_permutation(M);
        // const std::vector<size_t>& permutation = _permutation;
        size_t pos = 0;
        for (size_t k = 0; k < num_chains(); ++k) {
          // const std::vector<double>& samples_k_n = _samples[k][n];
          for (size_t m = warmup(); m < num_samples(k); ++m) {
            samples[_permutation[pos]] = _samples[k][n][m]; // _samples_k_n[m];
            ++pos;
          }
        }
      }

      /**
       * Write into the specified vector the kept samples for the
       * scalar parameter with the specified index in the chain with
       * the specified index.  The order of samples is the order in
       * which they were added.
       *
       * <p><b>Synchronization</b>: Chain-specific read.
       *
       * @param k Index of chain.
       * @param n Index of parameter.
       * @param samples Vector into which to write samples
       * @throw std::out_of_range If the specified chain index is greater
       * than or equal to the number of chains, or if the specified parameter
       * index is greater than or equal to the total number of parameters.
       */
      void
      get_kept_samples(size_t k,
                       size_t n,
                       std::vector<double>& samples) {
        validate_param_idx(n);
        samples.resize(0);
        samples.reserve(num_kept_samples(k));
        samples.insert(samples.end(),
                       _samples[k][n].begin() + warmup(),
                       _samples[k][n].end());
      }



      /**
       * Write into the specified vector the warmup samples for the
       * scalar parameter with the specified index.  The order of
       * samples is by chain, then by order in which the sample was
       * added to the chain.
       *
       * <p><b>Synchronization</b>: Cross-chain read.
       * 
       * @param n Index of parameter.
       * @param samples Vector into which samples are written.
       * @throw std::out_of_range If the parameter index is greater
       * than or equal to the total number of scalar parameters.
       */
      void
      get_warmup_samples(size_t n,
                         std::vector<double>& samples) {
        validate_param_idx(n);
        samples.resize(0);
        samples.reserve(num_warmup_samples());
        for (size_t k = 0; k < num_chains(); ++k) {
          if (num_warmup_samples(k) < warmup())
            samples.insert(samples.end(),
                           _samples[k][n].begin(),
                           _samples[k][n].end());
          else
            samples.insert(samples.end(),
                           _samples[k][n].begin(),
                           _samples[k][n].begin() + warmup());
        }
      }

      /**
       * Write into the specified vector the warmup samples for the
       * parameter with the specified index in the chain with the
       * specified index.  The order of samples is the order in which
       * they were added.
       *
       * <p><b>Synchronization</b>: Chain-specific read.
       *
       * @param k Index of chain.
       * @param n Index of parameter.
       * @param samples Vector into which to write samples
       * @throw std::out_of_range If the specified chain index is greater
       * than or equal to the number of chains, or if the specified parameter
       * index is greater than or equal to the total number of parameters.
       */
      void
      get_warmup_samples(size_t k,
                         size_t n,
                         std::vector<double>& samples) {
        validate_param_idx(n);
        samples.resize(0);
        samples.reserve(num_warmup_samples(k));
        if (num_warmup_samples(k) < warmup())
            samples.insert(samples.end(),
                           _samples[k][n].begin(),
                           _samples[k][n].end());
        else
            samples.insert(samples.end(),
                           _samples[k][n].begin(),
                           _samples[k][n].begin() + warmup());
      }

    
    };

  }
}


#endif


/*

double mean(size_t n);

double sd(size_t n);

void quantiles(size_t n,
               const vector<double>& probs,
               vector<double>& quantiles);
double quantile(size_t n,
                double prob);

pair<double,double> central_interval(size_t n,
                                     double prob);

pair<double,double> smallest_interval(size_t n,
                                      double prob);

double potential_scale_reduction(size_t n)

double split_potential_scale_reduction(size_t n);

double effective_sample_size(size_t n);

double mcmc_error_mean(size_t n);

double autocorrelation(size_t n, 
                       size_t k);
                   




void print(ostream&);

ostream& operator<<(ostream&, const chains&);
*/
