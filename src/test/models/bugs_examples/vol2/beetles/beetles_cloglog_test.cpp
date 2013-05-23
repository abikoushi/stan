#include <gtest/gtest.h>
#include <test/models/model_test_fixture.hpp>

class Models_BugsExamples_Vol2_BeetlesCloglog : 
  public Model_Test_Fixture<Models_BugsExamples_Vol2_BeetlesCloglog> {
protected:
  virtual void SetUp() {}
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("bugs_examples");
    model_path.push_back("vol2");
    model_path.push_back("beetles");
    model_path.push_back("beetles_cloglog");
    return model_path;
  }

  static bool has_data() {
    return true;
  }

  static bool has_init() {
    return true;
  }

  static int num_iterations(int i) {
    std::vector<int> num_iter;
    num_iter.push_back(2000); //iterations for nuts
    num_iter.push_back(200000); //iterations for unit_metro
    num_iter.push_back(200000); //iterations for diag_metro
    num_iter.push_back(200000); //iterations for dense_metro
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

    expected_values.push_back(make_pair(chains[i]->index("alpha"), -39.77));
    expected_values.push_back(make_pair(chains[i]->index("beta"), 22.15));

    expected_values.push_back(make_pair(chains[i]->index("rhat[1]"), 5.623));
    expected_values.push_back(make_pair(chains[i]->index("rhat[2]"), 11.28));
    expected_values.push_back(make_pair(chains[i]->index("rhat[3]"), 20.91));
    expected_values.push_back(make_pair(chains[i]->index("rhat[4]"), 30.32));
    expected_values.push_back(make_pair(chains[i]->index("rhat[5]"), 47.74));
    expected_values.push_back(make_pair(chains[i]->index("rhat[6]"), 54.08));
    expected_values.push_back(make_pair(chains[i]->index("rhat[7]"), 61.02));
    expected_values.push_back(make_pair(chains[i]->index("rhat[8]"), 59.92));
    
    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BugsExamples_Vol2_BeetlesCloglog,
            Model_Test_Fixture,
            Models_BugsExamples_Vol2_BeetlesCloglog);
