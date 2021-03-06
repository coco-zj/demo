// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: query.proto

#ifndef PROTOBUF_query_2eproto__INCLUDED
#define PROTOBUF_query_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_query_2eproto();
void protobuf_AssignDesc_query_2eproto();
void protobuf_ShutdownFile_query_2eproto();

class Query;

// ===================================================================

class Query : public ::google::protobuf::Message {
 public:
  Query();
  virtual ~Query();

  Query(const Query& from);

  inline Query& operator=(const Query& from) {
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
  static const Query& default_instance();

  void Swap(Query* other);

  // implements Message ----------------------------------------------

  Query* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Query& from);
  void MergeFrom(const Query& from);
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

  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional string from = 2;
  inline bool has_from() const;
  inline void clear_from();
  static const int kFromFieldNumber = 2;
  inline const ::std::string& from() const;
  inline void set_from(const ::std::string& value);
  inline void set_from(const char* value);
  inline void set_from(const char* value, size_t size);
  inline ::std::string* mutable_from();
  inline ::std::string* release_from();
  inline void set_allocated_from(::std::string* from);

  // @@protoc_insertion_point(class_scope:Query)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_from();
  inline void clear_has_from();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* from_;
  ::google::protobuf::int32 id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_query_2eproto();
  friend void protobuf_AssignDesc_query_2eproto();
  friend void protobuf_ShutdownFile_query_2eproto();

  void InitAsDefaultInstance();
  static Query* default_instance_;
};
// ===================================================================


// ===================================================================

// Query

// required int32 id = 1;
inline bool Query::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Query::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Query::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Query::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Query::id() const {
  return id_;
}
inline void Query::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional string from = 2;
inline bool Query::has_from() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Query::set_has_from() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Query::clear_has_from() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Query::clear_from() {
  if (from_ != &::google::protobuf::internal::kEmptyString) {
    from_->clear();
  }
  clear_has_from();
}
inline const ::std::string& Query::from() const {
  return *from_;
}
inline void Query::set_from(const ::std::string& value) {
  set_has_from();
  if (from_ == &::google::protobuf::internal::kEmptyString) {
    from_ = new ::std::string;
  }
  from_->assign(value);
}
inline void Query::set_from(const char* value) {
  set_has_from();
  if (from_ == &::google::protobuf::internal::kEmptyString) {
    from_ = new ::std::string;
  }
  from_->assign(value);
}
inline void Query::set_from(const char* value, size_t size) {
  set_has_from();
  if (from_ == &::google::protobuf::internal::kEmptyString) {
    from_ = new ::std::string;
  }
  from_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Query::mutable_from() {
  set_has_from();
  if (from_ == &::google::protobuf::internal::kEmptyString) {
    from_ = new ::std::string;
  }
  return from_;
}
inline ::std::string* Query::release_from() {
  clear_has_from();
  if (from_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = from_;
    from_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Query::set_allocated_from(::std::string* from) {
  if (from_ != &::google::protobuf::internal::kEmptyString) {
    delete from_;
  }
  if (from) {
    set_has_from();
    from_ = from;
  } else {
    clear_has_from();
    from_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_query_2eproto__INCLUDED
