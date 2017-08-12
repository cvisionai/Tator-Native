#include "test_species.h"
#include "species.h"

void TestSpecies::testSerialize() {
  fish_annotator::Species dog("Dog");
  dog.getSubspecies().push_back("Dalmation");
  dog.getSubspecies().push_back("Labrador");
  fish_annotator::Species cat("Cat");
  cat.getSubspecies().push_back("Savannah");
  cat.getSubspecies().push_back("Ragamuffin");
  cat.getSubspecies().push_back("Persian");
  fish_annotator::SpeciesList list;
  list.getSpecies().push_back(dog);
  list.getSpecies().push_back(cat);
  QVERIFY(fish_annotator::serialize(list, "species_list.json"));
  fish_annotator::SpeciesList deserialized_list;
  QVERIFY(fish_annotator::deserialize(deserialized_list, "species_list.json"));
  QVERIFY(list == deserialized_list);
}

QTEST_MAIN(TestSpecies)
#include "test_species.moc"

