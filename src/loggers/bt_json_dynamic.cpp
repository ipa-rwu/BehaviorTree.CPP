#include "behaviortree_cpp_v3/loggers/bt_json_dynamic.h"

namespace BT
{
std::atomic<bool> JsonDynamicLogger::ref_count(false);

JsonDynamicLogger::JsonDynamicLogger(const BT::Tree& tree, const char* filename_json) 
: StatusChangeLogger(tree.root_node)
{
    bool expected = false;
    file_os_.open(filename_json);

    if (!ref_count.compare_exchange_strong(expected, true))
    {
        throw LogicError("Only one instance of StdCoutLogger shall be created");
    }
}

JsonDynamicLogger::~JsonDynamicLogger()
{
    ref_count.store(false);
}

void JsonDynamicLogger::callback(Duration timestamp, const TreeNode& node, NodeStatus prev_status,
                             NodeStatus status)
{
    using namespace std::chrono;
    using namespace std;

    constexpr const size_t ws_count = 25;

    Json::Value root;
    Json::StyledWriter styledWriter;

    double since_epoch = duration<double>(timestamp).count();

    root["timestamp"] = since_epoch;
    root["name"] = node.name().c_str();
    root["prestatus"] = toStr(prev_status, true).c_str();
    root["currstatus"] = toStr(status, true).c_str();


    file_os_ << styledWriter.write(root);
    file_os_ << endl << endl;

    file_os_.flush();
}

void JsonDynamicLogger::flush()
{
    std::cout << std::flush;
	ref_count = false;
}

}   // end namespace
