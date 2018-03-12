#ifndef PRECICE_NO_MPI
#include "com/CommunicateBoundingBox.hpp"
#include "com/MPIDirectCommunication.hpp"
#include "mesh/Mesh.hpp"
#include "testing/Testing.hpp"
#include "testing/Fixtures.hpp"
#include "utils/Parallel.hpp"

using namespace precice;
using namespace precice::com;

BOOST_AUTO_TEST_SUITE(CommunicationTests)

BOOST_AUTO_TEST_SUITE(CommunicateBoundingBoxTests)

BOOST_FIXTURE_TEST_CASE(SendAndReceiveBoundingBox, testing::M2NFixture,
                       * testing::MinRanks(2)
                       * boost::unit_test::fixture<testing::MPICommRestrictFixture>(std::vector<int>({0, 1})))
{
  if (utils::Parallel::getCommunicatorSize() != 2)
    return;

  for (int dim = 2; dim <= 3; dim++) {
    mesh::Mesh::BoundingBox bb;

    for (int i=0; i < dim; i++) {
      bb.push_back(std::make_pair(i,i+1));
    }

    CommunicateBoundingBox comBB(m2n->getMasterCommunication());

    if (utils::Parallel::getProcessRank() == 0) {
      comBB.sendBoundingBox(bb, 0); // send to 0 as we communicate between both master ranks
    }
    else if (utils::Parallel::getProcessRank() == 1) {

      mesh::Mesh::BoundingBox bbCompare;
      for (int i=0; i < dim; i++) {
        bbCompare.push_back(std::make_pair(-1,-1));
      }

      comBB.receiveBoundingBox(bbCompare, 0);

      BOOST_TEST(bb==bbCompare);
    }
  }
}



BOOST_FIXTURE_TEST_CASE(SendAndReceiveBoundingBoxMap, testing::M2NFixture,
                       * testing::MinRanks(2)
                       * boost::unit_test::fixture<testing::MPICommRestrictFixture>(std::vector<int>({0, 1})))
{
  if (utils::Parallel::getCommunicatorSize() != 2)
    return;

  for (int dim = 2; dim <= 3; dim++) {
    mesh::Mesh::BoundingBox bb;
    mesh::Mesh::BoundingBoxMap bbm;

    for (int rank=0; rank <3; rank++) {

      for (int i=0; i < dim; i++) {
        bb.push_back(std::make_pair(i,i+1));
      }

      bbm[rank]=bb;
      bb.clear();
    }

    CommunicateBoundingBox comBB(m2n->getMasterCommunication());

    if (utils::Parallel::getProcessRank() == 0) {
      comBB.sendBoundingBoxMap(bbm, 0); // send to 0 as we communicate between both master ranks
    }
    else if (utils::Parallel::getProcessRank() == 1) {

      mesh::Mesh::BoundingBox bbCompare;
      mesh::Mesh::BoundingBoxMap bbmCompare;

    for (int rank=0; rank <3; rank++) {

      for (int i=0; i < dim; i++) {
        bbCompare.push_back(std::make_pair(-1,-1));
      }

      bbmCompare[rank]=bbCompare;
      bbCompare.clear();
    }

      comBB.receiveBoundingBoxMap(bbmCompare, 0);

      for(int rank=0; rank<3; rank++){

        BOOST_TEST(bbm[rank]==bbmCompare[rank]);
        
      }
    }
  }
}
//@todo: tests for all other methods

BOOST_AUTO_TEST_SUITE_END() // BB

BOOST_AUTO_TEST_SUITE_END() // Communication

#endif // not PRECICE_NO_MPI
