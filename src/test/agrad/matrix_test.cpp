#include <gtest/gtest.h>

#include <cstddef>
#include <stdexcept>
#include <complex>

#include <stan/agrad/matrix.hpp>

using stan::agrad::var;

using stan::math::matrix_d;
using stan::agrad::matrix_v;
using stan::math::vector_d;
using stan::agrad::vector_v;
using stan::math::row_vector_d;
using stan::agrad::row_vector_v;

typedef stan::agrad::var AVAR;
typedef std::vector<AVAR> AVEC;
typedef std::vector<double> VEC;


AVEC createAVEC(AVAR x) {
  AVEC v;
  v.push_back(x);
  return v;
}
AVEC createAVEC(AVAR x1, AVAR x2) {
  AVEC v = createAVEC(x1);
  v.push_back(x2);
  return v;
}
AVEC createAVEC(AVAR x1, AVAR x2, AVAR x3) {
  AVEC v = createAVEC(x1,x2);
  v.push_back(x3);
  return v;
}
AVEC createAVEC(AVAR x1, AVAR x2, AVAR x3, AVAR x4) {
  AVEC v = createAVEC(x1,x2,x3);
  v.push_back(x4);
  return v;
}

VEC cgrad(AVAR f, AVAR x1) {
  AVEC x = createAVEC(x1);
  VEC g;
  f.grad(x,g);
  return g;
}

VEC cgrad(AVAR f, AVAR x1, AVAR x2) {
  AVEC x = createAVEC(x1,x2);
  VEC g;
  f.grad(x,g);
  return g;
}

VEC cgrad(AVAR f, AVAR x1, AVAR x2, AVAR x3) {
  AVEC x = createAVEC(x1,x2,x3);
  VEC g;
  f.grad(x,g);
  return g;
}

VEC cgrad(AVAR f, AVAR x1, AVAR x2, AVAR x3, AVAR x4) {
  AVEC x = createAVEC(x1,x2,x3,x4);
  VEC g;
  f.grad(x,g);
  return g;
}

VEC cgradvec(AVAR f, AVEC x) {
  VEC g;
  f.grad(x,g);
  return g;
}


// to_var tests
TEST(agrad_matrix,to_var_scalar) {
  double d = 5.0;
  var v = 5.0;
  stan::agrad::var var_x = stan::agrad::to_var(d);
  EXPECT_FLOAT_EQ(5.0, var_x.val());
  
  var_x = stan::agrad::to_var(v);
  EXPECT_FLOAT_EQ(5.0, var_x.val());
}
TEST(agrad_matrix,to_var_matrix) {
  matrix_d m_d(2,3);
  m_d << 0, 1, 2, 3, 4, 5;
  matrix_v m_v = stan::agrad::to_var(m_d);
  
  EXPECT_EQ(2, m_v.rows());
  EXPECT_EQ(3, m_v.cols());
  for (int ii = 0; ii < 2; ii++) 
    for (int jj = 0; jj < 3; jj++)
      EXPECT_FLOAT_EQ(ii*3 + jj, m_v(ii, jj).val());
}
TEST(agrad_matrix,to_var_ref_matrix) {
  matrix_d m_d(2,3);
  m_d << 0, 1, 2, 3, 4, 5;

  matrix_v m_v(5,5);
  EXPECT_EQ(5, m_v.rows());
  EXPECT_EQ(5, m_v.cols());

  stan::agrad::to_var(m_d, m_v);  
  EXPECT_EQ(2, m_v.rows());
  EXPECT_EQ(3, m_v.cols());
  EXPECT_FLOAT_EQ(0, m_v(0, 0).val());
  EXPECT_FLOAT_EQ(1, m_v(0, 1).val());
  EXPECT_FLOAT_EQ(2, m_v(0, 2).val());
  EXPECT_FLOAT_EQ(3, m_v(1, 0).val());
  EXPECT_FLOAT_EQ(4, m_v(1, 1).val());
  EXPECT_FLOAT_EQ(5, m_v(1, 2).val());
}
TEST(agrad_matrix,to_var_vector) {
  vector_d d(5);
  vector_v v(5);
  
  d << 1, 2, 3, 4, 5;
  v << 1, 2, 3, 4, 5;
  
  vector_v out = stan::agrad::to_var(d);
  EXPECT_FLOAT_EQ(1, out(0).val());
  EXPECT_FLOAT_EQ(2, out(1).val());
  EXPECT_FLOAT_EQ(3, out(2).val());
  EXPECT_FLOAT_EQ(4, out(3).val());
  EXPECT_FLOAT_EQ(5, out(4).val());

  out = stan::agrad::to_var(v);
  EXPECT_FLOAT_EQ(1, out(0).val());
  EXPECT_FLOAT_EQ(2, out(1).val());
  EXPECT_FLOAT_EQ(3, out(2).val());
  EXPECT_FLOAT_EQ(4, out(3).val());
  EXPECT_FLOAT_EQ(5, out(4).val());
}
TEST(agrad_matrix,to_var_ref_vector) {
  vector_d d(5);
  vector_v v(5);
  
  d << 1, 2, 3, 4, 5;
  v << 1, 2, 3, 4, 5;
  
  vector_v output;
  stan::agrad::to_var(d, output);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());

  stan::agrad::to_var(v, output);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());
}
TEST(agrad_matrix,to_var_rowvector) {
  row_vector_d d(5);
  row_vector_v v(5);
  
  d << 1, 2, 3, 4, 5;
  v << 1, 2, 3, 4, 5;
  
  row_vector_v output = stan::agrad::to_var(d);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());

  output.resize(0);
  output = stan::agrad::to_var(v);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());
}
TEST(agrad_matrix,to_var_ref_rowvector) {
  row_vector_d d(5);
  row_vector_v v(5);

  d << 1, 2, 3, 4, 5;
  v << 1, 2, 3, 4, 5;
 
  row_vector_v output;
  stan::agrad::to_var(d, output);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());

  output.resize(0);
  stan::agrad::to_var(d, output);
  EXPECT_FLOAT_EQ(1, output(0).val());
  EXPECT_FLOAT_EQ(2, output(1).val());
  EXPECT_FLOAT_EQ(3, output(2).val());
  EXPECT_FLOAT_EQ(4, output(3).val());
  EXPECT_FLOAT_EQ(5, output(4).val());
}
// end to_var tests

// rows tests
TEST(agrad_matrix,rows_vector) {
  vector_v v(5);
  v << 0, 1, 2, 3, 4;
  EXPECT_EQ(5U, stan::agrad::rows(v));
  
  v.resize(0);
  EXPECT_EQ(0U, stan::agrad::rows(v));
}
TEST(agrad_matrix,rows_rowvector) {
  row_vector_v rv(5);
  rv << 0, 1, 2, 3, 4;
  EXPECT_EQ(1U, stan::agrad::rows(rv));

  rv.resize(0);
  EXPECT_EQ(1U, stan::agrad::rows(rv));
}
TEST(agrad_matrix,rows_matrix) {
  matrix_v m(2,3);
  m << 0, 1, 2, 3, 4, 5;
  EXPECT_EQ(2U, stan::agrad::rows(m));
  
  m.resize(0,2);
  EXPECT_EQ(0U, stan::agrad::rows(m));
}
// end rows tests

// cols tests
TEST(agrad_matrix,cols_vector) {
  vector_v v(5);
  v << 0, 1, 2, 3, 4;
  EXPECT_EQ(1U, stan::agrad::cols(v));

  v.resize(0);
  EXPECT_EQ(1U, stan::agrad::cols(v));
}
TEST(agrad_matrix,cols_rowvector) {
  row_vector_v rv(5);
  rv << 0, 1, 2, 3, 4;
  EXPECT_EQ(5U, stan::agrad::cols(rv));
  
  rv.resize(0);
  EXPECT_EQ(0U, stan::agrad::cols(rv));
}
TEST(agrad_matrix,cols_matrix) {
  matrix_v m(2,3);
  m << 0, 1, 2, 3, 4, 5;
  EXPECT_EQ(3U, stan::agrad::cols(m));
  
  m.resize(5, 0);
  EXPECT_EQ(0U, stan::agrad::cols(m));
}
// end cols_tests

// determinant tests
TEST(agrad_matrix,determinant) {
  matrix_v v(2,2);
  v << 0, 1, 2, 3;

  var det;
  det = stan::agrad::determinant(v);
  EXPECT_FLOAT_EQ(-2, det.val());
}
TEST(agrad_matrix,deteriminant_exception) {
  matrix_v v(2,3);

  var det;
  EXPECT_THROW(det = stan::agrad::determinant(v), std::domain_error);
}
TEST(agrad_matrix,determinant_grad) {
  matrix_v X(2,2);
  AVAR a = 2.0;
  AVAR b = 3.0;
  AVAR c = 5.0;
  AVAR d = 7.0;
  X << a, b, c, d;

  AVEC x = createAVEC(a,b,c,d);

  AVAR f = X.determinant();

  // det = ad - bc
  EXPECT_FLOAT_EQ(-1.0,f.val());

  std::vector<double> g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(7.0,g[0]);
  EXPECT_FLOAT_EQ(-5.0,g[1]);
  EXPECT_FLOAT_EQ(-3.0,g[2]);
  EXPECT_FLOAT_EQ(2.0,g[3]);
}
TEST(agrad_matrix,determinant3by3) {
  // just test it can handle it
  matrix_v Z(9,9);
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      Z(i,j) = i * j + 1;
  AVAR h = Z.determinant();
  h = h; // supresses set but not used warning
}

// end determinant tests

// dot_product tests
TEST(agrad_matrix, dot_product_vector_vector) {
  vector_d vd_1(3), vd_2(3);
  vector_v vv_1(3), vv_2(3);
  
  vd_1 << 1, 3, -5;
  vv_1 << 1, 3, -5;
  vd_2 << 4, -2, -1;
  vv_2 << 4, -2, -1;

  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(vv_1, vd_2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(vd_1, vv_2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(vv_1, vv_2).val());
}
TEST(agrad_matrix, dot_product_vector_vector_exception) {
  vector_d d1(3);
  vector_v v1(3);
  vector_d d2(2);
  vector_v v2(4);

  EXPECT_THROW(stan::agrad::dot_product(v1, d2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(d1, v2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(v1, v2), std::domain_error);
}
TEST(agrad_matrix, dot_product_rowvector_vector) {
  row_vector_d d1(3);
  row_vector_v v1(3);
  vector_d d2(3);
  vector_v v2(3);
  
  d1 << 1, 3, -5;
  v1 << 1, 3, -5;
  d2 << 4, -2, -1;
  v2 << 4, -2, -1;

  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, d2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(d1, v2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, v2).val());
}
TEST(agrad_matrix, dot_product_rowvector_vector_exception) {
  row_vector_d d1(3);
  row_vector_v v1(3);
  vector_d d2(2);
  vector_v v2(4);

  EXPECT_THROW(stan::agrad::dot_product(v1, d2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(d1, v2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(v1, v2), std::domain_error);
}
TEST(agrad_matrix, dot_product_vector_rowvector) {
  vector_d d1(3);
  vector_v v1(3);
  row_vector_d d2(3);
  row_vector_v v2(3);
  
  d1 << 1, 3, -5;
  v1 << 1, 3, -5;
  d2 << 4, -2, -1;
  v2 << 4, -2, -1;
  
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, d2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(d1, v2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, v2).val());
}
TEST(agrad_matrix, dot_product_vector_rowvector_exception) {
  vector_d d1(3);
  vector_v v1(3);
  row_vector_d d2(2);
  row_vector_v v2(4);

  EXPECT_THROW(stan::agrad::dot_product(v1, d2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(d1, v2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(v1, v2), std::domain_error);
}
TEST(agrad_matrix, dot_product_rowvector_rowvector) {
  row_vector_d d1(3), d2(3);
  row_vector_v v1(3), v2(3);
  
  d1 << 1, 3, -5;
  v1 << 1, 3, -5;
  d2 << 4, -2, -1;
  v2 << 4, -2, -1;

  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, d2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(d1, v2).val());
  EXPECT_FLOAT_EQ(3, stan::agrad::dot_product(v1, v2).val());
}
TEST(agrad_matrix, dot_product_rowvector_rowvector_exception) {
  row_vector_d d1(3), d2(2);
  row_vector_v v1(3), v2(4);

  EXPECT_THROW(stan::agrad::dot_product(v1, d2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(d1, v2), std::domain_error);
  EXPECT_THROW(stan::agrad::dot_product(v1, v2), std::domain_error);
}
// end dot_product tests

// exp tests
TEST(agrad_matrix, exp_matrix) {
  using stan::math::exp;
  matrix_d expected_output(2,2);
  matrix_v mv(2,2), output;
  int i,j;

  mv << 1, 2, 3, 4;
  expected_output << std::exp(1), std::exp(2), std::exp(3), std::exp(4);
  output = exp(mv);

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      EXPECT_FLOAT_EQ(expected_output(i,j), output(i,j).val());
}

// log tests
TEST(agrad_matrix, log_matrix) {
  using stan::math::log;
  matrix_d expected_output(2,2);
  matrix_v mv(2,2), output;
  int i,j;

  mv << 1, 2, 3, 4;
  expected_output << std::log(1), std::log(2), std::log(3), std::log(4);
  output = log(mv);

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      EXPECT_FLOAT_EQ(expected_output(i,j), output(i,j).val());
}

// scalar add/subtract tests
TEST(agrad_matrix,add_scalar) {
  matrix_v v(2,2);
  v << 1, 2, 3, 4;
  matrix_v result;

  result = add(2.0,v);
  EXPECT_FLOAT_EQ(3.0,result(0,0).val());
  EXPECT_FLOAT_EQ(4.0,result(0,1).val());
  EXPECT_FLOAT_EQ(5.0,result(1,0).val());
  EXPECT_FLOAT_EQ(6.0,result(1,1).val());

  result = add(v,2.0);
  EXPECT_FLOAT_EQ(3.0,result(0,0).val());
  EXPECT_FLOAT_EQ(4.0,result(0,1).val());
  EXPECT_FLOAT_EQ(5.0,result(1,0).val());
  EXPECT_FLOAT_EQ(6.0,result(1,1).val());
}

TEST(agrad_matrix,subtract_scalar) {
  using stan::math::subtract;
  matrix_v v(2,2);
  v << 1, 2, 3, 4;
  matrix_v result;

  result = subtract(2.0,v);
  EXPECT_FLOAT_EQ(1.0,result(0,0).val());
  EXPECT_FLOAT_EQ(0.0,result(0,1).val());
  EXPECT_FLOAT_EQ(-1.0,result(1,0).val());
  EXPECT_FLOAT_EQ(-2.0,result(1,1).val());

  result = subtract(v,2.0);
  EXPECT_FLOAT_EQ(-1.0,result(0,0).val());
  EXPECT_FLOAT_EQ(0.0,result(0,1).val());
  EXPECT_FLOAT_EQ(1.0,result(1,0).val());
  EXPECT_FLOAT_EQ(2.0,result(1,1).val());
}

// add tests
TEST(agrad_matrix, add_vector_vector) {
  using stan::math::add;
  vector_d vd_1(5);
  vector_d vd_2(5);
  vector_v vv_1(5);
  vector_v vv_2(5);
  
  vd_1 << 1, 2, 3, 4, 5;
  vv_1 << 1, 2, 3, 4, 5;
  vd_2 << 2, 3, 4, 5, 6;
  vv_2 << 2, 3, 4, 5, 6;
  
  vector_d expected_output(5);
  expected_output << 3, 5, 7, 9, 11;
  
  vector_d output_d;
  output_d = add(vd_1, vd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_d(0));
  EXPECT_FLOAT_EQ(expected_output(1), output_d(1));
  EXPECT_FLOAT_EQ(expected_output(2), output_d(2));
  EXPECT_FLOAT_EQ(expected_output(3), output_d(3));
  EXPECT_FLOAT_EQ(expected_output(4), output_d(4));  

  vector_v output_v = add(vv_1, vd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  

  output_v = add(vd_1, vv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  

  output_v = add(vv_1, vv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  
}
TEST(agrad_matrix, add_vector_vector_exception) {
  using stan::math::add;
  vector_d d1(5), d2(1);
  vector_v v1(5), v2(1);
  
  EXPECT_THROW(add(d1, d2), std::domain_error);
  EXPECT_THROW(add(v1, d2), std::domain_error);
  EXPECT_THROW(add(d1, v2), std::domain_error);
  EXPECT_THROW(add(v1, v2), std::domain_error);
}
TEST(agrad_matrix, add_rowvector_rowvector) {
  using stan::math::add;
  row_vector_d rvd_1(5), rvd_2(5);
  row_vector_v rvv_1(5), rvv_2(5);

  rvd_1 << 1, 2, 3, 4, 5;
  rvv_1 << 1, 2, 3, 4, 5;
  rvd_2 << 2, 3, 4, 5, 6;
  rvv_2 << 2, 3, 4, 5, 6;
  
  row_vector_d expected_output(5);
  expected_output << 3, 5, 7, 9, 11;
  
  row_vector_d output_d = add(rvd_1, rvd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_d(0));
  EXPECT_FLOAT_EQ(expected_output(1), output_d(1));
  EXPECT_FLOAT_EQ(expected_output(2), output_d(2));
  EXPECT_FLOAT_EQ(expected_output(3), output_d(3));
  EXPECT_FLOAT_EQ(expected_output(4), output_d(4));  

  row_vector_v output_v = add(rvv_1, rvd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  

  output_v = add(rvd_1, rvv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  

  output_v = add(rvv_1, rvv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_v(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output_v(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output_v(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output_v(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output_v(4).val());  
}
TEST(agrad_matrix, add_rowvector_rowvector_exception) {
  using stan::math::add;
  row_vector_d d1(5), d2(2);
  row_vector_v v1(5), v2(2);

  row_vector_v output;
  EXPECT_THROW( add(d1, d2), std::domain_error);
  EXPECT_THROW( add(d1, v2), std::domain_error);
  EXPECT_THROW( add(v1, d2), std::domain_error);
  EXPECT_THROW( add(v1, v2), std::domain_error);
}
TEST(agrad_matrix, add_matrix_matrix) {
  using stan::math::add;
  matrix_d md_1(2,2), md_2(2,2);
  matrix_v mv_1(2,2), mv_2(2,2);

  md_1 << -10, 1, 10, 0;
  mv_1 << -10, 1, 10, 0;
  md_2 << 10, -10, 1, 2;
  mv_2 << 10, -10, 1, 2;
  
  matrix_d expected_output(2,2);
  expected_output << 0, -9, 11, 2;
  
  matrix_d output_d = add(md_1, md_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output_d(0,0));
  EXPECT_FLOAT_EQ(expected_output(0,1), output_d(0,1));
  EXPECT_FLOAT_EQ(expected_output(1,0), output_d(1,0));
  EXPECT_FLOAT_EQ(expected_output(1,1), output_d(1,1));

  matrix_v output_v = add(mv_1, md_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output_v(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output_v(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output_v(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output_v(1,1).val());

  output_v = add(md_1, mv_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output_v(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output_v(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output_v(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output_v(1,1).val());

  output_v = add(mv_1, mv_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output_v(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output_v(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output_v(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output_v(1,1).val());
}
TEST(agrad_matrix, add_matrix_matrix_exception) {
  using stan::math::add;

  matrix_d d1(2,2), d2(1,2);
  matrix_v v1(2,2), v2(1,2);

  EXPECT_THROW(add(d1, d2), std::domain_error);
  EXPECT_THROW(add(d1, v2), std::domain_error);
  EXPECT_THROW(add(v1, d2), std::domain_error);
  EXPECT_THROW(add(v1, v2), std::domain_error);
}
// end add tests

// subtract tests
TEST(agrad_matrix, subtract_vector_vector) {
  using stan::math::subtract;

  vector_d expected_output(5);
  vector_v output;
  vector_d output_d;
  vector_d vd_1(5), vd_2(5);
  vector_v vv_1(5), vv_2(5);

  vd_1 << 0, 2, -6, 10, 6;
  vv_1 << 0, 2, -6, 10, 6;
  vd_2 << 2, 3, 4, 5, 6;
  vv_2 << 2, 3, 4, 5, 6;
  
  expected_output << -2, -1, -10, 5, 0;
  
  output_d = subtract(vd_1, vd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_d(0));
  EXPECT_FLOAT_EQ(expected_output(1), output_d(1));
  EXPECT_FLOAT_EQ(expected_output(2), output_d(2));
  EXPECT_FLOAT_EQ(expected_output(3), output_d(3));
  EXPECT_FLOAT_EQ(expected_output(4), output_d(4));  

  output = subtract(vv_1, vd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  

  output = subtract(vd_1, vv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  

  output = subtract(vv_1, vv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  
}
TEST(agrad_matrix, subtract_vector_vector_exception) {
  using stan::math::subtract;
  vector_d d1(5), d2(1);
  vector_v v1(5), v2(1);
  
  vector_v output;
  EXPECT_THROW( subtract(d1, d2), std::domain_error);
  EXPECT_THROW( subtract(v1, d2), std::domain_error);
  EXPECT_THROW( subtract(d1, v2), std::domain_error);
  EXPECT_THROW( subtract(v1, v2), std::domain_error);
}
TEST(agrad_matrix, subtract_rowvector_rowvector) {
  using stan::math::subtract;
  row_vector_d expected_output(5);
  row_vector_d  output_d;
  row_vector_v  output;
  row_vector_d rvd_1(5), rvd_2(5);
  row_vector_v rvv_1(5), rvv_2(5);

  rvd_1 << 0, 2, -6, 10, 6;
  rvv_1 << 0, 2, -6, 10, 6;
  rvd_2 << 2, 3, 4, 5, 6;
  rvv_2 << 2, 3, 4, 5, 6;
  
  expected_output << -2, -1, -10, 5, 0;
  
  output_d = subtract(rvd_1, rvd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output_d(0));
  EXPECT_FLOAT_EQ(expected_output(1), output_d(1));
  EXPECT_FLOAT_EQ(expected_output(2), output_d(2));
  EXPECT_FLOAT_EQ(expected_output(3), output_d(3));
  EXPECT_FLOAT_EQ(expected_output(4), output_d(4));

  output = subtract(rvv_1, rvd_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  

  output = subtract(rvd_1, rvv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  

  output = subtract(rvv_1, rvv_2);
  EXPECT_FLOAT_EQ(expected_output(0), output(0).val());
  EXPECT_FLOAT_EQ(expected_output(1), output(1).val());
  EXPECT_FLOAT_EQ(expected_output(2), output(2).val());
  EXPECT_FLOAT_EQ(expected_output(3), output(3).val());
  EXPECT_FLOAT_EQ(expected_output(4), output(4).val());  
}
TEST(agrad_matrix, subtract_rowvector_rowvector_exception) {
  using stan::math::subtract;
  row_vector_d d1(5), d2(2);
  row_vector_v v1(5), v2(2);

  row_vector_v output;
  EXPECT_THROW( subtract(d1, d2), std::domain_error);
  EXPECT_THROW( subtract(d1, v2), std::domain_error);
  EXPECT_THROW( subtract(v1, d2), std::domain_error);
  EXPECT_THROW( subtract(v1, v2), std::domain_error);
}
TEST(agrad_matrix, subtract_matrix_matrix) {
  using stan::math::subtract;
  matrix_d expected_output(2,2);
  matrix_v output;
  matrix_d md_1(2,2), md_2(2,2);
  matrix_v mv_1(2,2), mv_2(2,2);
  matrix_d md_mis (2, 3);
  matrix_v mv_mis (1, 1);

  md_1 << -10, 1, 10, 0;
  mv_1 << -10, 1, 10, 0;
  md_2 << 10, -10, 1, 2;
  mv_2 << 10, -10, 1, 2;
  
  expected_output << -20, 11, 9, -2;
  
  matrix_d output_d = subtract(md_1, md_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output_d(0,0));
  EXPECT_FLOAT_EQ(expected_output(0,1), output_d(0,1));
  EXPECT_FLOAT_EQ(expected_output(1,0), output_d(1,0));
  EXPECT_FLOAT_EQ(expected_output(1,1), output_d(1,1));

  output = subtract(mv_1, md_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output(1,1).val());

  output = subtract(md_1, mv_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output(1,1).val());

  output = subtract(mv_1, mv_2);
  EXPECT_FLOAT_EQ(expected_output(0,0), output(0,0).val());
  EXPECT_FLOAT_EQ(expected_output(0,1), output(0,1).val());
  EXPECT_FLOAT_EQ(expected_output(1,0), output(1,0).val());
  EXPECT_FLOAT_EQ(expected_output(1,1), output(1,1).val());
}
TEST(agrad_matrix, subtract_matrix_matrix_exception) {
  using stan::math::subtract;
  matrix_d d1(2,2), d2(1,2);
  matrix_v v1(2,2), v2(1,2);

  EXPECT_THROW( subtract(d1, d2), std::domain_error);
  EXPECT_THROW( subtract(d1, v2), std::domain_error);
  EXPECT_THROW( subtract(v1, d2), std::domain_error);
  EXPECT_THROW( subtract(v1, v2), std::domain_error);
}
// end subtract tests

// minus tests
TEST(agrad_matrix, minus_scalar) {
  double x = 10;
  var v = 11;
  
  EXPECT_FLOAT_EQ(-10, stan::agrad::minus(x));
  EXPECT_FLOAT_EQ(-11, stan::agrad::minus(v).val());
}
TEST(agrad_matrix, minus_vector) {
  vector_d d(3);
  vector_v v(3);

  d << -100, 0, 1;
  v << -100, 0, 1;
  
  vector_d output_d;
  output_d = stan::agrad::minus(d);
  EXPECT_FLOAT_EQ(100, output_d[0]);
  EXPECT_FLOAT_EQ(0, output_d[1]);
  EXPECT_FLOAT_EQ(-1, output_d[2]);

  vector_v output;
  output = stan::agrad::minus(v);
  EXPECT_FLOAT_EQ(100, output[0].val());
  EXPECT_FLOAT_EQ(0, output[1].val());
  EXPECT_FLOAT_EQ(-1, output[2].val());
}
TEST(agrad_matrix, minus_rowvector) {
  row_vector_d d(3);
  row_vector_v v(3);

  d << -100, 0, 1;
  v << -100, 0, 1;
  
  row_vector_d output_d;
  output_d = stan::agrad::minus(d);
  EXPECT_FLOAT_EQ(100, output_d[0]);
  EXPECT_FLOAT_EQ(0, output_d[1]);
  EXPECT_FLOAT_EQ(-1, output_d[2]);

  row_vector_v output;
  output = stan::agrad::minus(v);
  EXPECT_FLOAT_EQ(100, output[0].val());
  EXPECT_FLOAT_EQ(0, output[1].val());
  EXPECT_FLOAT_EQ(-1, output[2].val());
}
TEST(agrad_matrix, minus_matrix) {
  matrix_d d(2, 3);
  matrix_v v(2, 3);

  d << -100, 0, 1, 20, -40, 2;
  v << -100, 0, 1, 20, -40, 2;

  matrix_d output_d = stan::agrad::minus(d);
  EXPECT_FLOAT_EQ(100, output_d(0,0));
  EXPECT_FLOAT_EQ(  0, output_d(0,1));
  EXPECT_FLOAT_EQ( -1, output_d(0,2));
  EXPECT_FLOAT_EQ(-20, output_d(1,0));
  EXPECT_FLOAT_EQ( 40, output_d(1,1));
  EXPECT_FLOAT_EQ( -2, output_d(1,2));

  matrix_v output = stan::agrad::minus(v);
  EXPECT_FLOAT_EQ(100, output(0,0).val());
  EXPECT_FLOAT_EQ(  0, output(0,1).val());
  EXPECT_FLOAT_EQ( -1, output(0,2).val());
  EXPECT_FLOAT_EQ(-20, output(1,0).val());
  EXPECT_FLOAT_EQ( 40, output(1,1).val());
  EXPECT_FLOAT_EQ( -2, output(1,2).val());
}
// end minus tests

// divide tests
TEST(agrad_matrix, divide_scalar) {
  using stan::agrad::divide;
  double d1, d2;
  var    v1, v2;

  d1 = 10;
  v1 = 10;
  d2 = -2;
  v2 = -2;
  
  EXPECT_FLOAT_EQ(-5, divide(d1, d2));
  EXPECT_FLOAT_EQ(-5, divide(d1, v2).val());
  EXPECT_FLOAT_EQ(-5, divide(v1, d2).val());
  EXPECT_FLOAT_EQ(-5, divide(v1, v2).val());

  d2 = 0;
  v2 = 0;

  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), divide(d1, d2));
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), divide(d1, v2).val());
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), divide(v1, d2).val());
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), divide(v1, v2).val());

  d1 = 0;
  v1 = 0;
  EXPECT_TRUE(std::isnan(divide(d1, d2)));
  EXPECT_TRUE(std::isnan(divide(d1, v2).val()));
  EXPECT_TRUE(std::isnan(divide(v1, d2).val()));
  EXPECT_TRUE(std::isnan(divide(v1, v2).val()));
}
TEST(agrad_matrix, divide_vector) {
  using stan::math::divide;
  vector_d d1(3);
  vector_v v1(3);
  double d2;
  var v2;
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  d2 = -2;
  v2 = -2;
  
  vector_d output_d;
  output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(-50, output_d(0));
  EXPECT_FLOAT_EQ(  0, output_d(1));
  EXPECT_FLOAT_EQ(1.5, output_d(2));

  vector_v output;
  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());

  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());

  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());


  d2 = 0;
  v2 = 0;
  output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output_d(0));
  EXPECT_TRUE (std::isnan(output_d(1)));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output_d(2));

  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE (std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());

  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE (std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());

  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE (std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());
}
TEST(agrad_matrix, divide_rowvector) {
  using stan::math::divide;
  row_vector_d d1(3);
  row_vector_v v1(3);
  double d2;
  var v2;
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  d2 = -2;
  v2 = -2;
  
  row_vector_d output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(-50, output_d(0));
  EXPECT_FLOAT_EQ(  0, output_d(1));
  EXPECT_FLOAT_EQ(1.5, output_d(2));

  row_vector_v output;
  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());

  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());

  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(-50, output(0).val());
  EXPECT_FLOAT_EQ(  0, output(1).val());
  EXPECT_FLOAT_EQ(1.5, output(2).val());

  d2 = 0;
  v2 = 0;
  output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output_d(0));
  EXPECT_TRUE(std::isnan(output_d(1)));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output_d(2));

  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE(std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());

  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE (std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());

  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0).val());
  EXPECT_TRUE (std::isnan(output(1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(2).val());
}
TEST(agrad_matrix, divide_matrix) {
  using stan::math::divide;

  matrix_d d1(2,2);
  matrix_v v1(2,2);
  double d2;
  var v2;
  
  d1 << 100, 0, -3, 4;
  v1 << 100, 0, -3, 4;
  d2 = -2;
  v2 = -2;
  
  matrix_d output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(-50, output_d(0,0));
  EXPECT_FLOAT_EQ(  0, output_d(0,1));
  EXPECT_FLOAT_EQ(1.5, output_d(1,0));
  EXPECT_FLOAT_EQ( -2, output_d(1,1));

  matrix_v output;
  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(-50, output(0,0).val());
  EXPECT_FLOAT_EQ(  0, output(0,1).val());
  EXPECT_FLOAT_EQ(1.5, output(1,0).val());
  EXPECT_FLOAT_EQ( -2, output(1,1).val());
  
  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(-50, output(0,0).val());
  EXPECT_FLOAT_EQ(  0, output(0,1).val());
  EXPECT_FLOAT_EQ(1.5, output(1,0).val());
  EXPECT_FLOAT_EQ( -2, output(1,1).val());
  
  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(-50, output(0,0).val());
  EXPECT_FLOAT_EQ(  0, output(0,1).val());
  EXPECT_FLOAT_EQ(1.5, output(1,0).val());
  EXPECT_FLOAT_EQ( -2, output(1,1).val());

  d2 = 0;
  v2 = 0;
  output_d = divide(d1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output_d(0,0));
  EXPECT_TRUE(std::isnan(output_d(0,1)));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output_d(1,0));
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output_d(1,1));

  output = divide(d1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0,0).val());
  EXPECT_TRUE (std::isnan(output(0,1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(1,0).val());
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(1,1).val());

  output = divide(v1, d2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0,0).val());
  EXPECT_TRUE (std::isnan(output(0,1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(1,0).val());
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(1,1).val());

  output = divide(v1, v2);
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(0,0).val());
  EXPECT_TRUE (std::isnan(output(0,1).val()));
  EXPECT_FLOAT_EQ(-std::numeric_limits<double>::infinity(), output(1,0).val());
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), output(1,1).val());
}
// end divide tests

// min tests
TEST(agrad_matrix, min_vector) {
  using stan::math::min;
  vector_d d1(3);
  vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = min(d1);
  EXPECT_FLOAT_EQ(-3, output.val());
                   
  output = min(v1);
  EXPECT_FLOAT_EQ(-3, output.val());
}
TEST(agrad_matrix, min_vector_exception) {
  using stan::math::min;
  using stan::math::max;
  vector_d d;
  vector_v v;
  d.resize(0);
  v.resize(0);
  EXPECT_EQ(std::numeric_limits<double>::infinity(), min(v).val());
  EXPECT_EQ(-std::numeric_limits<double>::infinity(), max(v).val());
}
TEST(agrad_matrix, min_rowvector) {
  using stan::math::min;
  row_vector_d d1(3);
  row_vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = min(d1);
  EXPECT_FLOAT_EQ(-3, output.val());
                   
  output = min(v1);
  EXPECT_FLOAT_EQ(-3, output.val());
}
TEST(agrad_matrix, min_rowvector_exception) {
  using stan::math::min;
  row_vector_v v;
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), min(v).val());
}
TEST(agrad_matrix, min_matrix) {
  using stan::math::min;
  matrix_d d1(3,1);
  matrix_v v1(1,3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = min(d1);
  EXPECT_FLOAT_EQ(-3, output.val());
                   
  output = min(v1);
  EXPECT_FLOAT_EQ(-3, output.val());
}
TEST(agrad_matrix, min_matrix_exception) {
  using stan::math::min;
  matrix_v v;
  EXPECT_FLOAT_EQ(std::numeric_limits<double>::infinity(), min(v).val());
}
// end min tests

// max tests
TEST(agrad_matrix, max_vector) {
  using stan::math::max;

  vector_d d1(3);
  vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = max(d1);
  EXPECT_FLOAT_EQ(100, output.val());
                   
  output = max(v1);
  EXPECT_FLOAT_EQ(100, output.val());
}
TEST(agrad_matrix, max_vector_exception) {
  using stan::math::max;
  vector_v v;
  EXPECT_EQ(-std::numeric_limits<double>::infinity(), max(v).val());
}
TEST(agrad_matrix, max_rowvector) {
  using stan::math::max;
  row_vector_d d1(3);
  row_vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = max(d1);
  EXPECT_FLOAT_EQ(100, output.val());
                   
  output = max(v1);
  EXPECT_FLOAT_EQ(100, output.val());
}
TEST(agrad_matrix, max_rowvector_exception) {
  using stan::math::max;
  row_vector_v v;
  EXPECT_EQ(-std::numeric_limits<double>::infinity(), max(v).val());
}
TEST(agrad_matrix, max_matrix) {
  using stan::math::max;
  matrix_d d1(3,1);
  matrix_v v1(1,3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = max(d1);
  EXPECT_FLOAT_EQ(100, output.val());
                   
  output = max(v1);
  EXPECT_FLOAT_EQ(100, output.val());
}
TEST(agrad_matrix, max_matrix_exception) {
  using stan::math::max;
  matrix_v v;
  EXPECT_EQ(-std::numeric_limits<double>::infinity(), max(v).val());
}
// end max tests

// mean tests
TEST(agrad_matrix, mean_vector) {
  using stan::math::mean;
  vector_d d1(3);
  vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = mean(d1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
                   
  output = mean(v1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
}
TEST(agrad_matrix, mean_vector_exception) {
  using stan::math::mean;
  vector_d d;
  vector_v v;
  EXPECT_THROW(mean(d), std::domain_error);
  EXPECT_THROW(mean(v), std::domain_error);
}
TEST(agrad_matrix, mean_rowvector) {
  using stan::math::mean;
  row_vector_d d1(3);
  row_vector_v v1(3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = mean(d1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
                   
  output = mean(v1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
}
TEST(agrad_matrix, mean_rowvector_exception) {
  using stan::math::mean;
  row_vector_d d;
  row_vector_v v;
  EXPECT_THROW(mean(d), std::domain_error);
  EXPECT_THROW(mean(v), std::domain_error);
}
TEST(agrad_matrix, mean_matrix) {
  using stan::math::mean;
  matrix_d d1(3,1);
  matrix_v v1(1,3);
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  
  var output;
  output = mean(d1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
                   
  output = mean(v1);
  EXPECT_FLOAT_EQ(97.0/3.0, output.val());
}
TEST(agrad_matrix, mean_matrix_exception) {
  using stan::math::mean;
  matrix_d d;
  matrix_v v;
  EXPECT_THROW(mean(d), std::domain_error);
  EXPECT_THROW(mean(v), std::domain_error);
}
// end mean tests

// variance tests
TEST(agrad_matrix, variance_vector) {
  using stan::math::variance;
  vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  vector_d d1(6);
  vector_v v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val());

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val());  
}
TEST(agrad_matrix, variance_vector_exception) {
  using stan::math::variance;
  vector_d d1;
  vector_v v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(agrad_matrix, variance_rowvector) {
  using stan::math::variance;
  row_vector_d d(1);
  d << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(d));

  row_vector_d d1(6);
  row_vector_v v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val());

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val());  
}
TEST(agrad_matrix, variance_rowvector_exception) {
  using stan::math::variance;
  row_vector_d d1;
  row_vector_v v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
TEST(agrad_matrix, variance_matrix) {
  using stan::math::variance;
  matrix_d m(1,1);
  m << 12.9;
  EXPECT_FLOAT_EQ(0.0,variance(m));

  matrix_d d1(2, 3);
  matrix_v v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(17.5/5.0, variance(d1));
                   
  EXPECT_FLOAT_EQ(17.5/5.0, variance(v1).val());

  d1.resize(1,1);
  v1.resize(1,1);
  EXPECT_FLOAT_EQ(0.0, variance(d1));
  EXPECT_FLOAT_EQ(0.0, variance(v1).val());  
}
TEST(agrad_matrix, variance_matrix_exception) {
  using stan::math::variance;
  matrix_d d1;
  matrix_v v1;
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(0,1);
  v1.resize(0,1);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);

  d1.resize(1,0);
  v1.resize(1,0);
  EXPECT_THROW(variance(d1), std::domain_error);
  EXPECT_THROW(variance(v1), std::domain_error);
}
// end variance tests

// sd tests
TEST(agrad_matrix, sd_vector) {
  using stan::math::sd;
  vector_d v(1);
  v << 1.0;
  EXPECT_FLOAT_EQ(0.0, sd(v));

  vector_d d1(6);
  vector_v v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(d1));
                   
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(v1).val());
  
  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, sd(d1));
  EXPECT_FLOAT_EQ(0.0, sd(v1).val());
}
TEST(agrad_matrix, sd_vector_exception) {
  using stan::math::sd;
  vector_d d1;
  vector_v v1;
  EXPECT_THROW(sd(d1), std::domain_error);
  EXPECT_THROW(sd(v1), std::domain_error);
}
TEST(agrad_matrix, sd_rowvector) {
  using stan::math::sd;
  row_vector_d v(1);
  v << 1.0;
  EXPECT_FLOAT_EQ(0.0, sd(v));


  row_vector_d d1(6);
  row_vector_v v1(6);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(d1));
                   
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(v1).val());

  d1.resize(1);
  v1.resize(1);
  EXPECT_FLOAT_EQ(0.0, sd(d1));
  EXPECT_FLOAT_EQ(0.0, sd(v1).val());
}
TEST(agrad_matrix, sd_rowvector_exception) {
  using stan::math::sd;
  row_vector_d d;
  row_vector_v v;
  
  EXPECT_THROW(sd(d), std::domain_error);
  EXPECT_THROW(sd(v), std::domain_error);
}
TEST(agrad_matrix, sd_matrix) {
  using stan::math::sd;
  matrix_d v(1,1);
  v << 1.0;
  EXPECT_FLOAT_EQ(0.0, sd(v));

  matrix_d d1(2, 3);
  matrix_v v1(2, 3);
  
  d1 << 1, 2, 3, 4, 5, 6;
  v1 << 1, 2, 3, 4, 5, 6;
  
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(d1));
  EXPECT_FLOAT_EQ(std::sqrt(17.5/5.0), sd(v1).val());

  d1.resize(1, 1);
  v1.resize(1, 1);
  EXPECT_FLOAT_EQ(0.0, sd(d1));
  EXPECT_FLOAT_EQ(0.0, sd(v1).val());
}
TEST(agrad_matrix, sd_matrix_exception) {
  using stan::math::sd;
  matrix_d d;
  matrix_v v;

  EXPECT_THROW(sd(d), std::domain_error);
  EXPECT_THROW(sd(v), std::domain_error);

  d.resize(1, 0);
  v.resize(1, 0);
  EXPECT_THROW(sd(d), std::domain_error);
  EXPECT_THROW(sd(v), std::domain_error);

  d.resize(0, 1);
  v.resize(0, 1);
  EXPECT_THROW(sd(d), std::domain_error);
  EXPECT_THROW(sd(v), std::domain_error);
}
// end sd tests

// sum tests
TEST(agrad_matrix, sum_vector) {
  using stan::math::sum;
  vector_d d(6);
  vector_v v(6);
  
  d << 1, 2, 3, 4, 5, 6;
  v << 1, 2, 3, 4, 5, 6;
  
  var output;
  output = sum(d);
  EXPECT_FLOAT_EQ(21.0, output.val());
                   
  output = sum(v);
  EXPECT_FLOAT_EQ(21.0, output.val());

  d.resize(0);
  v.resize(0);
  EXPECT_FLOAT_EQ(0.0, sum(d));
  EXPECT_FLOAT_EQ(0.0, sum(v).val());
}
TEST(agrad_matrix, sum_rowvector) {
  using stan::math::sum;
  row_vector_d d(6);
  row_vector_v v(6);
  
  d << 1, 2, 3, 4, 5, 6;
  v << 1, 2, 3, 4, 5, 6;
  
  var output;
  output = sum(d);
  EXPECT_FLOAT_EQ(21.0, output.val());
                   
  output = sum(v);
  EXPECT_FLOAT_EQ(21.0, output.val());

  d.resize(0);
  v.resize(0);
  EXPECT_FLOAT_EQ(0.0, sum(d));
  EXPECT_FLOAT_EQ(0.0, sum(v).val());
}
TEST(agrad_matrix, sum_matrix) {
  using stan::math::sum;
  matrix_d d(2, 3);
  matrix_v v(2, 3);
  
  d << 1, 2, 3, 4, 5, 6;
  v << 1, 2, 3, 4, 5, 6;
  
  var output;
  output = sum(d);
  EXPECT_FLOAT_EQ(21.0, output.val());
                   
  output = sum(v);
  EXPECT_FLOAT_EQ(21.0, output.val());

  d.resize(0, 0);
  v.resize(0, 0);
  EXPECT_FLOAT_EQ(0.0, sum(d));
  EXPECT_FLOAT_EQ(0.0, sum(v).val());
}
// end sum tests

// multiply tests
TEST(agrad_matrix, multiply_scalar_scalar) {
  using stan::agrad::multiply;
  using stan::agrad::var;
  double d1, d2;
  var    v1, v2;

  d1 = 10;
  v1 = 10;
  d2 = -2;
  v2 = -2;
  
  EXPECT_FLOAT_EQ(-20.0, multiply(d1,d2));
  EXPECT_FLOAT_EQ(-20.0, multiply(d1, v2).val());
  EXPECT_FLOAT_EQ(-20.0, multiply(v1, d2).val());
  EXPECT_FLOAT_EQ(-20.0, multiply(v1, v2).val());

  EXPECT_FLOAT_EQ(6.0, multiply(var(3),var(2)).val());
  EXPECT_FLOAT_EQ(6.0, multiply(3.0,var(2)).val());
  EXPECT_FLOAT_EQ(6.0, multiply(var(3),2.0).val());

  
}
TEST(agrad_matrix, multiply_vector_scalar) {
  vector_d d1(3);
  vector_v v1(3);
  double d2;
  var v2;
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  d2 = -2;
  v2 = -2;
  
  vector_v output;
  output = multiply(d1, v2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());

  output = multiply(v1, d2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());

  output = multiply(v1, v2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());
}
TEST(agrad_matrix, multiply_rowvector_scalar) {
  row_vector_d d1(3);
  row_vector_v v1(3);
  double d2;
  var v2;
  
  d1 << 100, 0, -3;
  v1 << 100, 0, -3;
  d2 = -2;
  v2 = -2;
  
  row_vector_v output;
  output = multiply(d1, v2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());

  output = multiply(v1, d2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());

  output = multiply(v1, v2);
  EXPECT_FLOAT_EQ(-200, output(0).val());
  EXPECT_FLOAT_EQ(   0, output(1).val());
  EXPECT_FLOAT_EQ(   6, output(2).val());
}
TEST(agrad_matrix, multiply_matrix_scalar) {
  matrix_d d1(2,2);
  matrix_v v1(2,2);
  double d2;
  var v2;
  
  d1 << 100, 0, -3, 4;
  v1 << 100, 0, -3, 4;
  d2 = -2;
  v2 = -2;
  
  matrix_v output;
  output = multiply(d1, v2);
  EXPECT_FLOAT_EQ(-200, output(0,0).val());
  EXPECT_FLOAT_EQ(   0, output(0,1).val());
  EXPECT_FLOAT_EQ(   6, output(1,0).val());
  EXPECT_FLOAT_EQ(  -8, output(1,1).val());

  output = multiply(v1, d2);
  EXPECT_FLOAT_EQ(-200, output(0,0).val());
  EXPECT_FLOAT_EQ(   0, output(0,1).val());
  EXPECT_FLOAT_EQ(   6, output(1,0).val());
  EXPECT_FLOAT_EQ(  -8, output(1,1).val());
 
  output = multiply(v1, v2);
  EXPECT_FLOAT_EQ(-200, output(0,0).val());
  EXPECT_FLOAT_EQ(   0, output(0,1).val());
  EXPECT_FLOAT_EQ(   6, output(1,0).val());
  EXPECT_FLOAT_EQ(  -8, output(1,1).val());
}
TEST(agrad_matrix, multiply_rowvector_vector) {
  row_vector_d d1(3);
  row_vector_v v1(3);
  vector_d d2(3);
  vector_v v2(3);
  
  d1 << 1, 3, -5;
  v1 << 1, 3, -5;
  d2 << 4, -2, -1;
  v2 << 4, -2, -1;

  EXPECT_FLOAT_EQ(3, multiply(v1, v2).val());
  EXPECT_FLOAT_EQ(3, multiply(v1, d2).val());
  EXPECT_FLOAT_EQ(3, multiply(d1, v2).val());
  
  d1.resize(1);
  v1.resize(1);
  EXPECT_THROW(multiply(v1, v2), std::domain_error);
  EXPECT_THROW(multiply(v1, d2), std::domain_error);
  EXPECT_THROW(multiply(d1, v2), std::domain_error);
}
TEST(agrad_matrix, multiply_vector_rowvector) {
  vector_d d1(3);
  vector_v v1(3);
  row_vector_d d2(3);
  row_vector_v v2(3);
  
  d1 << 1, 3, -5;
  v1 << 1, 3, -5;
  d2 << 4, -2, -1;
  v2 << 4, -2, -1;

  matrix_v output = multiply(v1, v2);
  EXPECT_EQ(3, output.rows());
  EXPECT_EQ(3, output.cols());
  EXPECT_FLOAT_EQ(  4, output(0,0).val());
  EXPECT_FLOAT_EQ( -2, output(0,1).val());
  EXPECT_FLOAT_EQ( -1, output(0,2).val());
  EXPECT_FLOAT_EQ( 12, output(1,0).val());
  EXPECT_FLOAT_EQ( -6, output(1,1).val());
  EXPECT_FLOAT_EQ( -3, output(1,2).val());
  EXPECT_FLOAT_EQ(-20, output(2,0).val());
  EXPECT_FLOAT_EQ( 10, output(2,1).val());
  EXPECT_FLOAT_EQ(  5, output(2,2).val());
  
  output = multiply(v1, d2);
  EXPECT_EQ(3, output.rows());
  EXPECT_EQ(3, output.cols());
  EXPECT_FLOAT_EQ(  4, output(0,0).val());
  EXPECT_FLOAT_EQ( -2, output(0,1).val());
  EXPECT_FLOAT_EQ( -1, output(0,2).val());
  EXPECT_FLOAT_EQ( 12, output(1,0).val());
  EXPECT_FLOAT_EQ( -6, output(1,1).val());
  EXPECT_FLOAT_EQ( -3, output(1,2).val());
  EXPECT_FLOAT_EQ(-20, output(2,0).val());
  EXPECT_FLOAT_EQ( 10, output(2,1).val());
  EXPECT_FLOAT_EQ(  5, output(2,2).val());
  
  output = multiply(d1, v2);
  EXPECT_EQ(3, output.rows());
  EXPECT_EQ(3, output.cols());
  EXPECT_FLOAT_EQ(  4, output(0,0).val());
  EXPECT_FLOAT_EQ( -2, output(0,1).val());
  EXPECT_FLOAT_EQ( -1, output(0,2).val());
  EXPECT_FLOAT_EQ( 12, output(1,0).val());
  EXPECT_FLOAT_EQ( -6, output(1,1).val());
  EXPECT_FLOAT_EQ( -3, output(1,2).val());
  EXPECT_FLOAT_EQ(-20, output(2,0).val());
  EXPECT_FLOAT_EQ( 10, output(2,1).val());
  EXPECT_FLOAT_EQ(  5, output(2,2).val());
}
TEST(agrad_matrix, multiply_matrix_vector) {
  matrix_d d1(3,2);
  matrix_v v1(3,2);
  vector_d d2(2);
  vector_v v2(2);
  
  d1 << 1, 3, -5, 4, -2, -1;
  v1 << 1, 3, -5, 4, -2, -1;
  d2 << -2, 4;
  v2 << -2, 4;

  vector_v output = multiply(v1, v2);
  EXPECT_EQ(3, output.size());
  EXPECT_FLOAT_EQ(10, output(0).val());
  EXPECT_FLOAT_EQ(26, output(1).val());
  EXPECT_FLOAT_EQ( 0, output(2).val());

  
  output = multiply(v1, d2);
  EXPECT_EQ(3, output.size());
  EXPECT_FLOAT_EQ(10, output(0).val());
  EXPECT_FLOAT_EQ(26, output(1).val());
  EXPECT_FLOAT_EQ( 0, output(2).val());
  
  output = multiply(d1, v2);
  EXPECT_EQ(3, output.size());
  EXPECT_FLOAT_EQ(10, output(0).val());
  EXPECT_FLOAT_EQ(26, output(1).val());
  EXPECT_FLOAT_EQ( 0, output(2).val());
}
TEST(agrad_matrix, multiply_matrix_vector_exception) {
  matrix_d d1(3,2);
  matrix_v v1(3,2);
  vector_d d2(4);
  vector_v v2(4);
  EXPECT_THROW(multiply(v1, v2), std::domain_error);
  EXPECT_THROW(multiply(v1, d2), std::domain_error);
  EXPECT_THROW(multiply(d1, v2), std::domain_error);
}
TEST(agrad_matrix, multiply_rowvector_matrix) {
  row_vector_d d1(3);
  row_vector_v v1(3);
  matrix_d d2(3,2);
  matrix_v v2(3,2);
  
  d1 << -2, 4, 1;
  v1 << -2, 4, 1;
  d2 << 1, 3, -5, 4, -2, -1;
  v2 << 1, 3, -5, 4, -2, -1;

  vector_v output = multiply(v1, v2);
  EXPECT_EQ(2, output.size());
  EXPECT_FLOAT_EQ(-24, output(0).val());
  EXPECT_FLOAT_EQ(  9, output(1).val());

  output = multiply(v1, d2);
  EXPECT_EQ(2, output.size());
  EXPECT_FLOAT_EQ(-24, output(0).val());
  EXPECT_FLOAT_EQ(  9, output(1).val());
  
  output = multiply(d1, v2);
  EXPECT_EQ(2, output.size());
  EXPECT_FLOAT_EQ(-24, output(0).val());
  EXPECT_FLOAT_EQ(  9, output(1).val());
}
TEST(agrad_matrix, multiply_rowvector_matrix_exception) {
  row_vector_d d1(4);
  row_vector_v v1(4);
  matrix_d d2(3,2);
  matrix_v v2(3,2);
  EXPECT_THROW(multiply(v1, v2), std::domain_error);
  EXPECT_THROW(multiply(v1, d2), std::domain_error);
  EXPECT_THROW(multiply(d1, v2), std::domain_error);
}
TEST(agrad_matrix, multiply_matrix_matrix) {
  matrix_d d1(2,3);
  matrix_v v1(2,3);
  matrix_d d2(3,2);
  matrix_v v2(3,2);
  
  d1 << 9, 24, 3, 46, -9, -33;
  v1 << 9, 24, 3, 46, -9, -33;
  d2 << 1, 3, -5, 4, -2, -1;
  v2 << 1, 3, -5, 4, -2, -1;

  matrix_v output = multiply(v1, v2);
  EXPECT_EQ(2, output.rows());
  EXPECT_EQ(2, output.cols());
  EXPECT_FLOAT_EQ(-117, output(0,0).val());
  EXPECT_FLOAT_EQ( 120, output(0,1).val());
  EXPECT_FLOAT_EQ( 157, output(1,0).val());
  EXPECT_FLOAT_EQ( 135, output(1,1).val());

  output = multiply(v1, d2);
  EXPECT_EQ(2, output.rows());
  EXPECT_EQ(2, output.cols());
  EXPECT_FLOAT_EQ(-117, output(0,0).val());
  EXPECT_FLOAT_EQ( 120, output(0,1).val());
  EXPECT_FLOAT_EQ( 157, output(1,0).val());
  EXPECT_FLOAT_EQ( 135, output(1,1).val());
  
  output = multiply(d1, v2);
  EXPECT_EQ(2, output.rows());
  EXPECT_EQ(2, output.cols());
  EXPECT_FLOAT_EQ(-117, output(0,0).val());
  EXPECT_FLOAT_EQ( 120, output(0,1).val());
  EXPECT_FLOAT_EQ( 157, output(1,0).val());
  EXPECT_FLOAT_EQ( 135, output(1,1).val());
}
TEST(agrad_matrix, multiply_matrix_matrix_exception) {
  matrix_d d1(2,2);
  matrix_v v1(2,2);
  matrix_d d2(3,2);
  matrix_v v2(3,2);

  EXPECT_THROW(multiply(v1, v2), std::domain_error);
  EXPECT_THROW(multiply(v1, d2), std::domain_error);
  EXPECT_THROW(multiply(d1, v2), std::domain_error);
}
// end multiply tests



TEST(agrad_matrix,transpose_matrix) {
  using stan::math::transpose;
  EXPECT_EQ(0,transpose(matrix_v()).size());
  EXPECT_EQ(0,transpose(matrix_d()).size());

  matrix_v a(2,3);
  a << -1.0, 2.0, -3.0, 
        5.0, 10.0, 100.0;
  
  AVEC x = createAVEC(a(0,0), a(0,2), a(1,1));
  
  matrix_v c = transpose(a);
  EXPECT_FLOAT_EQ(-1.0,c(0,0).val());
  EXPECT_FLOAT_EQ(10.0,c(1,1).val());
  EXPECT_FLOAT_EQ(-3.0,c(2,0).val());
  EXPECT_EQ(3,c.rows());
  EXPECT_EQ(2,c.cols());

  VEC g = cgradvec(c(2,0),x);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(1.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
}
TEST(agrad_matrix,transpose_vector) {
  vector_v a(3);
  a << 1.0, 2.0, 3.0;
  
  AVEC x = createAVEC(a(0),a(1),a(2));

  row_vector_v a_tr = transpose(a);
  EXPECT_EQ(a.size(),a_tr.size());
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(a(i).val(),a_tr(i).val());

  VEC g = cgradvec(a_tr(1),x);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(1.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
}
TEST(agrad_matrix,transpose_row_vector) {
  row_vector_v a(3);
  a << 1.0, 2.0, 3.0;
  
  AVEC x = createAVEC(a(0),a(1),a(2));

  vector_v a_tr = transpose(a);
  EXPECT_EQ(a.size(),a_tr.size());
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(a(i).val(),a_tr(i).val());

  VEC g = cgradvec(a_tr(1),x);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(1.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
}


TEST(agrad_matrix,mv_trace) {
  using stan::math::trace;
  matrix_v a(2,2);
  a << -1.0, 2.0, 
       5.0, 10.0;
  
  AVEC x = createAVEC(a(0,0), a(0,1), a(1,0), a(1,1));

  AVAR s = trace(a);
  EXPECT_FLOAT_EQ(9.0,s.val());
  
  VEC g = cgradvec(s,x);
  EXPECT_FLOAT_EQ(1.0, g[0]);
  EXPECT_FLOAT_EQ(0.0, g[1]);
  EXPECT_FLOAT_EQ(0.0, g[2]);
  EXPECT_FLOAT_EQ(1.0, g[3]);
}  


TEST(agrad_matrix,mdivide_left_val) {
  matrix_v Av(2,2);
  matrix_d Ad(2,2);
  matrix_v I;

  Av << 2.0, 3.0, 
        5.0, 7.0;
  Ad << 2.0, 3.0, 
        5.0, 7.0;

  I = mdivide_left(Av,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = mdivide_left(Av,Ad);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = mdivide_left(Ad,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);
}

TEST(agrad_matrix,mdivide_right_val) {
  matrix_v Av(2,2);
  matrix_d Ad(2,2);
  matrix_v I;

  Av << 2.0, 3.0, 
        5.0, 7.0;
  Ad << 2.0, 3.0, 
        5.0, 7.0;

  I = mdivide_right(Av,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = mdivide_right(Av,Ad);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = mdivide_right(Ad,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);
}
TEST(matrixTest,mdivide_left_tri_val) {
  matrix_v Av(2,2);
  matrix_d Ad(2,2);
  matrix_v I;
  
  Av << 2.0, 0.0, 
        5.0, 7.0;
  Ad << 2.0, 0.0, 
        5.0, 7.0;

  I = stan::agrad::mdivide_left_tri<Eigen::Lower>(Av,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = stan::agrad::mdivide_left_tri<Eigen::Lower>(Av,Ad);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = stan::agrad::mdivide_left_tri<Eigen::Lower>(Ad,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  Av << 2.0, 3.0, 
        0.0, 7.0;
  Ad << 2.0, 3.0, 
        0.0, 7.0;

  I = stan::agrad::mdivide_left_tri<Eigen::Upper>(Av,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = stan::agrad::mdivide_left_tri<Eigen::Upper>(Av,Ad);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  I = stan::agrad::mdivide_left_tri<Eigen::Upper>(Ad,Av);
  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);
}
// FIXME:  Fails in g++ 4.2 -- can't find agrad version of mdivide_left_tri
//         Works in clang++ and later g++
// TEST(agrad_matrix,mdivide_left_tri2) {
//   using stan::math::mdivide_left_tri;
//   using stan::agrad::mdivide_left_tri;
//   int k = 3;
//   Eigen::Matrix<stan::agrad::var,Eigen::Dynamic,Eigen::Dynamic> L(k,k);
//   L << 1, 2, 3, 4, 5, 6, 7, 8, 9;
//   Eigen::Matrix<stan::agrad::var,Eigen::Dynamic,Eigen::Dynamic> I(k,k);
//   I.setIdentity();
//   L = mdivide_left_tri<Eigen::Lower>(L, I);
// }
TEST(agrad_matrix,inverse_val) {
  using stan::math::inverse;
  matrix_v a(2,2);
  a << 2.0, 3.0, 
       5.0, 7.0;

  matrix_v a_inv = inverse(a);

  matrix_v I = multiply(a,a_inv);

  EXPECT_NEAR(1.0,I(0,0).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(0,1).val(),1.0E-12);
  EXPECT_NEAR(0.0,I(1,0).val(),1.0E-12);
  EXPECT_NEAR(1.0,I(1,1).val(),1.0e-12);

  EXPECT_THROW(inverse(matrix_v(2,3)), std::domain_error);
}
TEST(agrad_matrix,inverse_grad) {
  using stan::math::inverse;
  
  for (size_t k = 0; k < 2; ++k) {
    for (size_t l = 0; l < 2; ++l) {

      matrix_v ad(2,2);
      ad << 2.0, 3.0, 
        5.0, 7.0;

      AVEC x = createAVEC(ad(0,0),ad(0,1),ad(1,0),ad(1,1));

      matrix_v ad_inv = inverse(ad);

      // int k = 0;
      // int l = 1;
      std::vector<double> g;
      ad_inv(k,l).grad(x,g);

      int idx = 0;
      for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
          EXPECT_FLOAT_EQ(-ad_inv(k,i).val() * ad_inv(j,l).val(), g[idx]);
          ++idx;
        }
      }
    }
  }
}
TEST(agrad_matrix,inverse_inverse_sum) {
  using stan::math::sum;
  using stan::math::inverse;

  matrix_v a(4,4);
  a << 2.0, 3.0, 4.0, 5.0, 
    9.0, -1.0, 2.0, 2.0,
    4.0, 3.0, 7.0, -1.0,
    0.0, 1.0, 19.0, 112.0;

  AVEC x;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      x.push_back(a(i,j));

  AVAR a_inv_inv_sum = sum(inverse(inverse(a)));

  VEC g = cgradvec(a_inv_inv_sum,x);

  for (size_t k = 0; k < x.size(); ++k)
    EXPECT_FLOAT_EQ(1.0,g[k]);
}



TEST(agrad_matrix,eigenval_sum) {
  using stan::math::eigenvalues;
  using stan::math::sum;

  EXPECT_THROW(eigenvalues(matrix_v(3,2)), std::domain_error);

  matrix_v a(3,3);
  a << 1.0, 2.0, 3.0, 5.0, 7.0, 9.0, 13.0, 11.0, 19.0;
  AVEC x = createAVEC(a(0,0), a(1,1), a(2,2), a(1,2));
  x.push_back(a(0,1));
  x.push_back(a(2,0));

  // grad sum eig = I
  vector_v a_eigenvalues = eigenvalues(a);
  
  AVAR sum_a_eigenvalues = sum(a_eigenvalues);
  
  VEC g = cgradvec(sum_a_eigenvalues,x);

  EXPECT_NEAR(1.0,g[0],1.0E-11);
  EXPECT_NEAR(1.0,g[1],1.0E-11);
  EXPECT_NEAR(1.0,g[2],1.0E-11);

  EXPECT_NEAR(0.0,g[3],1.0E-10);
  EXPECT_NEAR(0.0,g[4],1.0E-10);
  EXPECT_NEAR(0.0,g[5],1.0E-10);
}

TEST(agrad_matrix,mat_cholesky) {
  // symmetric
  matrix_v X(2,2);
  AVAR a = 3.0;
  AVAR b = -1.0;
  AVAR c = -1.0;
  AVAR d = 1.0;
  X << a, b, 
       c, d;
  
  matrix_v L = cholesky_decompose(X);

  matrix_v LL_trans = multiply(L,transpose(L));
  EXPECT_FLOAT_EQ(a.val(),LL_trans(0,0).val());
  EXPECT_FLOAT_EQ(b.val(),LL_trans(0,1).val());
  EXPECT_FLOAT_EQ(c.val(),LL_trans(1,0).val());
  EXPECT_FLOAT_EQ(d.val(),LL_trans(1,1).val());
}


// norm tests for raw calls; move into promotion lib
TEST(agrad_matrix,mv_squaredNorm) {
  matrix_v a(2,2);
  a << -1.0, 2.0, 
       5.0, 10.0;
  
  AVEC x = createAVEC(a(0,0), a(0,1), a(1,0), a(1,1));

  AVAR s = a.squaredNorm();
  EXPECT_FLOAT_EQ(130.0,s.val());
  
  VEC g = cgradvec(s,x);
  EXPECT_FLOAT_EQ(-2.0, g[0]);
  EXPECT_FLOAT_EQ(4.0, g[1]);
  EXPECT_FLOAT_EQ(10.0, g[2]);
  EXPECT_FLOAT_EQ(20.0, g[3]);
}  
TEST(agrad_matrix,mv_norm) {
  matrix_v a(2,1);
  a << -3.0, 4.0;
  
  AVEC x = createAVEC(a(0,0), a(1,0));

  AVAR s = a.norm();
  EXPECT_FLOAT_EQ(5.0,s.val());

  // (see hypot in special_functions_test) 
  VEC g = cgradvec(s,x);
  EXPECT_FLOAT_EQ(-3.0/5.0, g[0]);
  EXPECT_FLOAT_EQ(4.0/5.0, g[1]);
}  
TEST(agrad_matrix,mv_lp_norm) {
  matrix_v a(2,2);
  a << -1.0, 2.0, 
    5.0, 0.0;
  
  AVEC x = createAVEC(a(0,0), a(0,1), a(1,0), a(1,1));

  AVAR s = a.lpNorm<1>();
  EXPECT_FLOAT_EQ(8.0,s.val());
  
  VEC g = cgradvec(s,x);
  EXPECT_FLOAT_EQ(-1.0,g[0]);
  EXPECT_FLOAT_EQ(1.0,g[1]);
  EXPECT_FLOAT_EQ(1.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]); // ? depends on impl here, could be -1 or 1
}  
TEST(agrad_matrix,mv_lp_norm_inf) {
  matrix_v a(2,2);
  a << -1.0, 2.0, 
    -5.0, 0.0;
  
  AVEC x = createAVEC(a(0,0), a(0,1), a(1,0), a(1,1));

  AVAR s = a.lpNorm<Eigen::Infinity>();
  EXPECT_FLOAT_EQ(5.0,s.val());
  
  VEC g = cgradvec(s,x);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(-1.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]); 
}  

TEST(agradMatrix,multiply_scalar_vector_cv) {
  using stan::agrad::multiply;
  vector_v x(3);
  x << 1, 2, 3;
  AVEC x_ind = createAVEC(x(0),x(1),x(2));
  vector_v y = multiply(2.0,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(4.0,y(1).val());
  EXPECT_FLOAT_EQ(6.0,y(2).val());

  VEC g = cgradvec(y(0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
}
TEST(agradMatrix,multiply_scalar_vector_vv) {
  using stan::agrad::multiply;
  vector_v x(3);
  x << 1, 4, 9;
  AVAR two = 2.0;
  AVEC x_ind = createAVEC(x(0),x(1),x(2),two);
  vector_v y = multiply(two,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(8.0,y(1).val());
  EXPECT_FLOAT_EQ(18.0,y(2).val());

  VEC g = cgradvec(y(1),x_ind);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(2.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
  EXPECT_FLOAT_EQ(4.0,g[3]);
}
TEST(agradMatrix,multiply_scalar_vector_vc) {
  using stan::agrad::multiply;
  vector_v x(3);
  x << 1, 2, 3;
  AVAR two = 2.0;
  AVEC x_ind = createAVEC(two);
  vector_v y = multiply(two,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(4.0,y(1).val());
  EXPECT_FLOAT_EQ(6.0,y(2).val());

  VEC g = cgradvec(y(2),x_ind);
  EXPECT_FLOAT_EQ(3.0,g[0]);
}

TEST(agradMatrix,multiply_scalar_row_vector_cv) {
  using stan::agrad::multiply;
  row_vector_v x(3);
  x << 1, 2, 3;
  AVEC x_ind = createAVEC(x(0),x(1),x(2));
  row_vector_v y = multiply(2.0,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(4.0,y(1).val());
  EXPECT_FLOAT_EQ(6.0,y(2).val());

  VEC g = cgradvec(y(0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
}
TEST(agradMatrix,multiply_scalar_row_vector_vv) {
  using stan::agrad::multiply;
  row_vector_v x(3);
  x << 1, 4, 9;
  AVAR two = 2.0;
  AVEC x_ind = createAVEC(x(0),x(1),x(2),two);
  row_vector_v y = multiply(two,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(8.0,y(1).val());
  EXPECT_FLOAT_EQ(18.0,y(2).val());

  VEC g = cgradvec(y(1),x_ind);
  EXPECT_FLOAT_EQ(0.0,g[0]);
  EXPECT_FLOAT_EQ(2.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
  EXPECT_FLOAT_EQ(4.0,g[3]);
}
TEST(agradMatrix,multiply_scalar_row_vector_vc) {
  using stan::agrad::multiply;
  row_vector_v x(3);
  x << 1, 2, 3;
  AVAR two = 2.0;
  AVEC x_ind = createAVEC(two);
  row_vector_v y = multiply(two,x);
  EXPECT_FLOAT_EQ(2.0,y(0).val());
  EXPECT_FLOAT_EQ(4.0,y(1).val());
  EXPECT_FLOAT_EQ(6.0,y(2).val());

  VEC g = cgradvec(y(2),x_ind);
  EXPECT_FLOAT_EQ(3.0,g[0]);
}

TEST(agradMatrix,multiply_scalar_matrix_cv) {
  using stan::agrad::multiply;
  matrix_v x(2,3);
  x << 1, 2, 3, 4, 5, 6;
  AVEC x_ind = createAVEC(x(0,0),x(0,1),x(0,2),x(1,0));
  matrix_v y = multiply(2.0,x);
  EXPECT_FLOAT_EQ(2.0,y(0,0).val());
  EXPECT_FLOAT_EQ(4.0,y(0,1).val());
  EXPECT_FLOAT_EQ(6.0,y(0,2).val());

  VEC g = cgradvec(y(0,0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}

TEST(agradMatrix,multiply_scalar_matrix_vc) {
  using stan::agrad::multiply;
  matrix_d x(2,3);
  x << 1, 2, 3, 4, 5, 6;
  AVAR two = 2.0;
  AVEC x_ind = createAVEC(two);

  matrix_v y = multiply(two,x);
  EXPECT_FLOAT_EQ(2.0,y(0,0).val());
  EXPECT_FLOAT_EQ(4.0,y(0,1).val());
  EXPECT_FLOAT_EQ(6.0,y(0,2).val());

  VEC g = cgradvec(y(1,0),x_ind);
  EXPECT_FLOAT_EQ(4.0,g[0]);
}

TEST(agradMatrix,elt_multiply_vec_vv) {
  using stan::agrad::elt_multiply;
  vector_v x(2);
  x << 2, 5;
  vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1),y(0),y(1));
  vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(2.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_multiply_vec_vd) {
  using stan::agrad::elt_multiply;
  vector_v x(2);
  x << 2, 5;
  vector_d y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1));
  vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,elt_multiply_vec_dv) {
  using stan::agrad::elt_multiply;
  vector_d x(2);
  x << 2, 5;
  vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(y(0),y(1));
  vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}

TEST(agradMatrix,elt_multiply_row_vec_vv) {
  using stan::agrad::elt_multiply;
  row_vector_v x(2);
  x << 2, 5;
  row_vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1),y(0),y(1));
  row_vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(2.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_multiply_row_vec_vd) {
  using stan::agrad::elt_multiply;
  row_vector_v x(2);
  x << 2, 5;
  row_vector_d y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1));
  row_vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,elt_multiply_row_vec_dv) {
  using stan::agrad::elt_multiply;
  row_vector_d x(2);
  x << 2, 5;
  row_vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(y(0),y(1));
  row_vector_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0).val());
  EXPECT_FLOAT_EQ(500.0,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}


TEST(agradMatrix,elt_multiply_matrix_vv) {
  using stan::agrad::elt_multiply;
  matrix_v x(2,3);
  x << 2, 5, 6, 9, 13, 29;
  matrix_v y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(x(0,0),x(0,1),x(0,2),y(0,0));
  matrix_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0,0).val());
  EXPECT_FLOAT_EQ(500.0,z(0,1).val());
  EXPECT_FLOAT_EQ(29000000.0,z(1,2).val());

  VEC g = cgradvec(z(0,0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
  EXPECT_FLOAT_EQ(2.0,g[3]);
}
TEST(agradMatrix,elt_multiply_matrix_vd) {
  using stan::agrad::elt_multiply;
  matrix_v x(2,3);
  x << 2, 5, 6, 9, 13, 29;
  matrix_d y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(x(0,0),x(0,1),x(0,2),x(1,0));
  matrix_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0,0).val());
  EXPECT_FLOAT_EQ(500.0,z(0,1).val());
  EXPECT_FLOAT_EQ(29000000.0,z(1,2).val());

  VEC g = cgradvec(z(0,0),x_ind);
  EXPECT_FLOAT_EQ(10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(0.0,g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_multiply_matrix_dv) {
  using stan::agrad::elt_multiply;
  matrix_d x(2,3);
  x << 2, 5, 6, 9, 13, 29;
  matrix_v y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(y(0,0),y(0,1));
  matrix_v z = elt_multiply(x,y);
  EXPECT_FLOAT_EQ(20.0,z(0,0).val());
  EXPECT_FLOAT_EQ(500.0,z(0,1).val());
  EXPECT_FLOAT_EQ(29000000.0,z(1,2).val());

  VEC g = cgradvec(z(0,0),x_ind);
  EXPECT_FLOAT_EQ(2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}

TEST(agradMatrix,elt_divide_vec_vv) {
  using stan::agrad::elt_divide;
  vector_v x(2);
  x << 2, 5;
  vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1),y(0),y(1));
  vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_divide_vec_vd) {
  using stan::math::elt_divide;
  vector_v x(2);
  x << 2, 5;
  vector_d y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1));
  vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,elt_divide_vec_dv) {
  using stan::math::elt_divide;
  vector_d x(2);
  x << 2, 5;
  vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(y(0),y(1));
  vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}

TEST(agradMatrix,elt_divide_rowvec_vv) {
  using stan::agrad::elt_divide;
  row_vector_v x(2);
  x << 2, 5;
  row_vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1),y(0),y(1));
  row_vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_divide_rowvec_vd) {
  using stan::agrad::elt_divide;
  row_vector_v x(2);
  x << 2, 5;
  row_vector_d y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(x(0),x(1));
  row_vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,elt_divide_rowvec_dv) {
  using stan::agrad::elt_divide;
  row_vector_d x(2);
  x << 2, 5;
  row_vector_v y(2);
  y << 10, 100;
  AVEC x_ind = createAVEC(y(0),y(1));
  row_vector_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0).val());
  EXPECT_FLOAT_EQ(0.05,z(1).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}


TEST(agradMatrix,elt_divide_mat_vv) {
  using stan::agrad::elt_divide;
  matrix_v x(2,3);
  x << 2, 5, 7, 13, 29, 112;
  matrix_v y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(x(0,0),x(0,1),y(0,0),y(0,1));
  matrix_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0,0).val());
  EXPECT_FLOAT_EQ(0.05,z(0,1).val());
  EXPECT_FLOAT_EQ(112.0/1000000.0,z(1,2).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[2]);
  EXPECT_FLOAT_EQ(0.0,g[3]);
}
TEST(agradMatrix,elt_divide_mat_vd) {
  using stan::agrad::elt_divide;
  matrix_v x(2,3);
  x << 2, 5, 7, 13, 29, 112;
  matrix_d y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(x(0,0),x(0,1));
  matrix_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0,0).val());
  EXPECT_FLOAT_EQ(0.05,z(0,1).val());
  EXPECT_FLOAT_EQ(112.0/1000000.0,z(1,2).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(1.0/10.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,elt_divide_mat_dv) {
  using stan::agrad::elt_divide;
  matrix_d x(2,3);
  x << 2, 5, 7, 13, 29, 112;
  matrix_v y(2,3);
  y << 10, 100, 1000, 10000, 100000, 1000000;
  AVEC x_ind = createAVEC(y(0,0),y(0,1));
  matrix_v z = elt_divide(x,y);
  EXPECT_FLOAT_EQ(0.2,z(0,0).val());
  EXPECT_FLOAT_EQ(0.05,z(0,1).val());
  EXPECT_FLOAT_EQ(112.0/1000000.0,z(1,2).val());

  VEC g = cgradvec(z(0),x_ind);
  EXPECT_FLOAT_EQ(2.0 / (- 10.0 * 10.0), g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
}
TEST(agradMatrix,col_v) {
  using stan::agrad::col;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  vector_v z = col(y,1);
  EXPECT_EQ(2,z.size());
  EXPECT_FLOAT_EQ(1.0,z[0].val());
  EXPECT_FLOAT_EQ(4.0,z[1].val());

  vector_v w = col(y,2);
  EXPECT_EQ(2,w.size());
  EXPECT_EQ(2.0,w[0].val());
  EXPECT_EQ(5.0,w[1].val());
}
TEST(agradMatrix,col_v_exc0) {
  using stan::agrad::col;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  EXPECT_THROW(col(y,0),std::domain_error);
  EXPECT_THROW(col(y,7),std::domain_error);
}
TEST(agradMatrix,col_v_excHigh) {
  using stan::agrad::col;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  EXPECT_THROW(col(y,0),std::domain_error);
  EXPECT_THROW(col(y,5),std::domain_error);
}
TEST(agradMatrix,row_v) {
  using stan::agrad::row;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  vector_v z = row(y,1);
  EXPECT_EQ(3,z.size());
  EXPECT_FLOAT_EQ(1.0,z[0].val());
  EXPECT_FLOAT_EQ(2.0,z[1].val());
  EXPECT_FLOAT_EQ(3.0,z[2].val());

  vector_v w = row(y,2);
  EXPECT_EQ(3,w.size());
  EXPECT_EQ(4.0,w[0].val());
  EXPECT_EQ(5.0,w[1].val());
  EXPECT_EQ(6.0,w[2].val());
}
TEST(agradMatrix,row_v_exc0) {
  using stan::agrad::row;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  EXPECT_THROW(row(y,0),std::domain_error);
  EXPECT_THROW(row(y,7),std::domain_error);
}
TEST(agradMatrix,row_v_excHigh) {
  using stan::agrad::row;
  matrix_v y(2,3);
  y << 1, 2, 3, 4, 5, 6;
  EXPECT_THROW(row(y,0),std::domain_error);
  EXPECT_THROW(row(y,5),std::domain_error);
}
TEST(agradMatrix, dot_product_vv) {
  std::vector<var> a, b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(&a[0], &b[0], 3);
  EXPECT_EQ(2, c);
  std::vector<var> ab;
  std::vector<double> grad;
  for (size_t i = 0; i < 3; i++) {
    ab.push_back(a[i]);
    ab.push_back(b[i]);
  }
  c.grad(ab, grad);
  EXPECT_EQ(grad[0], 1);
  EXPECT_EQ(grad[1], -1);
  EXPECT_EQ(grad[2], 2);
  EXPECT_EQ(grad[3], 0);
  EXPECT_EQ(grad[4], 3);
  EXPECT_EQ(grad[5], 1);
}
TEST(agradMatrix, dot_product_dv) {
  std::vector<double> a;
  std::vector<var> b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(&a[0], &b[0], 3);
  EXPECT_EQ(2, c);
  std::vector<double> grad;
  c.grad(b, grad);
  EXPECT_EQ(grad[0], -1);
  EXPECT_EQ(grad[1], 0);
  EXPECT_EQ(grad[2], 1);
}
TEST(agradMatrix, dot_product_vd) {
  std::vector<var> a;
  std::vector<double> b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(&a[0], &b[0], 3);
  EXPECT_EQ(2, c);
  std::vector<double> grad;
  c.grad(a, grad);
  EXPECT_EQ(grad[0], 1);
  EXPECT_EQ(grad[1], 2);
  EXPECT_EQ(grad[2], 3);
}
TEST(agradMatrix, dot_product_vv_vec) {
  std::vector<var> a, b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(a, b);
  EXPECT_EQ(2, c);
  std::vector<var> ab;
  std::vector<double> grad;
  for (size_t i = 0; i < 3; i++) {
    ab.push_back(a[i]);
    ab.push_back(b[i]);
  }
  c.grad(ab, grad);
  EXPECT_EQ(grad[0], 1);
  EXPECT_EQ(grad[1], -1);
  EXPECT_EQ(grad[2], 2);
  EXPECT_EQ(grad[3], 0);
  EXPECT_EQ(grad[4], 3);
  EXPECT_EQ(grad[5], 1);
}
TEST(agradMatrix, dot_product_dv_vec) {
  std::vector<double> a;
  std::vector<var> b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(a, b);
  EXPECT_EQ(2, c);
  std::vector<double> grad;
  c.grad(b, grad);
  EXPECT_EQ(grad[0], -1);
  EXPECT_EQ(grad[1], 0);
  EXPECT_EQ(grad[2], 1);
}
TEST(agradMatrix, dot_product_vd_vec) {
  std::vector<var> a;
  std::vector<double> b;
  var c;
  for (int i = -1; i < 2; i++) { // a = (-1, 0, 1), b = (1, 2, 3)
    a.push_back(i);
    b.push_back(i + 2);
  }
  c = dot_product(a, b);
  EXPECT_EQ(2, c);
  std::vector<double> grad;
  c.grad(a, grad);
  EXPECT_EQ(grad[0], 1);
  EXPECT_EQ(grad[1], 2);
  EXPECT_EQ(grad[2], 3);
}

template <int R, int C>
void assert_val_grad(Eigen::Matrix<stan::agrad::var,R,C>& v) {
  v << -1.0, 0.0, 3.0;
  AVEC x = createAVEC(v(0),v(1),v(2));
  AVAR f = dot_self(v);
  std::vector<double> g;
  f.grad(x,g);
  
  EXPECT_FLOAT_EQ(-2.0,g[0]);
  EXPECT_FLOAT_EQ(0.0,g[1]);
  EXPECT_FLOAT_EQ(6.0,g[2]);
}  


TEST(agradMatrix, dot_self_vec) {
  using stan::agrad::var;
  using stan::math::dot_self;

  Eigen::Matrix<var,Eigen::Dynamic,1> v1(1);
  v1 << 2.0;
  EXPECT_NEAR(4.0,dot_self(v1).val(),1E-12);
  Eigen::Matrix<var,Eigen::Dynamic,1> v2(2);
  v2 << 2.0, 3.0;
  EXPECT_NEAR(13.0,dot_self(v2).val(),1E-12);
  Eigen::Matrix<var,Eigen::Dynamic,1> v3(3);
  v3 << 2.0, 3.0, 4.0;
  EXPECT_NEAR(29.0,dot_self(v3).val(),1E-12);  

  Eigen::Matrix<var,Eigen::Dynamic,1> v(3);
  assert_val_grad(v);

  Eigen::Matrix<var,1,Eigen::Dynamic> vv(3);
  assert_val_grad(vv);

  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> vvv(3,1);
  assert_val_grad(vvv);

  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> vvvv(1,3);
  assert_val_grad(vvvv);
}

TEST(agradMatrix,columns_dot_self) {
  using stan::agrad::var;
  using stan::math::columns_dot_self;

  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> m1(1,1);
  m1 << 2.0;
  EXPECT_NEAR(4.0,columns_dot_self(m1)(0,0).val(),1E-12);
  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> m2(1,2);
  m2 << 2.0, 3.0;
  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> x;
  x = columns_dot_self(m2);
  EXPECT_NEAR(4.0,x(0,0).val(),1E-12);
  EXPECT_NEAR(9.0,x(1,0).val(),1E-12);
  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> m3(2,2);
  m3 << 2.0, 3.0, 4.0, 5.0;
  x = columns_dot_self(m3);
  EXPECT_NEAR(20.0,x(0,0).val(),1E-12);
  EXPECT_NEAR(34.0,x(1,0).val(),1E-12);

  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> vvv(3,1);
  assert_val_grad(vvv);

  Eigen::Matrix<var,Eigen::Dynamic,Eigen::Dynamic> vvvv(1,3);
  assert_val_grad(vvvv);
}

TEST(MathMatrix,softmax) {
  using stan::agrad::var;
  using stan::math::softmax;
  using Eigen::Matrix;
  using Eigen::Dynamic;

  EXPECT_THROW(softmax(vector_v()),std::domain_error);
  
  Matrix<var,Dynamic,1> x(1);
  x << 0.0;
  
  Matrix<var,Dynamic,1> theta = softmax(x);
  EXPECT_EQ(1,theta.size());
  EXPECT_FLOAT_EQ(1.0,theta[0].val());

  Matrix<var,Dynamic,1> x2(2);
  x2 << -1.0, 1.0;
  Matrix<var,Dynamic,1> theta2 = softmax(x2);
  EXPECT_EQ(2,theta2.size());
  EXPECT_FLOAT_EQ(exp(-1)/(exp(-1) + exp(1)), theta2[0].val());
  EXPECT_FLOAT_EQ(exp(1)/(exp(-1) + exp(1)), theta2[1].val());

  Matrix<var,Dynamic,1> x3(3);
  x3 << -1.0, 1.0, 10.0;
  Matrix<var,Dynamic,1> theta3 = softmax(x3);
  EXPECT_EQ(3,theta3.size());
  EXPECT_FLOAT_EQ(exp(-1)/(exp(-1) + exp(1) + exp(10.0)), theta3[0].val());
  EXPECT_FLOAT_EQ(exp(1)/(exp(-1) + exp(1) + exp(10.0)), theta3[1].val());
  EXPECT_FLOAT_EQ(exp(10)/(exp(-1) + exp(1) + exp(10.0)), theta3[2].val());
}
TEST(AgradMatrix, meanStdVector) {
  using stan::math::mean; // should use arg-dep lookup
  using std::vector;
  using stan::agrad::var;
  vector<var> x(0);
  EXPECT_THROW(mean(x), std::domain_error);
  x.push_back(1.0);
  EXPECT_FLOAT_EQ(1.0, mean(x).val());
  x.push_back(2.0);
  EXPECT_FLOAT_EQ(1.5, mean(x).val());

  vector<var> y = createAVEC(1.0,2.0);
  var f = mean(y);
  vector<double> grad = cgrad(f, y[0], y[1]);
  EXPECT_FLOAT_EQ(0.5, grad[0]);
  EXPECT_FLOAT_EQ(0.5, grad[1]);
  EXPECT_EQ(2U, grad.size());
}
TEST(AgradMatrix, varianceStdVector) {
  using stan::math::variance; // should use arg-dep lookup
  using std::vector;
  using stan::agrad::var;

  vector<var> y1 = createAVEC(0.5,2.0,3.5);
  var f1 = variance(y1);
  vector<double> grad1 = cgrad(f1, y1[0], y1[1], y1[2]);
  double f1_val = f1.val(); // save before cleaned out

  vector<var> y2 = createAVEC(0.5,2.0,3.5);
  var mean2 = (y2[0] + y2[1] + y2[2]) / 3.0;
  var sum_sq_diff_2 
    = (y2[0] - mean2) * (y2[0] - mean2)
    + (y2[1] - mean2) * (y2[1] - mean2)
    + (y2[2] - mean2) * (y2[2] - mean2); 
  var f2 = sum_sq_diff_2 / (3 - 1);

  EXPECT_EQ(f2.val(), f1_val);

  vector<double> grad2 = cgrad(f2, y2[0], y2[1], y2[2]);

  EXPECT_EQ(3U, grad1.size());
  EXPECT_EQ(3U, grad2.size());
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(grad2[i], grad1[i]);
}
TEST(AgradMatrix, sdStdVector) {
  using stan::math::sd; // should use arg-dep lookup (and for sqrt)
  using std::vector;
  using stan::agrad::var;

  vector<var> y1 = createAVEC(0.5,2.0,3.5);
  var f1 = sd(y1);
  vector<double> grad1 = cgrad(f1, y1[0], y1[1], y1[2]);
  double f1_val = f1.val(); // save before cleaned out

  vector<var> y2 = createAVEC(0.5,2.0,3.5);
  var mean2 = (y2[0] + y2[1] + y2[2]) / 3.0;
  var sum_sq_diff_2 
    = (y2[0] - mean2) * (y2[0] - mean2)
    + (y2[1] - mean2) * (y2[1] - mean2)
    + (y2[2] - mean2) * (y2[2] - mean2); 
  var f2 = sqrt(sum_sq_diff_2 / (3 - 1));

  EXPECT_EQ(f2.val(), f1_val);

  vector<double> grad2 = cgrad(f2, y2[0], y2[1], y2[2]);

  EXPECT_EQ(3U, grad1.size());
  EXPECT_EQ(3U, grad2.size());
  for (size_t i = 0; i < 3; ++i)
    EXPECT_FLOAT_EQ(grad2[i], grad1[i]);
}


TEST(AgradMatrix, initializeVariable) {
  using stan::agrad::initialize_variable;
  using std::vector;
  using stan::agrad::var;

  using Eigen::Matrix;
  using Eigen::Dynamic;

  var a;
  initialize_variable(a, var(1.0));
  EXPECT_FLOAT_EQ(1.0, a.val());

  vector<var> b(3);
  initialize_variable(b, var(2.0));
  EXPECT_EQ(3U,b.size());
  EXPECT_FLOAT_EQ(2.0, b[0].val());
  EXPECT_FLOAT_EQ(2.0, b[1].val());
  EXPECT_FLOAT_EQ(2.0, b[2].val());

  vector<vector<var> > c(4,vector<var>(3));
  initialize_variable(c, var(3.0));
  for (size_t m = 0; m < c.size(); ++m)
    for (size_t n = 0; n < c[0].size(); ++n)
      EXPECT_FLOAT_EQ(3.0,c[m][n].val());

  Matrix<var, Dynamic, Dynamic> aa(5,7);
  initialize_variable(aa, var(4.0));
  for (int m = 0; m < aa.rows(); ++m)
    for (int n = 0; n < aa.cols(); ++n)
      EXPECT_FLOAT_EQ(4.0, aa(m,n).val());

  Matrix<var, Dynamic, 1> bb(5);
  initialize_variable(bb, var(5.0));
  for (int m = 0; m < bb.size(); ++m) 
      EXPECT_FLOAT_EQ(5.0, bb(m).val());

  Matrix<var,1,Dynamic> cc(12);
  initialize_variable(cc, var(7.0));
  for (int m = 0; m < cc.size(); ++m) 
    EXPECT_FLOAT_EQ(7.0, cc(m).val());
  
  Matrix<var,Dynamic,Dynamic> init_val(3,4);
  vector<Matrix<var,Dynamic,Dynamic> > dd(5, init_val);
  initialize_variable(dd, var(11.0));
  for (size_t i = 0; i < dd.size(); ++i)
    for (int m = 0; m < dd[0].rows(); ++m)
      for (int n = 0; n < dd[0].cols(); ++n)
        EXPECT_FLOAT_EQ(11.0, dd[i](m,n).val());
}

TEST(AgradMatrix, assign) {
  using stan::agrad::assign;
  using stan::agrad::var;
  using std::vector;
  using Eigen::Matrix;
  using Eigen::Dynamic;
  
  var x;
  assign(x,2.0);
  EXPECT_FLOAT_EQ(2.0,x.val());

  assign(x,2);
  EXPECT_FLOAT_EQ(2.0,x.val());

  var y(3.0);
  assign(x,y);
  EXPECT_FLOAT_EQ(3.0,x.val());

  double xd;
  assign(xd,2.0);
  EXPECT_FLOAT_EQ(2.0,xd);

  assign(xd,2);
  EXPECT_FLOAT_EQ(2.0,xd);

  int iii;
  assign(iii,2);
  EXPECT_EQ(2,iii);

  unsigned int j = 12;
  assign(iii,j);
  EXPECT_EQ(12,j);

  vector<double> y_dbl(2);
  y_dbl[0] = 2.0;
  y_dbl[1] = 3.0;
  
  vector<var> y_var(2);
  assign(y_var,y_dbl);
  EXPECT_FLOAT_EQ(2.0,y_var[0].val());
  EXPECT_FLOAT_EQ(3.0,y_var[1].val());

  Matrix<double,Dynamic,1> v_dbl(6);
  v_dbl << 1,2,3,4,5,6;
  Matrix<var,Dynamic,1> v_var(6);
  assign(v_var,v_dbl);
  EXPECT_FLOAT_EQ(1,v_var(0).val());
  EXPECT_FLOAT_EQ(6,v_var(5).val());

  Matrix<double,1,Dynamic> rv_dbl(3);
  rv_dbl << 2, 4, 6;
  Matrix<var,1,Dynamic> rv_var(3);
  assign(rv_var,rv_dbl);
  EXPECT_FLOAT_EQ(2,rv_var(0).val());
  EXPECT_FLOAT_EQ(4,rv_var(1).val());
  EXPECT_FLOAT_EQ(6,rv_var(2).val());

  Matrix<double,Dynamic,Dynamic> m_dbl(2,3);
  m_dbl << 2, 4, 6, 100, 200, 300;
  Matrix<var,Dynamic,Dynamic> m_var(2,3);
  assign(m_var,m_dbl);
  EXPECT_EQ(2,m_var.rows());
  EXPECT_EQ(3,m_var.cols());
  EXPECT_FLOAT_EQ(2,m_var(0,0).val());
  EXPECT_FLOAT_EQ(100,m_var(1,0).val());
  EXPECT_FLOAT_EQ(300,m_var(1,2).val());
}

TEST(AgradMatrix, UserCase1) {
  using std::vector;
  using stan::math::multiply;
  using stan::math::transpose;
  using stan::math::subtract;
  using stan::math::get_base1;
  using stan::agrad::assign;
  using stan::math::dot_product;

  // also tried DpKm1 > H
  size_t H = 3;
  size_t DpKm1 = 3;

  vector_v vk(DpKm1);
  for (size_t k = 0; k < DpKm1; ++k)
    vk[k] = (k + 1) * (k + 2);
  
  matrix_v L_etaprec(DpKm1,DpKm1);
  for (size_t m = 0; m < DpKm1; ++m)
    for (size_t n = 0; n < DpKm1; ++n)
      L_etaprec(m,n) = (m + 1) * (n + 1);

  vector_d etamu(DpKm1);
  for (size_t k = 0; k < DpKm1; ++k)
    etamu[k] = 10 + (k * k);
  
  vector<vector_d> eta(H,vector_d(DpKm1));
  for (size_t h = 0; h < H; ++h)
    for (size_t k = 0; k < DpKm1; ++k)
      eta[h][k] = (h + 1) * (k + 10);

  var lp__ = 0.0;

  for (size_t h = 1; h <= H; ++h) {
    assign(vk, multiply(transpose(L_etaprec),
                        subtract(get_base1(eta,h,"eta",1),
                                 etamu)));
    assign(lp__, (lp__ - (0.5 * dot_product(vk,vk))));
  }

  EXPECT_TRUE(lp__.val() != 0);
}

TEST(AgradMatrix,prod) {
  using stan::math::prod;
  vector_d vd;
  vector_v vv;
  EXPECT_FLOAT_EQ(1.0,prod(vd));
  EXPECT_FLOAT_EQ(1.0,prod(vv).val());

  vd = vector_d(1);
  vv = vector_v(1);
  vd << 2.0;
  vv << 2.0;
  EXPECT_FLOAT_EQ(2.0,prod(vd));
  EXPECT_FLOAT_EQ(2.0,prod(vv).val());

  vd = vector_d(2);
  vd << 2.0, 3.0;
  vv = vector_v(2);
  vv << 2.0, 3.0;
  std::vector<var> x(2);
  x[0] = vv[0];
  x[1] = vv[1];
  var f = prod(vv);
  EXPECT_FLOAT_EQ(6.0,prod(vd));
  EXPECT_FLOAT_EQ(6.0,f.val());
  std::vector<double> g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(3.0,g[0]);
  EXPECT_FLOAT_EQ(2.0,g[1]);
  
}
TEST(AgradMatrix,diagMatrix) {
  using stan::math::diag_matrix;
  EXPECT_EQ(0,diag_matrix(vector_v()).size());
  EXPECT_EQ(4,diag_matrix(vector_v(2)).size());
  EXPECT_EQ(0,diag_matrix(vector_d()).size());
  EXPECT_EQ(4,diag_matrix(vector_d(2)).size());

  vector_v v(3);
  v << 1, 4, 9;
  matrix_v m = diag_matrix(v);
  EXPECT_EQ(1,m(0,0).val());
  EXPECT_EQ(4,m(1,1).val());
  EXPECT_EQ(9,m(2,2).val());
}



void test_mult_LLT(const matrix_v& L) {
  using stan::agrad::var;
  matrix_v LLT_eigen = L * L.transpose();
  matrix_v LLT_stan = multiply_lower_tri_self_transpose(L);
  EXPECT_EQ(L.rows(),LLT_stan.rows());
  EXPECT_EQ(L.cols(),LLT_stan.cols());
  for (int m = 0; m < L.rows(); ++m)
    for (int n = 0; n < L.cols(); ++n)
      EXPECT_FLOAT_EQ(LLT_eigen(m,n).val(), LLT_stan(m,n).val());  
}

TEST(AgradMatrix, multiplyLowerTriSelfTransposeGrad1) {
  using stan::agrad::var;
  using stan::agrad::multiply_lower_tri_self_transpose;
  matrix_v L(1,1);
  L << 3.0;
  std::vector<var> x(1);
  x[0] = L(0,0);

  matrix_v LLt = multiply_lower_tri_self_transpose(L);
  std::vector<var> y(1);
  y[0] = LLt(0,0);
  
  EXPECT_FLOAT_EQ(9.0, LLt(0,0).val());

  std::vector<std::vector<double> > J;
  stan::agrad::jacobian(y,x,J);
  
  EXPECT_FLOAT_EQ(6.0, J[0][0]);
}

TEST(AgradMatrix, multiplyLowerTriSelfTransposeGrad2) {
  using stan::agrad::var;
  using stan::agrad::multiply_lower_tri_self_transpose;
  matrix_v L(2,2);
  L << 
    1, 0,
    2, 3;
  std::vector<var> x(3);
  x[0] = L(0,0);
  x[1] = L(1,0);
  x[2] = L(1,1);

  matrix_v LLt = multiply_lower_tri_self_transpose(L);
  std::vector<var> y(4);
  y[0] = LLt(0,0);
  y[1] = LLt(0,1);
  y[2] = LLt(1,0);
  y[3] = LLt(1,1);
  
  EXPECT_FLOAT_EQ(1.0, LLt(0,0).val());
  EXPECT_FLOAT_EQ(2.0, LLt(0,1).val());
  EXPECT_FLOAT_EQ(2.0, LLt(1,0).val());
  EXPECT_FLOAT_EQ(13.0, LLt(1,1).val());

  std::vector<std::vector<double> > J;
  stan::agrad::jacobian(y,x,J);

  // L = 1 0
  //     2 3
  // Jacobian J = Jacobian(L * L')
  // J = 2 0 0
  //     2 1 0
  //     2 1 0
  //     0 4 6
  EXPECT_FLOAT_EQ(2.0,J[0][0]);
  EXPECT_FLOAT_EQ(0.0,J[0][1]);
  EXPECT_FLOAT_EQ(0.0,J[0][2]);

  EXPECT_FLOAT_EQ(2.0,J[1][0]);
  EXPECT_FLOAT_EQ(1.0,J[1][1]);
  EXPECT_FLOAT_EQ(0.0,J[1][2]);

  EXPECT_FLOAT_EQ(2.0,J[2][0]);
  EXPECT_FLOAT_EQ(1.0,J[2][1]);
  EXPECT_FLOAT_EQ(0.0,J[2][2]);

  EXPECT_FLOAT_EQ(0.0,J[3][0]);
  EXPECT_FLOAT_EQ(4.0,J[3][1]);
  EXPECT_FLOAT_EQ(6.0,J[3][2]);
}

TEST(AgradMatrix, multiplyLowerTriSelfTransposeGrad3) {
  using stan::agrad::var;
  using stan::agrad::multiply_lower_tri_self_transpose;
  matrix_v L(3,3);
  L << 
    1, 0, 0,
    2, 3, 0,
    4, 5, 6;
  std::vector<var> x(6);
  x[0] = L(0,0);
  x[1] = L(1,0);
  x[2] = L(1,1);
  x[3] = L(2,0);
  x[4] = L(2,1);
  x[5] = L(2,2);

  matrix_v LLt = multiply_lower_tri_self_transpose(L);
  std::vector<var> y(9);
  y[0] = LLt(0,0);
  y[1] = LLt(0,1);
  y[2] = LLt(0,2);
  y[3] = LLt(1,0);
  y[4] = LLt(1,1);
  y[5] = LLt(1,2);
  y[6] = LLt(2,0);
  y[7] = LLt(2,1);
  y[8] = LLt(2,2);
  
  std::vector<std::vector<double> > J;
  stan::agrad::jacobian(y,x,J);

  // L = 1 0 0
  //     2 3 0
  //     4 5 6
  // Jacobian J = Jacobian(L * L')
  // J = 2 0 0 0 0 0
  //     2 1 0 0 0 0
  //     4 0 0 1 0 0
  //     2 1 0 0 0 0
  //     0 4 6 0 0 0
  //     0 4 5 2 3 0
  //     4 0 0 1 0 0
  //     0 4 5 2 3 0
  //     0 0 0 8 10 12

  EXPECT_FLOAT_EQ(2.0,J[0][0]);
  EXPECT_FLOAT_EQ(0.0,J[0][1]);
  EXPECT_FLOAT_EQ(0.0,J[0][2]);
  EXPECT_FLOAT_EQ(0.0,J[0][3]);
  EXPECT_FLOAT_EQ(0.0,J[0][4]);
  EXPECT_FLOAT_EQ(0.0,J[0][5]);

  EXPECT_FLOAT_EQ(2.0,J[1][0]);
  EXPECT_FLOAT_EQ(1.0,J[1][1]);
  EXPECT_FLOAT_EQ(0.0,J[1][2]);
  EXPECT_FLOAT_EQ(0.0,J[1][3]);
  EXPECT_FLOAT_EQ(0.0,J[1][4]);
  EXPECT_FLOAT_EQ(0.0,J[1][5]);

  EXPECT_FLOAT_EQ(4.0,J[2][0]);
  EXPECT_FLOAT_EQ(0.0,J[2][1]);  EXPECT_FLOAT_EQ(0.0,J[2][2]);
  EXPECT_FLOAT_EQ(1.0,J[2][3]);
  EXPECT_FLOAT_EQ(0.0,J[2][4]);
  EXPECT_FLOAT_EQ(0.0,J[2][5]);

  EXPECT_FLOAT_EQ(2.0,J[3][0]);
  EXPECT_FLOAT_EQ(1.0,J[3][1]);
  EXPECT_FLOAT_EQ(0.0,J[3][2]);
  EXPECT_FLOAT_EQ(0.0,J[3][3]);
  EXPECT_FLOAT_EQ(0.0,J[3][4]);
  EXPECT_FLOAT_EQ(0.0,J[3][5]);

  EXPECT_FLOAT_EQ(0.0,J[4][0]);
  EXPECT_FLOAT_EQ(4.0,J[4][1]);
  EXPECT_FLOAT_EQ(6.0,J[4][2]);
  EXPECT_FLOAT_EQ(0.0,J[4][3]);
  EXPECT_FLOAT_EQ(0.0,J[4][4]);
  EXPECT_FLOAT_EQ(0.0,J[4][5]);

  EXPECT_FLOAT_EQ(0.0,J[5][0]);
  EXPECT_FLOAT_EQ(4.0,J[5][1]);
  EXPECT_FLOAT_EQ(5.0,J[5][2]);
  EXPECT_FLOAT_EQ(2.0,J[5][3]);
  EXPECT_FLOAT_EQ(3.0,J[5][4]);
  EXPECT_FLOAT_EQ(0.0,J[5][5]);

  EXPECT_FLOAT_EQ(4.0,J[6][0]);
  EXPECT_FLOAT_EQ(0.0,J[6][1]);
  EXPECT_FLOAT_EQ(0.0,J[6][2]);
  EXPECT_FLOAT_EQ(1.0,J[6][3]);
  EXPECT_FLOAT_EQ(0.0,J[6][4]);
  EXPECT_FLOAT_EQ(0.0,J[6][5]);

  EXPECT_FLOAT_EQ(0.0,J[7][0]);
  EXPECT_FLOAT_EQ(4.0,J[7][1]);
  EXPECT_FLOAT_EQ(5.0,J[7][2]);
  EXPECT_FLOAT_EQ(2.0,J[7][3]);
  EXPECT_FLOAT_EQ(3.0,J[7][4]);
  EXPECT_FLOAT_EQ(0.0,J[7][5]);

  EXPECT_FLOAT_EQ(0.0,J[8][0]);
  EXPECT_FLOAT_EQ(0.0,J[8][1]);
  EXPECT_FLOAT_EQ(0.0,J[8][2]);
  EXPECT_FLOAT_EQ(8.0,J[8][3]);
  EXPECT_FLOAT_EQ(10.0,J[8][4]);
  EXPECT_FLOAT_EQ(12.0,J[8][5]);
}

TEST(AgradMatrix, multiplyLowerTriSelfTranspose) {
  using stan::agrad::multiply_lower_tri_self_transpose;
  matrix_v L(3,3);
  L << 1, 0, 0,   
       2, 3, 0,   
       4, 5, 6;
  test_mult_LLT(L);

  matrix_v I(2,2);
  I << 3, 0, 
       4, -3;
  test_mult_LLT(I);

  // matrix_v J(1,1);
  // J << 3.0;
  // test_mult_LLT(J);

  // matrix_v K(0,0);
  // test_mult_LLT(K);
}

void test_tcrossprod(const matrix_v& L) {
  using stan::agrad::var;
  using stan::agrad::tcrossprod;
  matrix_v LLT_eigen = L * L.transpose();
  matrix_v LLT_stan = tcrossprod(L);
  EXPECT_EQ(L.rows(),LLT_stan.rows());
  EXPECT_EQ(L.cols(),LLT_stan.cols());
  for (int m = 0; m < L.rows(); ++m)
    for (int n = 0; n < L.cols(); ++n)
      EXPECT_FLOAT_EQ(LLT_eigen(m,n).val(), LLT_stan(m,n).val());
}
/*
std::vector<double> grad_stan_tcrossprod(const matrix_v& L,
                                         int m_test, int n_test) {
  size_t K = (L.rows() * (L.rows() + 1)) / 2;
  std::vector<var> x(K);
  int pos = 0;
  for (int m = 0; m < L.rows(); ++m)
    for (int n = 0; n <= m; ++n)
      x[pos++] = L(m,n);
  matrix_v LLT =  tcrossprod(L);
  var f = LLT(m_test,n_test);
  std::vector<double> g;
  f.grad(x,g);
  return g;
}

void test_tcrossprod_grad(const matrix_v& L, int m_test, int n_test) {
  using std::vector;
  vector<double> g_eigen = grad_eigen(L,m_test,n_test);
  vector<double> g_stan = grad_stan_tcrossprod(L,m_test,n_test);
  std::cout << "********** GRAD SIZE=" << g_eigen.size() << std::endl;
  EXPECT_EQ(g_eigen.size(), g_stan.size());
  for (size_t i = 0; i < g_eigen.size(); ++i)
    EXPECT_FLOAT_EQ(g_eigen[i], g_stan[i]);
}
*/
TEST(AgradMatrix, tcrossprod) {
  matrix_v L(3,3);
  L << 1, 0, 0,
       2, 3, 0,
       4, 5, 6;
  test_tcrossprod(L);
//  test_tcrossprod_grad(L, L.rows(), L.cols());

  matrix_v I(2,2);
  I << 3, 0,
       4, -3;
  test_tcrossprod(I);
//  test_tcrossprod_grad(I, I.rows(), I.cols());

  matrix_v J(1,1);
  J << 3.0;
  test_tcrossprod(J);
//  test_tcrossprod_grad(J, J.rows(), J.cols());

  matrix_v K(0,0);
  test_tcrossprod(K);
//  test_tcrossprod_grad(K, K.rows(), K.cols());

}
void test_crossprod(const matrix_v& L) {
  using stan::agrad::var;
  using stan::agrad::crossprod;
  matrix_v LLT_eigen = L.transpose() * L;
  matrix_v LLT_stan = crossprod(L);
  EXPECT_EQ(L.rows(),LLT_stan.rows());
  EXPECT_EQ(L.cols(),LLT_stan.cols());
  for (int m = 0; m < L.rows(); ++m)
    for (int n = 0; n < L.cols(); ++n)
      EXPECT_FLOAT_EQ(LLT_eigen(m,n).val(), LLT_stan(m,n).val());
}

TEST(AgradMatrix, crossprod) {
  matrix_v L(3,3);
  L << 1, 0, 0,
       2, 3, 0,
       4, 5, 6;
  test_crossprod(L);
//  test_tcrossprod_grad(L, L.rows(), L.cols());

  matrix_v I(2,2);
  I << 3, 0,
       4, -3;
  test_crossprod(I);
//  test_tcrossprod_grad(I, I.rows(), I.cols());

  matrix_v J(1,1);
  J << 3.0;
  test_crossprod(J);
//  test_tcrossprod_grad(J, J.rows(), J.cols());

  matrix_v K(0,0);
  test_crossprod(K);
//  test_tcrossprod_grad(K, K.rows(), K.cols());

}
