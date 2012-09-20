#ifndef __STAN__PROB__DISTRIBUTIONS__DOUBLE_EXPONENTIAL_HPP__
#define __STAN__PROB__DISTRIBUTIONS__DOUBLE_EXPONENTIAL_HPP__

#include <stan/agrad.hpp>
#include <stan/math/error_handling.hpp>
#include <stan/math/special_functions.hpp>
#include <stan/meta/traits.hpp>
#include <stan/prob/constants.hpp>
#include <stan/prob/traits.hpp>

namespace stan {

  namespace prob {

    // DoubleExponential(y|mu,sigma)  [sigma > 0]
    // FIXME: add documentation
    template <bool Prop,
              typename T_y, typename T_loc, typename T_scale, 
              class Policy>
    typename return_type<T_y,T_loc,T_scale>::type
    double_exponential_log(const T_y& y, const T_loc& mu, const T_scale& sigma, 
                           const Policy&) {
      static const char* function
        = "stan::prob::double_exponential_log(%1%)";
      
      using stan::math::check_finite;
      using stan::math::check_positive;
      using stan::math::check_consistent_sizes;
      using stan::math::value_of;
      using stan::prob::include_summand;
      using std::log;
      using std::fabs;

      // check if any vectors are zero length
      if (!(stan::length(y) 
            && stan::length(mu) 
            && stan::length(sigma)))
        return 0.0;

      // set up return value accumulator
      double logp(0.0);
      if(!check_finite(function, y, "Random variable", &logp, Policy()))
        return logp;
      if(!check_finite(function, mu, "Location parameter", 
                       &logp, Policy()))
        return logp;
      if(!check_finite(function, sigma, "Scale parameter", 
                       &logp, Policy()))
        return logp;
      if(!check_positive(function, sigma, "Scale parameter", 
                         &logp, Policy()))
        return logp;
      if (!(check_consistent_sizes(function,
                                   y,mu,sigma,
				   "Random variable","Location parameter","Shape parameter",
                                   &logp, Policy())))
        return logp;
      
      // check if no variables are involved and prop-to
      if (!include_summand<Prop,T_y,T_loc,T_scale>::value)
	return 0.0;

      // set up template expressions wrapping scalars into vector views
      VectorView<const T_y> y_vec(y);
      VectorView<const T_loc> mu_vec(mu);
      VectorView<const T_scale> sigma_vec(sigma);
      size_t N = max_size(y, mu, sigma);
      agrad::OperandsAndPartials<T_y,T_loc,T_scale>
	operands_and_partials(y, mu, sigma, y_vec, mu_vec, sigma_vec);

      for (size_t n = 0; n < N; n++) {
	const double y_dbl = value_of(y_vec[n]);
	const double mu_dbl = value_of(mu_vec[n]);
	const double sigma_dbl = value_of(sigma_vec[n]);
	
	// reusable subexpressions values
	const double y_m_mu = y_dbl - mu_dbl;
	const double fabs_y_m_mu = fabs(y_m_mu);
	const double inv_sigma = 1.0 / sigma_dbl;

	// log probability
	if (include_summand<Prop>::value)
	  logp += NEG_LOG_TWO;
	if (include_summand<Prop,T_scale>::value)
	  logp -= log(sigma_dbl);
	if (include_summand<Prop,T_y,T_loc,T_scale>::value)
	  logp -= fabs_y_m_mu * inv_sigma;
	
	// gradients
	if (!is_constant<typename is_vector<T_y>::type>::value) {
	  if (y_m_mu > 0)
	    operands_and_partials.d_x1[n] -= inv_sigma;
	  if (y_m_mu < 0)
	    operands_and_partials.d_x1[n] += inv_sigma;
	}
	if (!is_constant<typename is_vector<T_loc>::type>::value) {
	  if (y_m_mu > 0)
	    operands_and_partials.d_x2[n] += inv_sigma;
	  if (y_m_mu < 0)
	    operands_and_partials.d_x2[n] -= inv_sigma;
	}
	if (!is_constant<typename is_vector<T_scale>::type>::value)
	  operands_and_partials.d_x3[n] += -inv_sigma + fabs_y_m_mu * inv_sigma * inv_sigma;
      }
      return operands_and_partials.to_var(logp);
    }


    template <bool Prop,
              typename T_y, typename T_loc, typename T_scale>
    typename return_type<T_y,T_loc,T_scale>::type
    double_exponential_log(const T_y& y, const T_loc& mu, 
                           const T_scale& sigma) {
      return double_exponential_log<Prop>(y,mu,sigma,
                                            stan::math::default_policy());
    }


    template <typename T_y, typename T_loc, typename T_scale, 
              class Policy>
    typename return_type<T_y,T_loc,T_scale>::type
    double_exponential_log(const T_y& y, const T_loc& mu, const T_scale& sigma, 
                           const Policy&) {
      return double_exponential_log<false>(y,mu,sigma,Policy());
    }

    template <typename T_y, typename T_loc, typename T_scale>
    typename return_type<T_y,T_loc,T_scale>::type
    double_exponential_log(const T_y& y, const T_loc& mu, 
                           const T_scale& sigma) {
      return double_exponential_log<false>(y,mu,sigma,
                                           stan::math::default_policy());
    }

    /** 
     * Calculates the double exponential cumulative density function.
     *
     * \f$ f(y|\mu,\sigma) = \begin{cases} \
           \frac{1}{2} \exp\left(\frac{y-\mu}{\sigma}\right), \mbox{if } y < \mu \\ 
           1 - \frac{1}{2} \exp\left(-\frac{y-\mu}{\sigma}\right), \mbox{if } y \ge \mu \
           \end{cases}\f$
     * 
     * @param y A scalar variate.
     * @param mu The location parameter.
     * @param sigma The scale parameter.
     * 
     * @return The cumulative density function.
     */
    template <typename T_y, typename T_loc, typename T_scale, 
              class Policy>
    typename return_type<T_y,T_loc,T_scale>::type
    double_exponential_cdf(const T_y& y, const T_loc& mu, const T_scale& sigma, 
                         const Policy&) {
      static const char* function
        = "stan::prob::double_exponential_cdf(%1%)";
      
      using stan::math::check_finite;
      using stan::math::check_positive;
      using boost::math::tools::promote_args;

      typename promote_args<T_y,T_loc,T_scale>::type lp(0.0);
      if(!check_finite(function, y, "Random variable", &lp, Policy()))
        return lp;
      if(!check_finite(function, mu, "Location parameter", 
                       &lp, Policy()))
        return lp;
      if(!check_finite(function, sigma, "Scale parameter", 
                       &lp, Policy()))
        return lp;
      if(!check_positive(function, sigma, "Scale parameter", 
                         &lp, Policy()))
        return lp;
      
      if (y < mu)
        return exp((y-mu)/sigma)/2;
      else
        return 1 - exp((mu-y)/sigma)/2;
    }
    
    template <typename T_y, typename T_loc, typename T_scale>
    typename boost::math::tools::promote_args<T_y,T_loc,T_scale>::type
    double_exponential_cdf(const T_y& y, const T_loc& mu, const T_scale& sigma) {
      return double_exponential_cdf(y,mu,sigma,stan::math::default_policy());
    }
    
  }
}
#endif
