#include <gtest/gtest.h>
#include <test/models/model_test_fixture.hpp>

class Models_BugsExamples_Vol1_Leukfr : 
  public Model_Test_Fixture<Models_BugsExamples_Vol1_Leukfr> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("bugs_examples");
    model_path.push_back("vol1");
    model_path.push_back("leukfr");
    model_path.push_back("leukfr");
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
    num_iter.push_back(8000); //iterations for nuts
    num_iter.push_back(400000); //iterations for unit_metro
    num_iter.push_back(400000); //iterations for diag_metro
    num_iter.push_back(400000); //iterations for dense_metro
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

    expected_values.push_back(make_pair(chains[i]->index("beta"), 1.607));
    expected_values.push_back(make_pair(chains[i]->index("sigma"), 0.2415));

    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BugsExamples_Vol1_Leukfr,
            Model_Test_Fixture,
            Models_BugsExamples_Vol1_Leukfr);
