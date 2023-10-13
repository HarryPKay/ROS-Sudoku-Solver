#include <ros/ros.h>
#include <ros/console.h>

#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>
#include <regex>

#include <mover_client/grid_num.h>
#include <mover_client/grid_num_vector.h>

using namespace std;
using namespace ros;
using namespace mover_client;

const int gridSize = 100;
const int hertz = 10;
const int sleepDuration = 2; // time to wait if anything is not ready.
const int maxWaitOnMsg = 2; // Maximum time to wait for new messages to come in.
const int answerIndex  = 4; // the line that solution is stored from clingo
const int exitDelay = 600;

const string wsName = "sudoku_bot_ws";
const string homeEnvVar = "HOME";
const string prefix = "interface";
const string nodeSuffix = "_node";
const string interfaceNodeName = "interface_node";
const string numGridsSo1VectorTopicName = "num_grids_sol/vector"; // topic name (publishes to)
const string numGridsVectorTopicName = "num_grids/vector"; // topic name (subscribes to)
const string aspProgramFileName = "my_sudoku_ASP_program.lp";
const string aspFactsFileName = "my_sudoku_facts.lp";
const string clingoFileName = "clingo";
const string solvedFileName = "result.txt";
const string solutionPrefix = "gridNumSol";

// Used to determine which lines ro replace.
const string gridDimensionTag = "%<GRID_DIMENSION>";
const string sqrtGridDimensionTag = "%<SQRT_GRID_DIMENSION>";
// Template for replacing some lines of ASP program.
const string aspGridDimension = "#const gridDimension = <NUMBER>.%<GRID_DIMENSION>";
const string aspSqrtGridDimension = "#const sqrtGridDimension = <NUMBER>.%<SQRT_GRID_DIMENSION>";

// Below must be global, used in callbacks
stack<grid_num_vector> inMsgs;
stack<grid_num_vector> outMsgs;
int maxNumberRowCol = 0;
bool solving = false;

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

// lazily read the file and search for the string to replace, then over write file.
void replaceLineInFileContainingStr(const string& filePath, const string& containingStr, const string& replacementLine)
{
    vector<string> lines;
    string line;
    ifstream inFile(filePath);
    if (inFile.is_open())
    {
        while (getline(inFile, line))
        {
            lines.push_back(line);
        }
        inFile.close();
    }
    else
    {
        fail("Problem encountered opening: " + filePath);
    }
    for (string& s : lines)
    {
        if (s.find(containingStr) != std::string::npos)
        {
            s = replacementLine;
        }
    }
    ofstream outFile(filePath);
    if (outFile.is_open())
    {
        for (const string& s : lines)
        {
            outFile << s << "\n";
        }
        outFile.close();
    }
    else
    {
        fail("Problem encountered opening: " + filePath);
    }
}

// Replace the macros in ASP with the appropriate size based on what data was received
void replaceASPConstants(const string& filePath)
{
    int size = maxNumberRowCol;
    int sqrtSize = sqrt(size);
    string s1 = aspGridDimension;
    string s2 = aspSqrtGridDimension;

    s1 = std::regex_replace(s1, std::regex("\\<NUMBER>"), to_string(size));
    s2 = std::regex_replace(s2, std::regex("\\<NUMBER>"), to_string(sqrtSize));
    replaceLineInFileContainingStr(filePath, gridDimensionTag, s1);
    replaceLineInFileContainingStr(filePath, sqrtGridDimensionTag, s2);
    maxNumberRowCol = 0;
}

string toFact(const grid_num_vector& msg)
{
    grid_num row, col, num;
    row = msg.numbered_grids[0];
    col = msg.numbered_grids[1];
    num = msg.numbered_grids[2];
    string result = solutionPrefix + "(" + to_string(row.row) + "," + to_string(col.col) + "," + to_string(num.num) + ")";
    return result;
}

void factsToFile(const string& srcPath)
{
    ofstream factsOut(srcPath + aspFactsFileName);
    if (factsOut.is_open())
    {
        while (!inMsgs.empty())
        {
            factsOut << toFact(inMsgs.top()) << ".\n";
            inMsgs.pop();
        }
        factsOut.close();
    }
    else
    {
        fail("Couldn't write to " + srcPath + aspFactsFileName);
    }
}

void vectorMsgSend(Publisher& pub)
{
    if (outMsgs.empty())
    {
        return;
    }

    while (pub.getNumSubscribers() == 0)
    {
        ROS_DEBUG_STREAM(prefix << ": " << "Waiting for subscribers to" << numGridsSo1VectorTopicName << "...");
        ros::Duration(sleepDuration).sleep();
    }

    ROS_DEBUG_STREAM(prefix << ": " << "Sending via topic " << numGridsSo1VectorTopicName << ": " << outMsgs.top());
    pub.publish(outMsgs.top());
    outMsgs.pop();
}

// Problem received
void vectorMsgReceivedCallBack(const grid_num_vector& msg)
{

    ROS_DEBUG_STREAM(prefix << ": " << "Received via topic " << numGridsVectorTopicName << ": " << msg);
    inMsgs.push(msg);
    grid_num row = msg.numbered_grids[0];
    grid_num col = msg.numbered_grids[1];
    grid_num num = msg.numbered_grids[2];
    if (row.row > maxNumberRowCol)
    {
        maxNumberRowCol = row.row;
    }
    if (col.col > maxNumberRowCol)
    {
        maxNumberRowCol = col.col;
    }
    if (num.num > maxNumberRowCol)
    {
        maxNumberRowCol = num.num;
    }
}

void notReceivngCheckCallBack(const ros::TimerEvent&)
{
    static int lastMsgsSize = 0;
    static int newMsgsSize = inMsgs.size();
    if (newMsgsSize > lastMsgsSize)
    {
        lastMsgsSize = newMsgsSize;
        solving = false;
        return;
    }
    if (inMsgs.empty())
    {
        return;
    }
    if (solving)
    {
        return;
    }
    ROS_INFO_STREAM(prefix << ": " << "> " << maxWaitOnMsg <<  "s elapsed, now solve.");
    solving = true;
    lastMsgsSize = 0;
    newMsgsSize = 0;
}

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

bool cleanFromStrMsg(char c)
{
    return !isdigit(c) && c != ',';
}

grid_num_vector stringToMsg(string& strMsg)
{
    strMsg.erase(std::remove_if(strMsg.begin(), strMsg.end(), cleanFromStrMsg), strMsg.end());
    vector<string> tokens;
    stringstream ss(strMsg);
    string intermediate;

    while(getline(ss, intermediate, ','))
    {
        tokens.push_back(intermediate);
    }

    grid_num row, col, num;
    grid_num_vector msg;
    if (tokens.size() < 3)
    {
        fail("stringToMsg(): incorrect size given.");
    }

    row.row = stoi(tokens[0]);
    col.col = stoi(tokens[1]);
    num.num = stoi(tokens[2]);
    msg.numbered_grids.push_back(row);
    msg.numbered_grids.push_back(col);
    msg.numbered_grids.push_back(num);
    return msg;
}

void solve(const string& srcPath, const string& sysCommand)
{
    if (!solving)
    {
        return;
    }
    // write facts to file and then set the size for ASP
    factsToFile(srcPath);
    replaceASPConstants(srcPath + aspProgramFileName);

    // Run clingo
    system(sysCommand.c_str());

    vector<string> lines;
    string line = "";
    ifstream file(srcPath + solvedFileName);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            lines.push_back(line);
        }
    }
    else
    {
        fail("Error with opening " + srcPath + solvedFileName);
    }

    // Convert string solution to msg type.
    if (lines[answerIndex + 1].find("UNSATISFIABLE") != std::string::npos)
    {
        ROS_INFO_STREAM(prefix << ": " << "UNSATISFIABLE");
    }
    else
    {
        ROS_INFO_STREAM(prefix << ": " << "SATISFIABLE");
        vector<string> tokens;
        stringstream ss(lines[answerIndex]);
        string intermediate;

        while(getline(ss, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }
        for (string& token : tokens)
        {
            outMsgs.push(stringToMsg(token));
        }
    }

    solving = false;
}

int main(int argc, char **argv)
{
    init(argc, argv, prefix + nodeSuffix);
    NodeHandle nh;
    ros::Rate rate(hertz);

    if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged();
    }

    const string homePath = getHomePath();
    const string srcPath = homePath + "/" + wsName + "/src/" + prefix + "/src/";
    const string sysCommand = "cd " + srcPath + " && ./" + clingoFileName + " ./" + aspProgramFileName + " ./" + aspFactsFileName + " > " + solvedFileName;

    ROS_INFO_STREAM(prefix << ": Ready.");

    Publisher pub = nh.advertise<grid_num_vector>(numGridsSo1VectorTopicName, gridSize * gridSize);

    Subscriber sub = nh.subscribe(numGridsVectorTopicName, gridSize * gridSize, &vectorMsgReceivedCallBack);

    ros::Timer timer = nh.createTimer(ros::Duration(maxWaitOnMsg), &notReceivngCheckCallBack);

    while (ros::ok)
    {
        solve(srcPath, sysCommand);
        vectorMsgSend(pub);
        ros::spinOnce();
        rate.sleep();
    }

    ros::shutdown;
    return EXIT_SUCCESS;
}
