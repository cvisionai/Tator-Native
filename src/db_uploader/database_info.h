/// @file
/// @brief Defines DatabaseInfo class which contains information needed
///        to connect to a database.

#ifndef DATABASE_INFO_H
#define DATABASE_INFO_H

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "serialization.h"

namespace fish_annotator { namespace db_uploader {

namespace pt = boost::property_tree;

/// Stores information needed to connect to a database.
class DatabaseInfo : public Serialization {
public:
  /// Constructor.
  DatabaseInfo();

  /// Gets server name.
  std::string getServer() const {return server_;}

  /// Gets database name.
  std::string getDatabase() const {return database_;}

  /// Gets username.
  std::string getUsername() const {return username_;}

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);
private:
  /// Server name.
  std::string server_;

  /// Database name.
  std::string database_;

  /// Username.
  std::string username_;
};

}} // namespace fish_annotator::db_uploader
#endif // DATABASE_INFO_H
