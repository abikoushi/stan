#include <gtest/gtest.h>
#include <test/models/model_test_fixture.hpp>

class Models_BasicDistributions_SortedPair : 
  public Model_Test_Fixture<Models_BasicDistributions_SortedPair> {
protected:
  virtual void SetUp() {
  }
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("basic_distributions");
    model_path.push_back("sorted_pair");
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
    num_iter.push_back(2000); //iterations for nuts
    num_iter.push_back(5000); //iterations for unit_metro
    num_iter.push_back(5000); //iterations for diag_metro
    num_iter.push_back(5000); //iterations for dense_metro
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
    std::vector<std::pair<int, double> > expected_values;
    return expected_values;
  }

};

INSTANTIATE_TYPED_TEST_CASE_P(Models_BasicDistributions_SortedPair,
            Model_Test_Fixture,
            Models_BasicDistributions_SortedPair);

TEST_F(Models_BasicDistributions_SortedPair,
       Test_Sorted_Pair) {
  populate_chains(0);
  Eigen::VectorXd a, b;
  a = chains[0]->samples(chains[0]->index("a"));
  b = chains[0]->samples(chains[0]->index("b"));
  
  for (int n = 0; n < chains[0]->num_samples(); n++) {
    EXPECT_GE(a(n), -1);
    EXPECT_LE(a(n), 1);
    EXPECT_GE(b(n), -1);
    EXPECT_LE(b(n), 1);
    EXPECT_GE(a(n), b(n))
      << n << ": expecting " << a(n) << " to be greater than or equal to " << b(n);
  }
}
