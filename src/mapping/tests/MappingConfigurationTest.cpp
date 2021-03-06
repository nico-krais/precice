#include "testing/Testing.hpp"
#include "mesh/config/DataConfiguration.hpp"
#include "mesh/config/MeshConfiguration.hpp"
#include "mapping/config/MappingConfiguration.hpp"
#include "mapping/Mapping.hpp"
#include "xml/XMLTag.hpp"

using namespace precice;
using namespace precice::mapping;

BOOST_AUTO_TEST_SUITE(MappingTests)
BOOST_AUTO_TEST_SUITE(Configuration)

BOOST_AUTO_TEST_CASE(Configuration)
{
  
  std::string pathToTests = testing::getPathToSources() + "/mapping/tests/";
  std::string file(pathToTests + "mapping-config.xml");
  using xml::XMLTag;
  XMLTag tag = xml::getRootTag();
  mesh::PtrDataConfiguration dataConfig( new mesh::DataConfiguration(tag) );
  dataConfig->setDimensions(3);
  mesh::PtrMeshConfiguration meshConfig(new mesh::MeshConfiguration(tag, dataConfig));
  meshConfig->setDimensions(3);
  mapping::MappingConfiguration mappingConfig(tag, meshConfig);
  xml::configure(tag, file);
    
  BOOST_TEST(meshConfig->meshes().size() == 3);
  BOOST_TEST(mappingConfig.mappings().size() == 3);
  BOOST_TEST(mappingConfig.mappings()[0].timing == MappingConfiguration::ON_DEMAND);
  BOOST_TEST(mappingConfig.mappings()[0].fromMesh == meshConfig->meshes()[0]);
  BOOST_TEST(mappingConfig.mappings()[0].toMesh == meshConfig->meshes()[2]);
  BOOST_TEST(mappingConfig.mappings()[0].direction == MappingConfiguration::WRITE);

  BOOST_TEST(mappingConfig.mappings()[1].timing == MappingConfiguration::INITIAL);
  BOOST_TEST(mappingConfig.mappings()[1].fromMesh == meshConfig->meshes()[2]);
  BOOST_TEST(mappingConfig.mappings()[1].toMesh == meshConfig->meshes()[1]);
  BOOST_TEST(mappingConfig.mappings()[1].direction == MappingConfiguration::READ);

  BOOST_TEST(mappingConfig.mappings()[2].timing == MappingConfiguration::ON_ADVANCE);
  BOOST_TEST(mappingConfig.mappings()[2].fromMesh == meshConfig->meshes()[1]);
  BOOST_TEST(mappingConfig.mappings()[2].toMesh == meshConfig->meshes()[0]);
  BOOST_TEST(mappingConfig.mappings()[2].direction == MappingConfiguration::WRITE);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
