#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "../FishGUI/document.h"
using namespace boost::unit_test;

void doc_init_function()
{
  boost::shared_ptr<FishDetector::Document> tester( new FishDetector::Document );
  std::uint64_t T = 0;
  BOOST_TEST(tester->getIDCounter() == T);
}

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
  
  framework::master_test_suite().
    add( BOOST_TEST_CASE( &doc_init_function));

  return 0;
}
