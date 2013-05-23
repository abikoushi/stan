#include <gtest/gtest.h>
#include <test/models/model_test_fixture.hpp>

class Models_BasicDistributions_NormalMixture  : 
  public Model_Test_Fixture<Models_BasicDistributions_NormalMixture> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("basic_distributions");
    model_path.push_back("normal_mixture");
    return model_path;
  }

  static bool has_data() {
    return false;
  }

  static bool has_init() {
    return false;
  }

  static int num_iterations(int i) {
    std::vector<int> num_iter;
    num_iter.push_back(8000); //iterations for nuts
    num_iter.push_back(80000); //iterations for unit_metro
    num_iter.push_back(80000); //iterations for diag_metro
    num_iter.push_back(80000); //iterations for dense_metro
    return num_iter[i];
  }

  static std::vector<int> skip_chains_test(int i) {
    std::vector<int> params_to_skip;
    return params_to_skip;
  }

  static void populate_chains(int i) {
    default_populate_chains(i);
  }

  static std::vector<std::pair<int, double> >
  get_expected_values(int i) {
    using std::make_pair;
    std::vector<std::pair<int, double> > expected_values;
    
    expected_values.push_back(make_pair(chains[i]->index("y"), 
          0.25*0.0 + 0.75*4));
    
    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BasicDistributions_NormalMixture,
            Model_Test_Fixture,
            Models_BasicDistributions_NormalMixture);
