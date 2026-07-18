// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros2_robot_middleware:action/MoveToPose.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
#include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
#include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_Goal__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_Goal__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_member_array[4] = {
  {
    "target_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Goal, target_x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Goal, target_y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_theta",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Goal, target_theta),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_speed",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Goal, max_speed),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_Goal",  // message name
  4,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_Goal),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_Goal__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_Goal__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_Goal__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Goal)() {
  if (!ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_Goal__rosidl_typesupport_introspection_c__MoveToPose_Goal_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_Result__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_Result__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_member_array[4] = {
  {
    "reached",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Result, reached),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "final_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Result, final_x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "final_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Result, final_y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "elapsed_time",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Result, elapsed_time),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_Result",  // message name
  4,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_Result),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_Result__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_Result__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_Result__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Result)() {
  if (!ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_Result__rosidl_typesupport_introspection_c__MoveToPose_Result_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_Feedback__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_Feedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_member_array[4] = {
  {
    "current_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Feedback, current_x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Feedback, current_y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "distance_remaining",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Feedback, distance_remaining),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "percent_complete",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_Feedback, percent_complete),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_Feedback",  // message name
  4,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_Feedback),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_Feedback__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_Feedback__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_Feedback__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Feedback)() {
  if (!ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_Feedback__rosidl_typesupport_introspection_c__MoveToPose_Feedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `goal`
#include "ros2_robot_middleware/action/move_to_pose.h"
// Member `goal`
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "goal",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Request, goal),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_SendGoal_Request),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Request)() {
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Goal)();
  if (!ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Response, accepted),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Response, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_SendGoal_Response),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Response)() {
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "ros2_robot_middleware/action/move_to_pose.h"
// Member `request`
// Member `response`
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__fini(message_memory);
}

size_t ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_SendGoal_Event__request(
  const void * untyped_member)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__request(
  const void * untyped_member, size_t index)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__request(
  void * untyped_member, size_t index)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Request * item =
    ((const ros2_robot_middleware__action__MoveToPose_SendGoal_Request *)
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__request(untyped_member, index));
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request * value =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Request *)(untyped_value);
  *value = *item;
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request * item =
    ((ros2_robot_middleware__action__MoveToPose_SendGoal_Request *)
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__request(untyped_member, index));
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Request * value =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Request *)(untyped_value);
  *item = *value;
}

bool ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_SendGoal_Event__request(
  void * untyped_member, size_t size)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence *)(untyped_member);
  ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence__fini(member);
  return ros2_robot_middleware__action__MoveToPose_SendGoal_Request__Sequence__init(member, size);
}

size_t ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_SendGoal_Event__response(
  const void * untyped_member)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__response(
  const void * untyped_member, size_t index)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__response(
  void * untyped_member, size_t index)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Response * item =
    ((const ros2_robot_middleware__action__MoveToPose_SendGoal_Response *)
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__response(untyped_member, index));
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response * value =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Response *)(untyped_value);
  *value = *item;
}

void ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response * item =
    ((ros2_robot_middleware__action__MoveToPose_SendGoal_Response *)
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__response(untyped_member, index));
  const ros2_robot_middleware__action__MoveToPose_SendGoal_Response * value =
    (const ros2_robot_middleware__action__MoveToPose_SendGoal_Response *)(untyped_value);
  *item = *value;
}

bool ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_SendGoal_Event__response(
  void * untyped_member, size_t size)
{
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence *)(untyped_member);
  ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence__fini(member);
  return ros2_robot_middleware__action__MoveToPose_SendGoal_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Event, request),  // bytes offset in struct
    NULL,  // default value
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_SendGoal_Event__request,  // size() function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__request,  // get_const(index) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__request,  // get(index) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_SendGoal_Event__request,  // fetch(index, &value) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_SendGoal_Event__request,  // assign(index, value) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_SendGoal_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_SendGoal_Event, response),  // bytes offset in struct
    NULL,  // default value
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_SendGoal_Event__response,  // size() function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_SendGoal_Event__response,  // get_const(index) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_SendGoal_Event__response,  // get(index) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_SendGoal_Event__response,  // fetch(index, &value) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_SendGoal_Event__response,  // assign(index, value) function pointer
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_SendGoal_Event),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Event)() {
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Request)();
  ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Response)();
  if (!ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_members = {
  "ros2_robot_middleware__action",  // service namespace
  "MoveToPose_SendGoal",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle,
  NULL,  // response message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle
  NULL  // event_message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle
};


static rosidl_service_type_support_t ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_type_support_handle = {
  0,
  &ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_members,
  get_service_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Request__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Request_message_type_support_handle,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Response__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Response_message_type_support_handle,
  &ros2_robot_middleware__action__MoveToPose_SendGoal_Event__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_robot_middleware,
    action,
    MoveToPose_SendGoal
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_robot_middleware,
    action,
    MoveToPose_SendGoal
  ),
  &ros2_robot_middleware__action__MoveToPose_SendGoal__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_SendGoal__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_SendGoal__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal)(void) {
  if (!ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_SendGoal_Event)()->data;
  }

  return &ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_SendGoal_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_GetResult_Request),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Request__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Request__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Request)() {
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  if (!ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `result`
// already included above
// #include "ros2_robot_middleware/action/move_to_pose.h"
// Member `result`
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Response, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_GetResult_Response),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Response__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Response__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Response)() {
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Result)();
  if (!ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `info`
// already included above
// #include "service_msgs/msg/service_event_info.h"
// Member `info`
// already included above
// #include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "ros2_robot_middleware/action/move_to_pose.h"
// Member `request`
// Member `response`
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__fini(message_memory);
}

size_t ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_GetResult_Event__request(
  const void * untyped_member)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__request(
  const void * untyped_member, size_t index)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__request(
  void * untyped_member, size_t index)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Request * item =
    ((const ros2_robot_middleware__action__MoveToPose_GetResult_Request *)
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__request(untyped_member, index));
  ros2_robot_middleware__action__MoveToPose_GetResult_Request * value =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Request *)(untyped_value);
  *value = *item;
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Request * item =
    ((ros2_robot_middleware__action__MoveToPose_GetResult_Request *)
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__request(untyped_member, index));
  const ros2_robot_middleware__action__MoveToPose_GetResult_Request * value =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Request *)(untyped_value);
  *item = *value;
}

bool ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_GetResult_Event__request(
  void * untyped_member, size_t size)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence *)(untyped_member);
  ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence__fini(member);
  return ros2_robot_middleware__action__MoveToPose_GetResult_Request__Sequence__init(member, size);
}

size_t ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_GetResult_Event__response(
  const void * untyped_member)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__response(
  const void * untyped_member, size_t index)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence * member =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__response(
  void * untyped_member, size_t index)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros2_robot_middleware__action__MoveToPose_GetResult_Response * item =
    ((const ros2_robot_middleware__action__MoveToPose_GetResult_Response *)
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__response(untyped_member, index));
  ros2_robot_middleware__action__MoveToPose_GetResult_Response * value =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Response *)(untyped_value);
  *value = *item;
}

void ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Response * item =
    ((ros2_robot_middleware__action__MoveToPose_GetResult_Response *)
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__response(untyped_member, index));
  const ros2_robot_middleware__action__MoveToPose_GetResult_Response * value =
    (const ros2_robot_middleware__action__MoveToPose_GetResult_Response *)(untyped_value);
  *item = *value;
}

bool ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_GetResult_Event__response(
  void * untyped_member, size_t size)
{
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence * member =
    (ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence *)(untyped_member);
  ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence__fini(member);
  return ros2_robot_middleware__action__MoveToPose_GetResult_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Event, request),  // bytes offset in struct
    NULL,  // default value
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_GetResult_Event__request,  // size() function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__request,  // get_const(index) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__request,  // get(index) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_GetResult_Event__request,  // fetch(index, &value) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_GetResult_Event__request,  // assign(index, value) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_GetResult_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_GetResult_Event, response),  // bytes offset in struct
    NULL,  // default value
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveToPose_GetResult_Event__response,  // size() function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveToPose_GetResult_Event__response,  // get_const(index) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveToPose_GetResult_Event__response,  // get(index) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveToPose_GetResult_Event__response,  // fetch(index, &value) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveToPose_GetResult_Event__response,  // assign(index, value) function pointer
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveToPose_GetResult_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_GetResult_Event),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Event__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Event__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Event)() {
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Request)();
  ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Response)();
  if (!ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_members = {
  "ros2_robot_middleware__action",  // service namespace
  "MoveToPose_GetResult",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle,
  NULL,  // response message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle
  NULL  // event_message
  // ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle
};


static rosidl_service_type_support_t ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_type_support_handle = {
  0,
  &ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_members,
  get_service_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Request__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Request_message_type_support_handle,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Response__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Response_message_type_support_handle,
  &ros2_robot_middleware__action__MoveToPose_GetResult_Event__rosidl_typesupport_introspection_c__MoveToPose_GetResult_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_robot_middleware,
    action,
    MoveToPose_GetResult
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros2_robot_middleware,
    action,
    MoveToPose_GetResult
  ),
  &ros2_robot_middleware__action__MoveToPose_GetResult__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_GetResult__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_GetResult__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult)(void) {
  if (!ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_GetResult_Event)()->data;
  }

  return &ros2_robot_middleware__action__detail__move_to_pose__rosidl_typesupport_introspection_c__MoveToPose_GetResult_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros2_robot_middleware/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__functions.h"
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `feedback`
// already included above
// #include "ros2_robot_middleware/action/move_to_pose.h"
// Member `feedback`
// already included above
// #include "ros2_robot_middleware/action/detail/move_to_pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__init(message_memory);
}

void ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_fini_function(void * message_memory)
{
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_FeedbackMessage, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "feedback",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros2_robot_middleware__action__MoveToPose_FeedbackMessage, feedback),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_members = {
  "ros2_robot_middleware__action",  // message namespace
  "MoveToPose_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(ros2_robot_middleware__action__MoveToPose_FeedbackMessage),
  false,  // has_any_key_member_
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_member_array,  // message members
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_type_support_handle = {
  0,
  &ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &ros2_robot_middleware__action__MoveToPose_FeedbackMessage__get_type_hash,
  &ros2_robot_middleware__action__MoveToPose_FeedbackMessage__get_type_description,
  &ros2_robot_middleware__action__MoveToPose_FeedbackMessage__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros2_robot_middleware
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_FeedbackMessage)() {
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros2_robot_middleware, action, MoveToPose_Feedback)();
  if (!ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_type_support_handle.typesupport_identifier) {
    ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros2_robot_middleware__action__MoveToPose_FeedbackMessage__rosidl_typesupport_introspection_c__MoveToPose_FeedbackMessage_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
