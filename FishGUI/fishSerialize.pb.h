// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fishSerialize.proto

#ifndef PROTOBUF_fishSerialize_2eproto__INCLUDED
#define PROTOBUF_fishSerialize_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace fishSerialize {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_fishSerialize_2eproto();
void protobuf_AssignDesc_fishSerialize_2eproto();
void protobuf_ShutdownFile_fishSerialize_2eproto();

class Fish;
class FishList;

enum Fish_fTypeEnum {
  Fish_fTypeEnum_ROUND = 0,
  Fish_fTypeEnum_FLAT = 1,
  Fish_fTypeEnum_SKATE = 2
};
bool Fish_fTypeEnum_IsValid(int value);
const Fish_fTypeEnum Fish_fTypeEnum_fTypeEnum_MIN = Fish_fTypeEnum_ROUND;
const Fish_fTypeEnum Fish_fTypeEnum_fTypeEnum_MAX = Fish_fTypeEnum_SKATE;
const int Fish_fTypeEnum_fTypeEnum_ARRAYSIZE = Fish_fTypeEnum_fTypeEnum_MAX + 1;

const ::google::protobuf::EnumDescriptor* Fish_fTypeEnum_descriptor();
inline const ::std::string& Fish_fTypeEnum_Name(Fish_fTypeEnum value) {
  return ::google::protobuf::internal::NameOfEnum(
    Fish_fTypeEnum_descriptor(), value);
}
inline bool Fish_fTypeEnum_Parse(
    const ::std::string& name, Fish_fTypeEnum* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Fish_fTypeEnum>(
    Fish_fTypeEnum_descriptor(), name, value);
}
// ===================================================================

class Fish : public ::google::protobuf::Message {
 public:
  Fish();
  virtual ~Fish();

  Fish(const Fish& from);

  inline Fish& operator=(const Fish& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Fish& default_instance();

  void Swap(Fish* other);

  // implements Message ----------------------------------------------

  Fish* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Fish& from);
  void MergeFrom(const Fish& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Fish_fTypeEnum fTypeEnum;
  static const fTypeEnum ROUND = Fish_fTypeEnum_ROUND;
  static const fTypeEnum FLAT = Fish_fTypeEnum_FLAT;
  static const fTypeEnum SKATE = Fish_fTypeEnum_SKATE;
  static inline bool fTypeEnum_IsValid(int value) {
    return Fish_fTypeEnum_IsValid(value);
  }
  static const fTypeEnum fTypeEnum_MIN =
    Fish_fTypeEnum_fTypeEnum_MIN;
  static const fTypeEnum fTypeEnum_MAX =
    Fish_fTypeEnum_fTypeEnum_MAX;
  static const int fTypeEnum_ARRAYSIZE =
    Fish_fTypeEnum_fTypeEnum_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  fTypeEnum_descriptor() {
    return Fish_fTypeEnum_descriptor();
  }
  static inline const ::std::string& fTypeEnum_Name(fTypeEnum value) {
    return Fish_fTypeEnum_Name(value);
  }
  static inline bool fTypeEnum_Parse(const ::std::string& name,
      fTypeEnum* value) {
    return Fish_fTypeEnum_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required .fishSerialize.Fish.fTypeEnum fType = 1;
  inline bool has_ftype() const;
  inline void clear_ftype();
  static const int kFTypeFieldNumber = 1;
  inline ::fishSerialize::Fish_fTypeEnum ftype() const;
  inline void set_ftype(::fishSerialize::Fish_fTypeEnum value);

  // optional string fSpecies = 2;
  inline bool has_fspecies() const;
  inline void clear_fspecies();
  static const int kFSpeciesFieldNumber = 2;
  inline const ::std::string& fspecies() const;
  inline void set_fspecies(const ::std::string& value);
  inline void set_fspecies(const char* value);
  inline void set_fspecies(const char* value, size_t size);
  inline ::std::string* mutable_fspecies();
  inline ::std::string* release_fspecies();
  inline void set_allocated_fspecies(::std::string* fspecies);

  // required int32 fFrame = 3;
  inline bool has_fframe() const;
  inline void clear_fframe();
  static const int kFFrameFieldNumber = 3;
  inline ::google::protobuf::int32 fframe() const;
  inline void set_fframe(::google::protobuf::int32 value);

  // optional float xLoc = 4;
  inline bool has_xloc() const;
  inline void clear_xloc();
  static const int kXLocFieldNumber = 4;
  inline float xloc() const;
  inline void set_xloc(float value);

  // optional float yLoc = 5;
  inline bool has_yloc() const;
  inline void clear_yloc();
  static const int kYLocFieldNumber = 5;
  inline float yloc() const;
  inline void set_yloc(float value);

  // @@protoc_insertion_point(class_scope:fishSerialize.Fish)
 private:
  inline void set_has_ftype();
  inline void clear_has_ftype();
  inline void set_has_fspecies();
  inline void clear_has_fspecies();
  inline void set_has_fframe();
  inline void clear_has_fframe();
  inline void set_has_xloc();
  inline void clear_has_xloc();
  inline void set_has_yloc();
  inline void clear_has_yloc();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* fspecies_;
  int ftype_;
  ::google::protobuf::int32 fframe_;
  float xloc_;
  float yloc_;
  friend void  protobuf_AddDesc_fishSerialize_2eproto();
  friend void protobuf_AssignDesc_fishSerialize_2eproto();
  friend void protobuf_ShutdownFile_fishSerialize_2eproto();

  void InitAsDefaultInstance();
  static Fish* default_instance_;
};
// -------------------------------------------------------------------

class FishList : public ::google::protobuf::Message {
 public:
  FishList();
  virtual ~FishList();

  FishList(const FishList& from);

  inline FishList& operator=(const FishList& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FishList& default_instance();

  void Swap(FishList* other);

  // implements Message ----------------------------------------------

  FishList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FishList& from);
  void MergeFrom(const FishList& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bool towOpen = 1 [default = true];
  inline bool has_towopen() const;
  inline void clear_towopen();
  static const int kTowOpenFieldNumber = 1;
  inline bool towopen() const;
  inline void set_towopen(bool value);

  // repeated .fishSerialize.Fish fish = 2;
  inline int fish_size() const;
  inline void clear_fish();
  static const int kFishFieldNumber = 2;
  inline const ::fishSerialize::Fish& fish(int index) const;
  inline ::fishSerialize::Fish* mutable_fish(int index);
  inline ::fishSerialize::Fish* add_fish();
  inline const ::google::protobuf::RepeatedPtrField< ::fishSerialize::Fish >&
      fish() const;
  inline ::google::protobuf::RepeatedPtrField< ::fishSerialize::Fish >*
      mutable_fish();

  // @@protoc_insertion_point(class_scope:fishSerialize.FishList)
 private:
  inline void set_has_towopen();
  inline void clear_has_towopen();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::fishSerialize::Fish > fish_;
  bool towopen_;
  friend void  protobuf_AddDesc_fishSerialize_2eproto();
  friend void protobuf_AssignDesc_fishSerialize_2eproto();
  friend void protobuf_ShutdownFile_fishSerialize_2eproto();

  void InitAsDefaultInstance();
  static FishList* default_instance_;
};
// ===================================================================


// ===================================================================

// Fish

// required .fishSerialize.Fish.fTypeEnum fType = 1;
inline bool Fish::has_ftype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Fish::set_has_ftype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Fish::clear_has_ftype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Fish::clear_ftype() {
  ftype_ = 0;
  clear_has_ftype();
}
inline ::fishSerialize::Fish_fTypeEnum Fish::ftype() const {
  // @@protoc_insertion_point(field_get:fishSerialize.Fish.fType)
  return static_cast< ::fishSerialize::Fish_fTypeEnum >(ftype_);
}
inline void Fish::set_ftype(::fishSerialize::Fish_fTypeEnum value) {
  assert(::fishSerialize::Fish_fTypeEnum_IsValid(value));
  set_has_ftype();
  ftype_ = value;
  // @@protoc_insertion_point(field_set:fishSerialize.Fish.fType)
}

// optional string fSpecies = 2;
inline bool Fish::has_fspecies() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Fish::set_has_fspecies() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Fish::clear_has_fspecies() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Fish::clear_fspecies() {
  if (fspecies_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    fspecies_->clear();
  }
  clear_has_fspecies();
}
inline const ::std::string& Fish::fspecies() const {
  // @@protoc_insertion_point(field_get:fishSerialize.Fish.fSpecies)
  return *fspecies_;
}
inline void Fish::set_fspecies(const ::std::string& value) {
  set_has_fspecies();
  if (fspecies_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    fspecies_ = new ::std::string;
  }
  fspecies_->assign(value);
  // @@protoc_insertion_point(field_set:fishSerialize.Fish.fSpecies)
}
inline void Fish::set_fspecies(const char* value) {
  set_has_fspecies();
  if (fspecies_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    fspecies_ = new ::std::string;
  }
  fspecies_->assign(value);
  // @@protoc_insertion_point(field_set_char:fishSerialize.Fish.fSpecies)
}
inline void Fish::set_fspecies(const char* value, size_t size) {
  set_has_fspecies();
  if (fspecies_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    fspecies_ = new ::std::string;
  }
  fspecies_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:fishSerialize.Fish.fSpecies)
}
inline ::std::string* Fish::mutable_fspecies() {
  set_has_fspecies();
  if (fspecies_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    fspecies_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:fishSerialize.Fish.fSpecies)
  return fspecies_;
}
inline ::std::string* Fish::release_fspecies() {
  clear_has_fspecies();
  if (fspecies_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = fspecies_;
    fspecies_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Fish::set_allocated_fspecies(::std::string* fspecies) {
  if (fspecies_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete fspecies_;
  }
  if (fspecies) {
    set_has_fspecies();
    fspecies_ = fspecies;
  } else {
    clear_has_fspecies();
    fspecies_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:fishSerialize.Fish.fSpecies)
}

// required int32 fFrame = 3;
inline bool Fish::has_fframe() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Fish::set_has_fframe() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Fish::clear_has_fframe() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Fish::clear_fframe() {
  fframe_ = 0;
  clear_has_fframe();
}
inline ::google::protobuf::int32 Fish::fframe() const {
  // @@protoc_insertion_point(field_get:fishSerialize.Fish.fFrame)
  return fframe_;
}
inline void Fish::set_fframe(::google::protobuf::int32 value) {
  set_has_fframe();
  fframe_ = value;
  // @@protoc_insertion_point(field_set:fishSerialize.Fish.fFrame)
}

// optional float xLoc = 4;
inline bool Fish::has_xloc() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Fish::set_has_xloc() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Fish::clear_has_xloc() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Fish::clear_xloc() {
  xloc_ = 0;
  clear_has_xloc();
}
inline float Fish::xloc() const {
  // @@protoc_insertion_point(field_get:fishSerialize.Fish.xLoc)
  return xloc_;
}
inline void Fish::set_xloc(float value) {
  set_has_xloc();
  xloc_ = value;
  // @@protoc_insertion_point(field_set:fishSerialize.Fish.xLoc)
}

// optional float yLoc = 5;
inline bool Fish::has_yloc() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Fish::set_has_yloc() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Fish::clear_has_yloc() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Fish::clear_yloc() {
  yloc_ = 0;
  clear_has_yloc();
}
inline float Fish::yloc() const {
  // @@protoc_insertion_point(field_get:fishSerialize.Fish.yLoc)
  return yloc_;
}
inline void Fish::set_yloc(float value) {
  set_has_yloc();
  yloc_ = value;
  // @@protoc_insertion_point(field_set:fishSerialize.Fish.yLoc)
}

// -------------------------------------------------------------------

// FishList

// optional bool towOpen = 1 [default = true];
inline bool FishList::has_towopen() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FishList::set_has_towopen() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FishList::clear_has_towopen() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FishList::clear_towopen() {
  towopen_ = true;
  clear_has_towopen();
}
inline bool FishList::towopen() const {
  // @@protoc_insertion_point(field_get:fishSerialize.FishList.towOpen)
  return towopen_;
}
inline void FishList::set_towopen(bool value) {
  set_has_towopen();
  towopen_ = value;
  // @@protoc_insertion_point(field_set:fishSerialize.FishList.towOpen)
}

// repeated .fishSerialize.Fish fish = 2;
inline int FishList::fish_size() const {
  return fish_.size();
}
inline void FishList::clear_fish() {
  fish_.Clear();
}
inline const ::fishSerialize::Fish& FishList::fish(int index) const {
  // @@protoc_insertion_point(field_get:fishSerialize.FishList.fish)
  return fish_.Get(index);
}
inline ::fishSerialize::Fish* FishList::mutable_fish(int index) {
  // @@protoc_insertion_point(field_mutable:fishSerialize.FishList.fish)
  return fish_.Mutable(index);
}
inline ::fishSerialize::Fish* FishList::add_fish() {
  // @@protoc_insertion_point(field_add:fishSerialize.FishList.fish)
  return fish_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::fishSerialize::Fish >&
FishList::fish() const {
  // @@protoc_insertion_point(field_list:fishSerialize.FishList.fish)
  return fish_;
}
inline ::google::protobuf::RepeatedPtrField< ::fishSerialize::Fish >*
FishList::mutable_fish() {
  // @@protoc_insertion_point(field_mutable_list:fishSerialize.FishList.fish)
  return &fish_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace fishSerialize

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::fishSerialize::Fish_fTypeEnum> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fishSerialize::Fish_fTypeEnum>() {
  return ::fishSerialize::Fish_fTypeEnum_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_fishSerialize_2eproto__INCLUDED
