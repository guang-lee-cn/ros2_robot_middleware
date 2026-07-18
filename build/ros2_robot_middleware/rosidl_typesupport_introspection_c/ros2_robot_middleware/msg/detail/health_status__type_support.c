// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros2_robot_middleware:msg/HealthStatus.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros2_robot_middleware/msg/detail/health_status__rosidl_typesupport_introspection_c.h"
#include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros2_robot_middleware/msg/detail/health_status__functions.h"
#include "ros2_robot_middleware/msg/detail/health_status__struct.h"


// Include directives for member types
// Member `node_name`
// Member `status`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__msg__HealthStatus__init(message_memory);
}

void ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_fini_function(void * message_memory)
{
  ros2_robot_middleware__msg__HealthStatus__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_member_array[4] = {
  {
    "node_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthStatus, node_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthStatus, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "last_seen_s",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthStatus, last_seen_s),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "timeout_s",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthStatus, timeout_s),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_members = {
  "ros2_robot_middleware__msg",  // message namespace
  "HealthStatus",  // message name
  4,  // number of fields
  sizeof(ros2_robot_middleware__msg__HealthStatus),
  false,  // has_any_key_member_
  ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_member_array,  // message members
  ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_type_support_handle = {
  0,
  &ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__msg__HealthStatus__get_type_hash,
  &ros2_robot_middleware__msg__HealthStatus__get_type_description,
  &ros2_robot_middleware__msg__HealthStatus__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, msg, HealthStatus)() {
  if (!ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__msg__HealthStatus__rosidl_typesupport_introspection_c__HealthStatus_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
