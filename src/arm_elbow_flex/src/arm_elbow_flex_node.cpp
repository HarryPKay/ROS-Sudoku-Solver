#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/Float64.h>

#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#include <fstream>
#include <sstream>
#include <string>

#include <move_arm_joints/move_and_confirm.h>

using namespace std;
using namespace move_arm_joints;

const string jointPrefix = "_joint";
const string workSpaceName = "sudoku_bot_ws";
const string homeEnvVar = "HOME";
const string prefix = "arm_elbow_flex";
const string NodeSuffix = "_joint_node";
const string serviceSuffix = "_joint_service";
const string servoName = prefix + "_servo";
const string servoLogName = prefix + "_servo_confirm.log";
const string shutdownService = prefix + "_shutdown_service";
const string publisherName = "/" + prefix + jointPrefix + "/command";

const int sleepDuration = 3;
const int exitDelay = 600;
const int precision = 4;
const int gridSize = 100;


const float marginOfError = 0.0025;

string servoCommand = ""; // Must be global for callback
string pkgSrcPath = ""; // Must be global for callback
ros::Publisher pub;

bool withInRange(float lValue, float rValue, float error)
{
    return fabs(lValue - rValue) < error;
}

void fail(const string& msg)
{
    ROS_ERROR_STREAM(prefix << ": " << msg);
    ROS_INFO_STREAM(prefix << ": " << "Shutting down in " << exitDelay << "s.");
    ros::Duration(exitDelay).sleep();
    ros::shutdown();
    exit(EXIT_FAILURE);
}

// Try to get home from environ variable so that the
// program knows where to find servo, sensor and log files.
string getHomePath()
{
    const char *homePath;

    if ((homePath = getenv(homeEnvVar.c_str())) == NULL)
    {
        homePath = getpwuid(getuid())->pw_dir;
    }
    if (homePath == NULL)
    {
        fail("Error finding home directory");
    }

    return ((string)homePath) + "/";
}

void readSensor(float &fValue, int precision)
{
    ifstream file;
    file.open(pkgSrcPath + servoLogName);
    std::stringstream ss;
    string intermediate;

    if (file.is_open())
    {
        getline(file, intermediate);
        file.close();
    }
    else
    {
        fail("Error opening " + pkgSrcPath + servoLogName);
    }

    ss << std::fixed << std::setprecision(precision) << intermediate;
    fValue = stof(ss.str());
}

// Checks if the log contains a different value than last time read.
// From this, infers whether the arm is probably moving or not.
bool checkServoBusy()
{
    static float lastValue = 0;
    static float newValue = 0;

    readSensor(newValue, precision);

    if (lastValue != newValue)
    {
        lastValue = newValue;
        return true;
    }
    return false;
}

// Read req, tell servo, then wait before reading sensor and returning that value in resp.
bool serviceCallBack(move_and_confirm::Request &req, move_and_confirm::Response &resp)
{
    // Extract to precision the move and tell it to servor if it is not busy.
    float move = req.move;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << to_string(move);

    // Don't send command if servo is busy to prevent overloading it.
    //bool isServerBusy = checkServoBusy();
    //if (!isServerBusy)
    //{
        //string servoCommandWithArg = servoCommand + " " + ss.str();
        //system(servoCommandWithArg.c_str());
        std_msgs::Float64 msg;
        msg.data = req.move;
        pub.publish(msg);
        resp.confirm = req.move;
        ROS_DEBUG_STREAM(prefix << ": " << "Received request with " << req.move << ", responding with "  << resp.confirm);
    //}

    ros::Duration(sleepDuration).sleep();
    //float value = 0.0;
    //readSensor(value, precision);
    //resp.confirm = value;
//    if (isServerBusy || resp.confirm == req.move)
//    {
//        ROS_DEBUG_STREAM(prefix << ": " << "Received request with " << req.move << ", responding with "  << resp.confirm);
//    }
//    else
//    {
//        ROS_DEBUG_STREAM(prefix << ": " << "Received request with " << req.move << ", responding with "  << resp.confirm << " (servo was busy)");
//    }

    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, prefix + NodeSuffix);
    ros::NodeHandle nh;

    if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged();
    }

    string homePath = getHomePath();
    pkgSrcPath = homePath + workSpaceName + "/src/" + prefix + "/src/";
    servoCommand = "cd " + pkgSrcPath + " && ./" + servoName;

    ros::ServiceServer arm_elbow_flex_joint_service = nh.advertiseService(prefix + serviceSuffix, &serviceCallBack);
    pub = nh.advertise<std_msgs::Float64>(publisherName, gridSize * gridSize);


    ROS_INFO_STREAM(prefix << ": Ready.");

    ros::spin();

    ros::Duration(exitDelay).sleep();
    ros::shutdown;
    return EXIT_SUCCESS;
}
