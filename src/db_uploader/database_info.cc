#include "database_info.h"

namespace tator { namespace db_uploader {

DatabaseInfo::DatabaseInfo()
  : server_()
  , database_()
  , username_() {
}

pt::ptree DatabaseInfo::write() const {
  pt::ptree tree;
  tree.put("server", server_);
  tree.put("database", database_);
  tree.put("username", username_);
  return tree;
}

void DatabaseInfo::read(const pt::ptree &tree) {
  server_ = tree.get<std::string>("server");
  database_ = tree.get<std::string>("database");
  username_ = tree.get<std::string>("username");
}

}} // namespace tator::db_uploader

