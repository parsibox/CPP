// Generated by the protocol buffer compiler.  DO NOT EDIT!

#include "UssdData.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format_inl.h>

namespace {

const ::google::protobuf::Descriptor* UssdData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  UssdData_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* UssdData_operation_type_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* UssdData_Msg_Type_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_UssdData_2eproto() {
  protobuf_AddDesc_UssdData_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "UssdData.proto");
  GOOGLE_CHECK(file != NULL);
  UssdData_descriptor_ = file->message_type(0);
  static const int UssdData_offsets_[17] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, version_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, dcs_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, oa_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, da_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, msg_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, op_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, msg_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, srv_code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, status_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, session_status_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, delivery_report_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, imsi_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, sender_session_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, receiver_session_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, username_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, password_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, app_id_),
  };
  UssdData_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      UssdData_descriptor_,
      UssdData::default_instance_,
      UssdData_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UssdData, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(UssdData));
  UssdData_operation_type_descriptor_ = UssdData_descriptor_->enum_type(0);
  UssdData_Msg_Type_descriptor_ = UssdData_descriptor_->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_UssdData_2eproto);
}

void protobuf_RegisterTypes() {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    UssdData_descriptor_, &UssdData::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_UssdData_2eproto() {
  delete UssdData::default_instance_;
  delete UssdData_reflection_;
}

void protobuf_AddDesc_UssdData_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016UssdData.proto\"\320\004\n\010UssdData\022\017\n\007version"
    "\030\001 \001(\r\022\013\n\003dcs\030\002 \001(\r\022\n\n\002oa\030\003 \001(\t\022\n\n\002da\030\004 "
    "\001(\t\022\013\n\003msg\030\005 \001(\014\022)\n\007op_type\030\006 \001(\0162\030.Ussd"
    "Data.operation_type\022$\n\010msg_type\030\007 \001(\0162\022."
    "UssdData.Msg_Type\022\020\n\010srv_code\030\010 \001(\t\022\016\n\006s"
    "tatus\030\t \001(\005\022\031\n\016session_status\030\n \001(\005:\0010\022\032"
    "\n\017delivery_report\030\013 \001(\005:\0010\022\014\n\004imsi\030\014 \001(\t"
    "\022\034\n\021sender_session_id\030\r \001(\005:\0010\022\036\n\023receiv"
    "er_session_id\030\016 \001(\005:\0010\022\020\n\010username\030\017 \001(\t"
    "\022\020\n\010password\030\020 \001(\t\022\016\n\006app_id\030\021 \001(\t\"\226\001\n\016o"
    "peration_type\022\t\n\005LOGIN\020\001\022\016\n\nLOGIN_RESP\020\002"
    "\022\n\n\006LOGOUT\020\003\022\017\n\013LOGOUT_RESP\020\004\022\t\n\005START\020\005"
    "\022\014\n\010CONTINUE\020\006\022\007\n\003END\020\007\022\t\n\005ABORT\020\010\022\014\n\010EN"
    "Q_LINK\020\t\022\021\n\rENQ_LINK_RESP\020\n\">\n\010Msg_Type\022"
    "\013\n\007REQUEST\020\001\022\n\n\006NOTIFY\020\002\022\014\n\010RESPONCE\020\003\022\013"
    "\n\007RELEASE\020\004", 611);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "UssdData.proto", &protobuf_RegisterTypes);
  UssdData::default_instance_ = new UssdData();
  UssdData::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_UssdData_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_UssdData_2eproto {
  StaticDescriptorInitializer_UssdData_2eproto() {
    protobuf_AddDesc_UssdData_2eproto();
  }
} static_descriptor_initializer_UssdData_2eproto_;


// ===================================================================

const ::google::protobuf::EnumDescriptor* UssdData_operation_type_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return UssdData_operation_type_descriptor_;
}
bool UssdData_operation_type_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const UssdData_operation_type UssdData::LOGIN;
const UssdData_operation_type UssdData::LOGIN_RESP;
const UssdData_operation_type UssdData::LOGOUT;
const UssdData_operation_type UssdData::LOGOUT_RESP;
const UssdData_operation_type UssdData::START;
const UssdData_operation_type UssdData::CONTINUE;
const UssdData_operation_type UssdData::END;
const UssdData_operation_type UssdData::ABORT;
const UssdData_operation_type UssdData::ENQ_LINK;
const UssdData_operation_type UssdData::ENQ_LINK_RESP;
const UssdData_operation_type UssdData::operation_type_MIN;
const UssdData_operation_type UssdData::operation_type_MAX;
#endif  // _MSC_VER
const ::google::protobuf::EnumDescriptor* UssdData_Msg_Type_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return UssdData_Msg_Type_descriptor_;
}
bool UssdData_Msg_Type_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const UssdData_Msg_Type UssdData::REQUEST;
const UssdData_Msg_Type UssdData::NOTIFY;
const UssdData_Msg_Type UssdData::RESPONCE;
const UssdData_Msg_Type UssdData::RELEASE;
const UssdData_Msg_Type UssdData::Msg_Type_MIN;
const UssdData_Msg_Type UssdData::Msg_Type_MAX;
#endif  // _MSC_VER
const ::std::string UssdData::_default_oa_;
const ::std::string UssdData::_default_da_;
const ::std::string UssdData::_default_msg_;
const ::std::string UssdData::_default_srv_code_;
const ::std::string UssdData::_default_imsi_;
const ::std::string UssdData::_default_username_;
const ::std::string UssdData::_default_password_;
const ::std::string UssdData::_default_app_id_;
#ifndef _MSC_VER
const int UssdData::kVersionFieldNumber;
const int UssdData::kDcsFieldNumber;
const int UssdData::kOaFieldNumber;
const int UssdData::kDaFieldNumber;
const int UssdData::kMsgFieldNumber;
const int UssdData::kOpTypeFieldNumber;
const int UssdData::kMsgTypeFieldNumber;
const int UssdData::kSrvCodeFieldNumber;
const int UssdData::kStatusFieldNumber;
const int UssdData::kSessionStatusFieldNumber;
const int UssdData::kDeliveryReportFieldNumber;
const int UssdData::kImsiFieldNumber;
const int UssdData::kSenderSessionIdFieldNumber;
const int UssdData::kReceiverSessionIdFieldNumber;
const int UssdData::kUsernameFieldNumber;
const int UssdData::kPasswordFieldNumber;
const int UssdData::kAppIdFieldNumber;
#endif  // !_MSC_VER

UssdData::UssdData()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void UssdData::InitAsDefaultInstance() {}

UssdData::UssdData(const UssdData& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void UssdData::SharedCtor() {
  _cached_size_ = 0;
  version_ = 0u;
  dcs_ = 0u;
  oa_ = const_cast< ::std::string*>(&_default_oa_);
  da_ = const_cast< ::std::string*>(&_default_da_);
  msg_ = const_cast< ::std::string*>(&_default_msg_);
  op_type_ = 1;
  msg_type_ = 1;
  srv_code_ = const_cast< ::std::string*>(&_default_srv_code_);
  status_ = 0;
  session_status_ = 0;
  delivery_report_ = 0;
  imsi_ = const_cast< ::std::string*>(&_default_imsi_);
  sender_session_id_ = 0;
  receiver_session_id_ = 0;
  username_ = const_cast< ::std::string*>(&_default_username_);
  password_ = const_cast< ::std::string*>(&_default_password_);
  app_id_ = const_cast< ::std::string*>(&_default_app_id_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

UssdData::~UssdData() {
  SharedDtor();
}

void UssdData::SharedDtor() {
  if (oa_ != &_default_oa_) {
    delete oa_;
  }
  if (da_ != &_default_da_) {
    delete da_;
  }
  if (msg_ != &_default_msg_) {
    delete msg_;
  }
  if (srv_code_ != &_default_srv_code_) {
    delete srv_code_;
  }
  if (imsi_ != &_default_imsi_) {
    delete imsi_;
  }
  if (username_ != &_default_username_) {
    delete username_;
  }
  if (password_ != &_default_password_) {
    delete password_;
  }
  if (app_id_ != &_default_app_id_) {
    delete app_id_;
  }
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* UssdData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return UssdData_descriptor_;
}

const UssdData& UssdData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_UssdData_2eproto();  return *default_instance_;
}

UssdData* UssdData::default_instance_ = NULL;

UssdData* UssdData::New() const {
  return new UssdData;
}

void UssdData::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    version_ = 0u;
    dcs_ = 0u;
    if (_has_bit(2)) {
      if (oa_ != &_default_oa_) {
        oa_->clear();
      }
    }
    if (_has_bit(3)) {
      if (da_ != &_default_da_) {
        da_->clear();
      }
    }
    if (_has_bit(4)) {
      if (msg_ != &_default_msg_) {
        msg_->clear();
      }
    }
    op_type_ = 1;
    msg_type_ = 1;
    if (_has_bit(7)) {
      if (srv_code_ != &_default_srv_code_) {
        srv_code_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    status_ = 0;
    session_status_ = 0;
    delivery_report_ = 0;
    if (_has_bit(11)) {
      if (imsi_ != &_default_imsi_) {
        imsi_->clear();
      }
    }
    sender_session_id_ = 0;
    receiver_session_id_ = 0;
    if (_has_bit(14)) {
      if (username_ != &_default_username_) {
        username_->clear();
      }
    }
    if (_has_bit(15)) {
      if (password_ != &_default_password_) {
        password_->clear();
      }
    }
  }
  if (_has_bits_[16 / 32] & (0xffu << (16 % 32))) {
    if (_has_bit(16)) {
      if (app_id_ != &_default_app_id_) {
        app_id_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool UssdData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormat::GetTagFieldNumber(tag)) {
      // optional uint32 version = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormat::ReadUInt32(
              input, &version_));
        _set_bit(0);
        if (input->ExpectTag(16)) goto parse_dcs;
        break;
      }
      
      // optional uint32 dcs = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_dcs:
        DO_(::google::protobuf::internal::WireFormat::ReadUInt32(
              input, &dcs_));
        _set_bit(1);
        if (input->ExpectTag(26)) goto parse_oa;
        break;
      }
      
      // optional string oa = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_oa:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_oa()));
        if (input->ExpectTag(34)) goto parse_da;
        break;
      }
      
      // optional string da = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_da:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_da()));
        if (input->ExpectTag(42)) goto parse_msg;
        break;
      }
      
      // optional bytes msg = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_msg:
        DO_(::google::protobuf::internal::WireFormat::ReadBytes(input, mutable_msg()));
        if (input->ExpectTag(48)) goto parse_op_type;
        break;
      }
      
      // optional .UssdData.operation_type op_type = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_op_type:
        int value;
        DO_(::google::protobuf::internal::WireFormat::ReadEnum(input, &value));
        if (::UssdData_operation_type_IsValid(value)) {
          set_op_type(static_cast< ::UssdData_operation_type >(value));
        } else {
          mutable_unknown_fields()->AddVarint(6, value);
        }
        if (input->ExpectTag(56)) goto parse_msg_type;
        break;
      }
      
      // optional .UssdData.Msg_Type msg_type = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_msg_type:
        int value;
        DO_(::google::protobuf::internal::WireFormat::ReadEnum(input, &value));
        if (::UssdData_Msg_Type_IsValid(value)) {
          set_msg_type(static_cast< ::UssdData_Msg_Type >(value));
        } else {
          mutable_unknown_fields()->AddVarint(7, value);
        }
        if (input->ExpectTag(66)) goto parse_srv_code;
        break;
      }
      
      // optional string srv_code = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_srv_code:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_srv_code()));
        if (input->ExpectTag(72)) goto parse_status;
        break;
      }
      
      // optional int32 status = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_status:
        DO_(::google::protobuf::internal::WireFormat::ReadInt32(
              input, &status_));
        _set_bit(8);
        if (input->ExpectTag(80)) goto parse_session_status;
        break;
      }
      
      // optional int32 session_status = 10 [default = 0];
      case 10: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_session_status:
        DO_(::google::protobuf::internal::WireFormat::ReadInt32(
              input, &session_status_));
        _set_bit(9);
        if (input->ExpectTag(88)) goto parse_delivery_report;
        break;
      }
      
      // optional int32 delivery_report = 11 [default = 0];
      case 11: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_delivery_report:
        DO_(::google::protobuf::internal::WireFormat::ReadInt32(
              input, &delivery_report_));
        _set_bit(10);
        if (input->ExpectTag(98)) goto parse_imsi;
        break;
      }
      
      // optional string imsi = 12;
      case 12: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_imsi:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_imsi()));
        if (input->ExpectTag(104)) goto parse_sender_session_id;
        break;
      }
      
      // optional int32 sender_session_id = 13 [default = 0];
      case 13: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_sender_session_id:
        DO_(::google::protobuf::internal::WireFormat::ReadInt32(
              input, &sender_session_id_));
        _set_bit(12);
        if (input->ExpectTag(112)) goto parse_receiver_session_id;
        break;
      }
      
      // optional int32 receiver_session_id = 14 [default = 0];
      case 14: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
       parse_receiver_session_id:
        DO_(::google::protobuf::internal::WireFormat::ReadInt32(
              input, &receiver_session_id_));
        _set_bit(13);
        if (input->ExpectTag(122)) goto parse_username;
        break;
      }
      
      // optional string username = 15;
      case 15: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_username:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_username()));
        if (input->ExpectTag(130)) goto parse_password;
        break;
      }
      
      // optional string password = 16;
      case 16: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_password:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_password()));
        if (input->ExpectTag(138)) goto parse_app_id;
        break;
      }
      
      // optional string app_id = 17;
      case 17: {
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormat::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_app_id:
        DO_(::google::protobuf::internal::WireFormat::ReadString(input, mutable_app_id()));
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormat::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormat::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void UssdData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    UssdData::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional uint32 version = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::WriteUInt32(1, this->version(), output);
  }
  
  // optional uint32 dcs = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::WriteUInt32(2, this->dcs(), output);
  }
  
  // optional string oa = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::WriteString(3, this->oa(), output);
  }
  
  // optional string da = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormat::WriteString(4, this->da(), output);
  }
  
  // optional bytes msg = 5;
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormat::WriteBytes(5, this->msg(), output);
  }
  
  // optional .UssdData.operation_type op_type = 6;
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormat::WriteEnum(6, this->op_type(), output);
  }
  
  // optional .UssdData.Msg_Type msg_type = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormat::WriteEnum(7, this->msg_type(), output);
  }
  
  // optional string srv_code = 8;
  if (_has_bit(7)) {
    ::google::protobuf::internal::WireFormat::WriteString(8, this->srv_code(), output);
  }
  
  // optional int32 status = 9;
  if (_has_bit(8)) {
    ::google::protobuf::internal::WireFormat::WriteInt32(9, this->status(), output);
  }
  
  // optional int32 session_status = 10 [default = 0];
  if (_has_bit(9)) {
    ::google::protobuf::internal::WireFormat::WriteInt32(10, this->session_status(), output);
  }
  
  // optional int32 delivery_report = 11 [default = 0];
  if (_has_bit(10)) {
    ::google::protobuf::internal::WireFormat::WriteInt32(11, this->delivery_report(), output);
  }
  
  // optional string imsi = 12;
  if (_has_bit(11)) {
    ::google::protobuf::internal::WireFormat::WriteString(12, this->imsi(), output);
  }
  
  // optional int32 sender_session_id = 13 [default = 0];
  if (_has_bit(12)) {
    ::google::protobuf::internal::WireFormat::WriteInt32(13, this->sender_session_id(), output);
  }
  
  // optional int32 receiver_session_id = 14 [default = 0];
  if (_has_bit(13)) {
    ::google::protobuf::internal::WireFormat::WriteInt32(14, this->receiver_session_id(), output);
  }
  
  // optional string username = 15;
  if (_has_bit(14)) {
    ::google::protobuf::internal::WireFormat::WriteString(15, this->username(), output);
  }
  
  // optional string password = 16;
  if (_has_bit(15)) {
    ::google::protobuf::internal::WireFormat::WriteString(16, this->password(), output);
  }
  
  // optional string app_id = 17;
  if (_has_bit(16)) {
    ::google::protobuf::internal::WireFormat::WriteString(17, this->app_id(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* UssdData::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional uint32 version = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormat::WriteUInt32ToArray(1, this->version(), target);
  }
  
  // optional uint32 dcs = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormat::WriteUInt32ToArray(2, this->dcs(), target);
  }
  
  // optional string oa = 3;
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(3, this->oa(), target);
  }
  
  // optional string da = 4;
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(4, this->da(), target);
  }
  
  // optional bytes msg = 5;
  if (_has_bit(4)) {
    target = ::google::protobuf::internal::WireFormat::WriteBytesToArray(5, this->msg(), target);
  }
  
  // optional .UssdData.operation_type op_type = 6;
  if (_has_bit(5)) {
    target = ::google::protobuf::internal::WireFormat::WriteEnumToArray(6, this->op_type(), target);
  }
  
  // optional .UssdData.Msg_Type msg_type = 7;
  if (_has_bit(6)) {
    target = ::google::protobuf::internal::WireFormat::WriteEnumToArray(7, this->msg_type(), target);
  }
  
  // optional string srv_code = 8;
  if (_has_bit(7)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(8, this->srv_code(), target);
  }
  
  // optional int32 status = 9;
  if (_has_bit(8)) {
    target = ::google::protobuf::internal::WireFormat::WriteInt32ToArray(9, this->status(), target);
  }
  
  // optional int32 session_status = 10 [default = 0];
  if (_has_bit(9)) {
    target = ::google::protobuf::internal::WireFormat::WriteInt32ToArray(10, this->session_status(), target);
  }
  
  // optional int32 delivery_report = 11 [default = 0];
  if (_has_bit(10)) {
    target = ::google::protobuf::internal::WireFormat::WriteInt32ToArray(11, this->delivery_report(), target);
  }
  
  // optional string imsi = 12;
  if (_has_bit(11)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(12, this->imsi(), target);
  }
  
  // optional int32 sender_session_id = 13 [default = 0];
  if (_has_bit(12)) {
    target = ::google::protobuf::internal::WireFormat::WriteInt32ToArray(13, this->sender_session_id(), target);
  }
  
  // optional int32 receiver_session_id = 14 [default = 0];
  if (_has_bit(13)) {
    target = ::google::protobuf::internal::WireFormat::WriteInt32ToArray(14, this->receiver_session_id(), target);
  }
  
  // optional string username = 15;
  if (_has_bit(14)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(15, this->username(), target);
  }
  
  // optional string password = 16;
  if (_has_bit(15)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(16, this->password(), target);
  }
  
  // optional string app_id = 17;
  if (_has_bit(16)) {
    target = ::google::protobuf::internal::WireFormat::WriteStringToArray(17, this->app_id(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int UssdData::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional uint32 version = 1;
    if (has_version()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::UInt32Size(
          this->version());
    }
    
    // optional uint32 dcs = 2;
    if (has_dcs()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::UInt32Size(
          this->dcs());
    }
    
    // optional string oa = 3;
    if (has_oa()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::StringSize(this->oa());
    }
    
    // optional string da = 4;
    if (has_da()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::StringSize(this->da());
    }
    
    // optional bytes msg = 5;
    if (has_msg()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::BytesSize(this->msg());
    }
    
    // optional .UssdData.operation_type op_type = 6;
    if (has_op_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::EnumSize(this->op_type());
    }
    
    // optional .UssdData.Msg_Type msg_type = 7;
    if (has_msg_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::EnumSize(this->msg_type());
    }
    
    // optional string srv_code = 8;
    if (has_srv_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::StringSize(this->srv_code());
    }
    
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional int32 status = 9;
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::Int32Size(
          this->status());
    }
    
    // optional int32 session_status = 10 [default = 0];
    if (has_session_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::Int32Size(
          this->session_status());
    }
    
    // optional int32 delivery_report = 11 [default = 0];
    if (has_delivery_report()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::Int32Size(
          this->delivery_report());
    }
    
    // optional string imsi = 12;
    if (has_imsi()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::StringSize(this->imsi());
    }
    
    // optional int32 sender_session_id = 13 [default = 0];
    if (has_sender_session_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::Int32Size(
          this->sender_session_id());
    }
    
    // optional int32 receiver_session_id = 14 [default = 0];
    if (has_receiver_session_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::Int32Size(
          this->receiver_session_id());
    }
    
    // optional string username = 15;
    if (has_username()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormat::StringSize(this->username());
    }
    
    // optional string password = 16;
    if (has_password()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormat::StringSize(this->password());
    }
    
  }
  if (_has_bits_[16 / 32] & (0xffu << (16 % 32))) {
    // optional string app_id = 17;
    if (has_app_id()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormat::StringSize(this->app_id());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  _cached_size_ = total_size;
  return total_size;
}

void UssdData::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const UssdData* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const UssdData*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void UssdData::MergeFrom(const UssdData& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_version(from.version());
    }
    if (from._has_bit(1)) {
      set_dcs(from.dcs());
    }
    if (from._has_bit(2)) {
      set_oa(from.oa());
    }
    if (from._has_bit(3)) {
      set_da(from.da());
    }
    if (from._has_bit(4)) {
      set_msg(from.msg());
    }
    if (from._has_bit(5)) {
      set_op_type(from.op_type());
    }
    if (from._has_bit(6)) {
      set_msg_type(from.msg_type());
    }
    if (from._has_bit(7)) {
      set_srv_code(from.srv_code());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from._has_bit(8)) {
      set_status(from.status());
    }
    if (from._has_bit(9)) {
      set_session_status(from.session_status());
    }
    if (from._has_bit(10)) {
      set_delivery_report(from.delivery_report());
    }
    if (from._has_bit(11)) {
      set_imsi(from.imsi());
    }
    if (from._has_bit(12)) {
      set_sender_session_id(from.sender_session_id());
    }
    if (from._has_bit(13)) {
      set_receiver_session_id(from.receiver_session_id());
    }
    if (from._has_bit(14)) {
      set_username(from.username());
    }
    if (from._has_bit(15)) {
      set_password(from.password());
    }
  }
  if (from._has_bits_[16 / 32] & (0xffu << (16 % 32))) {
    if (from._has_bit(16)) {
      set_app_id(from.app_id());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void UssdData::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UssdData::CopyFrom(const UssdData& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UssdData::Swap(UssdData* other) {
  if (other != this) {
    std::swap(version_, other->version_);
    std::swap(dcs_, other->dcs_);
    std::swap(oa_, other->oa_);
    std::swap(da_, other->da_);
    std::swap(msg_, other->msg_);
    std::swap(op_type_, other->op_type_);
    std::swap(msg_type_, other->msg_type_);
    std::swap(srv_code_, other->srv_code_);
    std::swap(status_, other->status_);
    std::swap(session_status_, other->session_status_);
    std::swap(delivery_report_, other->delivery_report_);
    std::swap(imsi_, other->imsi_);
    std::swap(sender_session_id_, other->sender_session_id_);
    std::swap(receiver_session_id_, other->receiver_session_id_);
    std::swap(username_, other->username_);
    std::swap(password_, other->password_);
    std::swap(app_id_, other->app_id_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

bool UssdData::IsInitialized() const {
  
  return true;
}

const ::google::protobuf::Descriptor* UssdData::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* UssdData::GetReflection() const {
  protobuf_AssignDescriptorsOnce();
  return UssdData_reflection_;
}
