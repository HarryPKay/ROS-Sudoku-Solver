// Generated by gencpp from file move_arm_joints/move_and_confirmRequest.msg
// DO NOT EDIT!


#ifndef MOVE_ARM_JOINTS_MESSAGE_MOVE_AND_CONFIRMREQUEST_H
#define MOVE_ARM_JOINTS_MESSAGE_MOVE_AND_CONFIRMREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace move_arm_joints
{
template <class ContainerAllocator>
struct move_and_confirmRequest_
{
  typedef move_and_confirmRequest_<ContainerAllocator> Type;

  move_and_confirmRequest_()
    : move(0.0)  {
    }
  move_and_confirmRequest_(const ContainerAllocator& _alloc)
    : move(0.0)  {
  (void)_alloc;
    }



   typedef double _move_type;
  _move_type move;





  typedef boost::shared_ptr< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> const> ConstPtr;

}; // struct move_and_confirmRequest_

typedef ::move_arm_joints::move_and_confirmRequest_<std::allocator<void> > move_and_confirmRequest;

typedef boost::shared_ptr< ::move_arm_joints::move_and_confirmRequest > move_and_confirmRequestPtr;
typedef boost::shared_ptr< ::move_arm_joints::move_and_confirmRequest const> move_and_confirmRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace move_arm_joints

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ceacabbaabc689d773d1812184680153";
  }

  static const char* value(const ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xceacabbaabc689d7ULL;
  static const uint64_t static_value2 = 0x73d1812184680153ULL;
};

template<class ContainerAllocator>
struct DataType< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "move_arm_joints/move_and_confirmRequest";
  }

  static const char* value(const ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 move\n\
";
  }

  static const char* value(const ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.move);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct move_and_confirmRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::move_arm_joints::move_and_confirmRequest_<ContainerAllocator>& v)
  {
    s << indent << "move: ";
    Printer<double>::stream(s, indent + "  ", v.move);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MOVE_ARM_JOINTS_MESSAGE_MOVE_AND_CONFIRMREQUEST_H