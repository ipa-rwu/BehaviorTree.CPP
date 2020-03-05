#ifndef BT_JSON_DYNAMIC_H
#define BT_JSON_DYNAMIC_H

#include <jsoncpp/json/json.h>
#include <string>
#include <iostream>
#include <fstream>

#include <cstring>
#include "abstract_logger.h"

namespace BT
{
/**
 * @brief AddStdCoutLoggerToTree. Give  the root node of a tree,
 * a simple callback is subscribed to any status change of each node.
 *
 *
 * @param root_node
 * @return Important: the returned shared_ptr must not go out of scope,
 *         otherwise the logger is removed.
 */

class JsonDynamicLogger : public StatusChangeLogger
{
    static std::atomic<bool> ref_count;

  public:
    JsonDynamicLogger(const BT::Tree& tree, const char* filename_json);
    ~JsonDynamicLogger() override;

    const char* filename_json;
    
    virtual void callback(Duration timestamp, const TreeNode& node, NodeStatus prev_status,
                          NodeStatus status) override;

    virtual void flush() override;

  private:
    std::ofstream file_os_;

};

}   // end namespace

#endif   // BT_JSON_DYNAMIC_H
