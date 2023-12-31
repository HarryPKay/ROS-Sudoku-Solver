# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "move_arm_joints: 0 messages, 1 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(move_arm_joints_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_custom_target(_move_arm_joints_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "move_arm_joints" "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(move_arm_joints
  "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/move_arm_joints
)

### Generating Module File
_generate_module_cpp(move_arm_joints
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/move_arm_joints
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(move_arm_joints_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(move_arm_joints_generate_messages move_arm_joints_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_dependencies(move_arm_joints_generate_messages_cpp _move_arm_joints_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(move_arm_joints_gencpp)
add_dependencies(move_arm_joints_gencpp move_arm_joints_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS move_arm_joints_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(move_arm_joints
  "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/move_arm_joints
)

### Generating Module File
_generate_module_eus(move_arm_joints
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/move_arm_joints
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(move_arm_joints_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(move_arm_joints_generate_messages move_arm_joints_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_dependencies(move_arm_joints_generate_messages_eus _move_arm_joints_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(move_arm_joints_geneus)
add_dependencies(move_arm_joints_geneus move_arm_joints_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS move_arm_joints_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(move_arm_joints
  "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/move_arm_joints
)

### Generating Module File
_generate_module_lisp(move_arm_joints
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/move_arm_joints
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(move_arm_joints_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(move_arm_joints_generate_messages move_arm_joints_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_dependencies(move_arm_joints_generate_messages_lisp _move_arm_joints_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(move_arm_joints_genlisp)
add_dependencies(move_arm_joints_genlisp move_arm_joints_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS move_arm_joints_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(move_arm_joints
  "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/move_arm_joints
)

### Generating Module File
_generate_module_nodejs(move_arm_joints
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/move_arm_joints
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(move_arm_joints_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(move_arm_joints_generate_messages move_arm_joints_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_dependencies(move_arm_joints_generate_messages_nodejs _move_arm_joints_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(move_arm_joints_gennodejs)
add_dependencies(move_arm_joints_gennodejs move_arm_joints_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS move_arm_joints_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(move_arm_joints
  "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/move_arm_joints
)

### Generating Module File
_generate_module_py(move_arm_joints
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/move_arm_joints
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(move_arm_joints_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(move_arm_joints_generate_messages move_arm_joints_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/sudoku_bot_ws/src/move_arm_joints/srv/move_and_confirm.srv" NAME_WE)
add_dependencies(move_arm_joints_generate_messages_py _move_arm_joints_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(move_arm_joints_genpy)
add_dependencies(move_arm_joints_genpy move_arm_joints_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS move_arm_joints_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/move_arm_joints)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/move_arm_joints
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(move_arm_joints_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/move_arm_joints)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/move_arm_joints
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(move_arm_joints_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/move_arm_joints)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/move_arm_joints
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(move_arm_joints_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/move_arm_joints)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/move_arm_joints
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(move_arm_joints_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/move_arm_joints)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/move_arm_joints\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/move_arm_joints
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(move_arm_joints_generate_messages_py std_msgs_generate_messages_py)
endif()
