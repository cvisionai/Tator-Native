#include "test_species.h"
#include "fish_detector/common/species.h"

void TestSpecies::testSerialize() {
  fish_detector::Species dog("Dog");
  dog.getSubspecies().push_back("Dalmation");
  dog.getSubspecies().push_back("Labrador");
  fish_detector::Species cat("Cat");
  cat.getSubspecies().push_back("Savannah");
  cat.getSubspecies().push_back("Ragamuffin");
  cat.getSubspecies().push_back("Persian");
  fish_detector::SpeciesList list;
  list.getSpecies().push_back(dog);
  list.getSpecies().push_back(cat);
  QVERIFY(fish_detector::serialize(list, "species_list.json"));
  fish_detector::SpeciesList deserialized_list;
  QVERIFY(fish_detector::deserialize(deserialized_list, "species_list.json"));
  QVERIFY(list == deserialized_list);
}

QTEST_MAIN(TestSpecies)
#include "test_species.moc"

