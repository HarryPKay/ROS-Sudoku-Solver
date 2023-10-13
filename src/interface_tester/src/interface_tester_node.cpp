#include <ros/ros.h>
#include <ros/console.h>

#include <string>
#include <iostream>

#include <mover_client/grid_num.h>
#include <mover_client/grid_num_vector.h>

using namespace std;
using namespace ros;
using namespace mover_client;

const string prefix = "interface_tester";
const string nodeSuffix = "_node";
const string numGridsVectorTopicName = "num_grids/vector"; // topic name (subscribes to)
const int hertz = 10;
const int gridSize = 100;
const int exitDelay = 600;

struct Cell
{
    Cell(int x, int y, int z): x(x), y(y), z(z) {}
    int x, y, z;
};

grid_num_vector generateRandomMsg()
{
    grid_num row, col, num;
    grid_num_vector msg;
    row.row = 1 + (rand()%9);
    col.col = 1 + (rand()%9);
    num.num = 1 + (rand()%9);
    msg.numbered_grids.push_back(row);
    msg.numbered_grids.push_back(col);
    msg.numbered_grids.push_back(num);
    return msg;
}

void vectorMsgSend(Publisher& pub,  const grid_num_vector& msg)
{
    ROS_DEBUG_STREAM(prefix << ": " << "Publishing to " << numGridsVectorTopicName << ": " << msg);
    pub.publish(msg);
}

int main(int argc, char **argv)
{
    init(argc, argv, prefix + nodeSuffix);
    NodeHandle nh;
    ros::Rate rate(hertz);

    if(ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged();
    }

    Publisher pub = nh.advertise<grid_num_vector>(numGridsVectorTopicName, gridSize * gridSize);

    //srand(time(0));

    //vector<Cell> instance = {Cell(1,1,2)};
    //vector<Cell> instance = {Cell(1,1,2), Cell(2,2,3)};

    // Tests cast for automatically changing ASP to use 4 by 4 grid
    vector<Cell> instance = {Cell(1,1,2), Cell(2,2,3), Cell(1,3,4), Cell(2,4,1), Cell(3,2,2), Cell(4,1,1), Cell(4,3,3), Cell(3,4,4)};

    // Tests cast for automatically changing ASP to use 9 by 9 grid
    //vector<Cell> instance = {Cell(1,1,2), Cell(2,2,3), Cell(1,3,4), Cell(2,4,1), Cell(3,2,2), Cell(4,1,1), Cell(4,3,3), Cell(3,4,9)};
    while (ok)
    {
        if (pub.getNumSubscribers() > 0)
        {
            Cell cell = instance.back();
            instance.pop_back();
            grid_num row, col, num;
            grid_num_vector msg;
            row.row = cell.x;
            col.col = cell.y;
            num.num = cell.z;
            msg.numbered_grids.push_back(row);
            msg.numbered_grids.push_back(col);
            msg.numbered_grids.push_back(num);
            vectorMsgSend(pub, msg);
        }

        spinOnce();
        rate.sleep();

        if (instance.size() == 0)
        {
            break;
        }
    }

    ROS_INFO_STREAM(prefix << ": " << "Data sent, shutting down in " << exitDelay << "s.");
    ros::Duration(exitDelay).sleep();
    ros::shutdown();
    return 0;
}

