// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: api.proto

#ifndef PROTOBUF_api_2eproto__INCLUDED
#define PROTOBUF_api_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace xtech {
namespace selfomat {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_api_2eproto();
void protobuf_AssignDesc_api_2eproto();
void protobuf_ShutdownFile_api_2eproto();

class UpdateStatus;

enum UpdateStatus_State {
  UpdateStatus_State_IDLE = 0,
  UpdateStatus_State_UPDATE_IN_PROGRESS = 1
};
bool UpdateStatus_State_IsValid(int value);
const UpdateStatus_State UpdateStatus_State_State_MIN = UpdateStatus_State_IDLE;
const UpdateStatus_State UpdateStatus_State_State_MAX = UpdateStatus_State_UPDATE_IN_PROGRESS;
const int UpdateStatus_State_State_ARRAYSIZE = UpdateStatus_State_State_MAX + 1;

const ::google::protobuf::EnumDescriptor* UpdateStatus_State_descriptor();
inline const ::std::string& UpdateStatus_State_Name(UpdateStatus_State value) {
  return ::google::protobuf::internal::NameOfEnum(
    UpdateStatus_State_descriptor(), value);
}
inline bool UpdateStatus_State_Parse(
    const ::std::string& name, UpdateStatus_State* value) {
  return ::google::protobuf::internal::ParseNamedEnum<UpdateStatus_State>(
    UpdateStatus_State_descriptor(), name, value);
}
// ===================================================================

class UpdateStatus : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:xtech.selfomat.UpdateStatus) */ {
 public:
  UpdateStatus();
  virtual ~UpdateStatus();

  UpdateStatus(const UpdateStatus& from);

  inline UpdateStatus& operator=(const UpdateStatus& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const UpdateStatus& default_instance();

  void Swap(UpdateStatus* other);

  // implements Message ----------------------------------------------

  inline UpdateStatus* New() const { return New(NULL); }

  UpdateStatus* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UpdateStatus& from);
  void MergeFrom(const UpdateStatus& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(UpdateStatus* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef UpdateStatus_State State;
  static const State IDLE =
    UpdateStatus_State_IDLE;
  static const State UPDATE_IN_PROGRESS =
    UpdateStatus_State_UPDATE_IN_PROGRESS;
  static inline bool State_IsValid(int value) {
    return UpdateStatus_State_IsValid(value);
  }
  static const State State_MIN =
    UpdateStatus_State_State_MIN;
  static const State State_MAX =
    UpdateStatus_State_State_MAX;
  static const int State_ARRAYSIZE =
    UpdateStatus_State_State_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  State_descriptor() {
    return UpdateStatus_State_descriptor();
  }
  static inline const ::std::string& State_Name(State value) {
    return UpdateStatus_State_Name(value);
  }
  static inline bool State_Parse(const ::std::string& name,
      State* value) {
    return UpdateStatus_State_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required .xtech.selfomat.UpdateStatus.State current_state = 1;
  bool has_current_state() const;
  void clear_current_state();
  static const int kCurrentStateFieldNumber = 1;
  ::xtech::selfomat::UpdateStatus_State current_state() const;
  void set_current_state(::xtech::selfomat::UpdateStatus_State value);

  // optional string current_version = 2;
  bool has_current_version() const;
  void clear_current_version();
  static const int kCurrentVersionFieldNumber = 2;
  const ::std::string& current_version() const;
  void set_current_version(const ::std::string& value);
  void set_current_version(const char* value);
  void set_current_version(const char* value, size_t size);
  ::std::string* mutable_current_version();
  ::std::string* release_current_version();
  void set_allocated_current_version(::std::string* current_version);

  // @@protoc_insertion_point(class_scope:xtech.selfomat.UpdateStatus)
 private:
  inline void set_has_current_state();
  inline void clear_has_current_state();
  inline void set_has_current_version();
  inline void clear_has_current_version();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr current_version_;
  int current_state_;
  friend void  protobuf_AddDesc_api_2eproto();
  friend void protobuf_AssignDesc_api_2eproto();
  friend void protobuf_ShutdownFile_api_2eproto();

  void InitAsDefaultInstance();
  static UpdateStatus* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// UpdateStatus

// required .xtech.selfomat.UpdateStatus.State current_state = 1;
inline bool UpdateStatus::has_current_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void UpdateStatus::set_has_current_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void UpdateStatus::clear_has_current_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void UpdateStatus::clear_current_state() {
  current_state_ = 0;
  clear_has_current_state();
}
inline ::xtech::selfomat::UpdateStatus_State UpdateStatus::current_state() const {
  // @@protoc_insertion_point(field_get:xtech.selfomat.UpdateStatus.current_state)
  return static_cast< ::xtech::selfomat::UpdateStatus_State >(current_state_);
}
inline void UpdateStatus::set_current_state(::xtech::selfomat::UpdateStatus_State value) {
  assert(::xtech::selfomat::UpdateStatus_State_IsValid(value));
  set_has_current_state();
  current_state_ = value;
  // @@protoc_insertion_point(field_set:xtech.selfomat.UpdateStatus.current_state)
}

// optional string current_version = 2;
inline bool UpdateStatus::has_current_version() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void UpdateStatus::set_has_current_version() {
  _has_bits_[0] |= 0x00000002u;
}
inline void UpdateStatus::clear_has_current_version() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void UpdateStatus::clear_current_version() {
  current_version_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_current_version();
}
inline const ::std::string& UpdateStatus::current_version() const {
  // @@protoc_insertion_point(field_get:xtech.selfomat.UpdateStatus.current_version)
  return current_version_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UpdateStatus::set_current_version(const ::std::string& value) {
  set_has_current_version();
  current_version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:xtech.selfomat.UpdateStatus.current_version)
}
inline void UpdateStatus::set_current_version(const char* value) {
  set_has_current_version();
  current_version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:xtech.selfomat.UpdateStatus.current_version)
}
inline void UpdateStatus::set_current_version(const char* value, size_t size) {
  set_has_current_version();
  current_version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:xtech.selfomat.UpdateStatus.current_version)
}
inline ::std::string* UpdateStatus::mutable_current_version() {
  set_has_current_version();
  // @@protoc_insertion_point(field_mutable:xtech.selfomat.UpdateStatus.current_version)
  return current_version_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* UpdateStatus::release_current_version() {
  // @@protoc_insertion_point(field_release:xtech.selfomat.UpdateStatus.current_version)
  clear_has_current_version();
  return current_version_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void UpdateStatus::set_allocated_current_version(::std::string* current_version) {
  if (current_version != NULL) {
    set_has_current_version();
  } else {
    clear_has_current_version();
  }
  current_version_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), current_version);
  // @@protoc_insertion_point(field_set_allocated:xtech.selfomat.UpdateStatus.current_version)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace selfomat
}  // namespace xtech

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::xtech::selfomat::UpdateStatus_State> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::xtech::selfomat::UpdateStatus_State>() {
  return ::xtech::selfomat::UpdateStatus_State_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_api_2eproto__INCLUDED