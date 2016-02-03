#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <fstream>
#include "../FishGUI/document.h"
using namespace boost::unit_test;

void doc_init_function()
{
  boost::shared_ptr<FishDetector::Document> tester( new FishDetector::Document );
  std::uint64_t T = 0;
  BOOST_TEST(tester->getIDCounter() == T);
}

void json_simple_test()
{
  boost::shared_ptr<FishDetector::Document> tester(new FishDetector::Document);
  uint64_t x,y,h,w,frame;
  x = 1;
  y = 1;
  h = 5;
  w = 5;
  frame = 1;
  tester->addAnnotation();
  tester->addAnnotationLocation(std::uint64_t(0), frame, FishDetector::Rect(x,y,w,h));
  std::string filename = "../test.json";
  BOOST_TEST(tester->writeJSON(filename) == 0);

}

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
  
  framework::master_test_suite().
    add( BOOST_TEST_CASE( &doc_init_function));
  framework::master_test_suite().
    add( BOOST_TEST_CASE( &json_simple_test));

  return 0;
}