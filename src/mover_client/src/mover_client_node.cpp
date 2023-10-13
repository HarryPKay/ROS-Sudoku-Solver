#include <ros/ros.h>
#include <ros/console.h>

#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <mover_client/grid_num.h>
#include <mover_client/grid_num_vector.h>
#include <move_arm_joints/move_and_confirm.h>

using namespace std;
using namespace ros;
using namespace mover_client;
using namespace move_arm_joints;

typedef unordered_map<string, float> JointNameToValue;
typedef unordered_map<string, JointNameToValue> CellToJointValue;
typedef unordered_map<string, ServiceClient*> jointNameToClient;

const int hertz = 10;
const int checkDuration = 2;
const int waitDuration = 5;
const int precision = 4;
const int exitDelay = 600;
const int gridNumVectorSize = 3;
const int gridSize = 100;

const string workSpaceName = "sudoku_bot_ws";
const string homeEnvVar = "HOME";
const string serviceSuffix = "_joint_service";
const string prefix = "mover_client";
const string nodeName = prefix + "_node";
const string armShoulderPanPrefix = "arm_shoulder_pan";
const string armElbowFlexPrefix = "arm_elbow_flex";
const string armWristFlexPrefix = "arm_wrist_flex";
const string armShoulderLiftPrefix = "arm_shoulder_lift";
const string gripperPrefix = "gripper";
const string numGridsSolVectorTopicName = "num_grids_sol/vector";
const string paramFileName = "param.csv";

const string armElbowFlexJointService = armElbowFlexPrefix + serviceSuffix;
const string armShoulderLiftJointService = armShoulderLiftPrefix + serviceSuffix;
const string armShoulderPanJointService = armShoulderPanPrefix + serviceSuffix;
const string armWristFlexJointService = armWristFlexPrefix + serviceSuffix;
const string gripperJointService = gripperPrefix + serviceSuffix;

const vector<string> serviceNames = {
    armElbowFlexJointService, armShoulderLiftJointService, armShoulderPanJointService, armWristFlexJointService, gripperJointService
};

const vector<string> prefixes = {
    armShoulderPanPrefix, armElbowFlexPrefix, armWristFlexPrefix, armShoulderLiftPrefix, gripperPrefix
};

const vector<string> columnNames = {
    "row", "col", "num", armShoulderPanPrefix, armElbowFlexPrefix, armWristFlexPrefix, armShoulderLiftPrefix, gripperPrefix
};

stack<grid_num_vector> inMsgs; // Used in callback, must be global.

void fail(const string& msg)
{
    ROS_ERROR_STREAM(prefix << ": " << msg);
    ROS_INFO_STREAM(prefix << ": " << "Shutting down in " << exitDelay << "s.");
    ros::Duration(exitDelay).sleep();
    ros::shutdown();
    exit(EXIT_FAILURE);
}

string getHomePath()
{
    const char *homePath;

    // Try to get home from environ variable, then from system.
    if ((homePath = getenv(homeEnvVar.c_str())) == NULL)
    {
        homePath = getpwuid(getuid())->pw_dir;
    }
    if (homePath == NULL)
    {
        fail("Error finding home path");
    }

    return ((string)homePath) + "/";
}

void mapJointValues(CellToJointValue& map, const string& paramPath)
{
    ROS_INFO_STREAM(prefix << ": " << "Waiting for mapping params.csv ...");
    ifstream file(paramPath);
    if (!file.is_open())
    {
        fail("Error with opening file " + paramPath);
    }

    vector<string> lines;
    string line = "";
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    for (int i = 0; i < lines.size(); ++i)
    {
        if (i == 0)
        {
            continue;
        }

        vector<string> tokens;
        stringstream ss(lines[i]);
        string intermediate;
        while(getline(ss, intermediate, ','))
        {
            tokens.push_back(intermediate);
        }

        string key = "";
        JointNameToValue subMap;
        for (int i = 0; i < columnNames.size(); ++i)
        {
            if (i < gridNumVectorSize)
            {
                key += tokens[i];
                if (i != gridNumVectorSize - 1)
                {
                    key += ",";
                }
                continue;
            }
            subMap[columnNames[i]] = stof(tokens[i]);

            if (i + 1 == columnNames.size())
            {
                map[key] = subMap;
            }
        }
    }
}

bool getNextCellKey(string& key)
{
    if (inMsgs.empty())
    {
        return false;
    }

    grid_num_vector msg = inMsgs.top();
    inMsgs.pop();
    grid_num row = msg.numbered_grids[0];
    grid_num col = msg.numbered_grids[1];
    grid_num num = msg.numbered_grids[2];
    key = to_string(row.row) + "," + to_string(col.col) + "," + to_string(num.num);

    return true;
}

void sendJointRequest(jointNameToClient& clientMap, const string& serviceName, float move)
{

    if (!clientMap[serviceName])
    {
        fail("Mapping for " + serviceName + " doesn't point any where.");
    }

    move_and_confirm::Response resp;
    move_and_confirm::Request req;
    while (!ros::service::exists(serviceName, true))
    {
        ros::Duration(waitDuration).sleep();
    }

    bool success = false;
    req.move = move;

    do
    {
        ROS_DEBUG_STREAM(prefix << ": " << serviceName << " requesting: " << req.move);
        success = clientMap[serviceName]->call(req, resp);
        if(success)
        {
            ROS_DEBUG_STREAM(prefix << ": " << serviceName << " responded: " << resp.confirm);
        }
        else
        {
            fail("Failed to request service from client: " + serviceName);
        }
    } while (req.move != resp.confirm);
}

void sendAllJointRequests(jointNameToClient& clientMap, CellToJointValue& jointValueMap)
{
    string key;
    if (!getNextCellKey(key))
    {
        return;
    }

    for (const string& prefix : prefixes)
    {
        sendJointRequest(clientMap, prefix + serviceSuffix, jointValueMap[key][prefix]);
    }
}

void numGridSolCallBack(const grid_num_vector& msg)
{
    ROS_DEBUG_STREAM(prefix << ": " << "Received via " << numGridsSolVectorTopicName << ": " << msg);
    inMsgs.push(msg);
}

int main(int argc, char **argv)
{
    string homePath = getHomePath();
    string pkgSrcPath = homePath + workSpaceName + "/src/" + prefix + "/src/";

    // Node initialization
    init(argc, argv, nodeName);
    NodeHandle nh;

    if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged();
    }

    ros::Rate rate(hertz);

    CellToJointValue jointValueMap;
    mapJointValues(jointValueMap, pkgSrcPath + paramFileName);

    ServiceClient arm_eblow_flex_client = nh.serviceClient<move_and_confirm>(armElbowFlexJointService);
    ServiceClient arm_shoulder_lift_client = nh.serviceClient<move_and_confirm>(armShoulderLiftJointService);
    ServiceClient arm_shoulder_pan_client = nh.serviceClient<move_and_confirm>(armShoulderPanJointService);
    ServiceClient arm_wrist_flex_client = nh.serviceClient<move_and_confirm>(armWristFlexJointService);
    ServiceClient gripper_client = nh.serviceClient<move_and_confirm>(gripperJointService);

    // Put into a container so it cans be looped through.
    jointNameToClient clientMap;
    clientMap[armElbowFlexJointService] = &arm_eblow_flex_client;
    clientMap[armShoulderLiftJointService] = &arm_shoulder_lift_client;
    clientMap[armShoulderPanJointService] = &arm_shoulder_pan_client;
    clientMap[armWristFlexJointService] = &arm_wrist_flex_client;
    clientMap[gripperJointService] = &gripper_client;

    Subscriber sub = nh.subscribe(numGridsSolVectorTopicName, gridSize * gridSize, &numGridSolCallBack);
    ROS_INFO_STREAM(prefix << ": " << nodeName << " ready");
    while (ros::ok)
    {
        sendAllJointRequests(clientMap, jointValueMap);
        ros::spinOnce();
        rate.sleep();
    }

    ros::shutdown();
    return 0;
}
