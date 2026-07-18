// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros2_robot_middleware:msg/HealthReport.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros2_robot_middleware/msg/detail/health_report__rosidl_typesupport_introspection_c.h"
#include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros2_robot_middleware/msg/detail/health_report__functions.h"
#include "ros2_robot_middleware/msg/detail/health_report__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `nodes`
#include "ros2_robot_middleware/msg/health_status.h"
// Member `nodes`
#include "ros2_robot_middleware/msg/detail/health_status__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__msg__HealthReport__init(message_memory);
}

void ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_fini_function(void * message_memory)
{
  ros2_robot_middleware__msg__HealthReport__fini(message_memory);
}

size_t ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__size_function__HealthReport__nodes(
  const void * untyped_member)
{
  const ros2_robot_middleware__msg__HealthStatus__Sequence * member =
    (const ros2_robot_middleware__msg__HealthStatus__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_const_function__HealthReport__nodes(
  const void * untyped_member, size_t index)
{
  const ros2_robot_middleware__msg__HealthStatus__Sequence * member =
    (const ros2_robot_middleware__msg__HealthStatus__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_function__HealthReport__nodes(
  void * untyped_member, size_t index)
{
  ros2_robot_middleware__msg__HealthStatus__Sequence * member =
    (ros2_robot_middleware__msg__HealthStatus__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__fetch_function__HealthReport__nodes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_robot_middleware__msg__HealthStatus * item =
    ((const ros2_robot_middleware__msg__HealthStatus *)
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_const_function__HealthReport__nodes(untyped_member, index));
  ros2_robot_middleware__msg__HealthStatus * value =
    (ros2_robot_middleware__msg__HealthStatus *)(untyped_value);
  *value = *item;
}

void ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__assign_function__HealthReport__nodes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_robot_middleware__msg__HealthStatus * item =
    ((ros2_robot_middleware__msg__HealthStatus *)
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_function__HealthReport__nodes(untyped_member, index));
  const ros2_robot_middleware__msg__HealthStatus * value =
    (const ros2_robot_middleware__msg__HealthStatus *)(untyped_value);
  *item = *value;
}

bool ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__resize_function__HealthReport__nodes(
  void * untyped_member, size_t size)
{
  ros2_robot_middleware__msg__HealthStatus__Sequence * member =
    (ros2_robot_middleware__msg__HealthStatus__Sequence *)(untyped_member);
  ros2_robot_middleware__msg__HealthStatus__Sequence__fini(member);
  return ros2_robot_middleware__msg__HealthStatus__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthReport, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "nodes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__msg__HealthReport, nodes),  // bytes offset in struct
    NULL,  // default value
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__size_function__HealthReport__nodes,  // size() function pointer
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_const_function__HealthReport__nodes,  // get_const(index) function pointer
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__get_function__HealthReport__nodes,  // get(index) function pointer
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__fetch_function__HealthReport__nodes,  // fetch(index, &value) function pointer
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__assign_function__HealthReport__nodes,  // assign(index, value) function pointer
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__resize_function__HealthReport__nodes  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_members = {
  "ros2_robot_middleware__msg",  // message namespace
  "HealthReport",  // message name
  2,  // number of fields
  sizeof(ros2_robot_middleware__msg__HealthReport),
  false,  // has_any_key_member_
  ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_member_array,  // message members
  ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_type_support_handle = {
  0,
  &ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__msg__HealthReport__get_type_hash,
  &ros2_robot_middleware__msg__HealthReport__get_type_description,
  &ros2_robot_middleware__msg__HealthReport__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, msg, HealthReport)() {
  ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, msg, HealthStatus)();
  if (!ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__msg__HealthReport__rosidl_typesupport_introspection_c__HealthReport_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
