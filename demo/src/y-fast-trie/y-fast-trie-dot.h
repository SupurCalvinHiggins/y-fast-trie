#include "../../../src/y-fast-trie/y-fast-trie.h"
#include "../red-black-tree/red-black-tree-dot.h"
#include <string>
#include <sstream>

template <typename Key_>
class YFastTrieDot : public YFastTrie<Key_, RedBlackTreeDot<Key_>> {
private:
    std::string ptr_to_str(void* ptr) const noexcept {
        std::ostringstream ss;
        ss << ptr;
        return "n" + ss.str();
    }

    std::string key_to_str(key_type key) const noexcept {
        std::ostringstream ss;
        ss << key;
        return ss.str();
    }

public:
    std::string to_dot() const noexcept {
        std::string output;
        output += "digraph {\n"

        for (const auto& level : this->lss_) {
            std::string level_output;
            level_output += "{rank = same; ";

            for (const auto& key_node_pair : level) {
                auto key = key_node_pair.first;
                auto node = key_node_pair.second;
                if (node->get_left()) 
                    output += "\t" + ptr_to_str(node) + " -> " + ptr_to_str(node->get_left()) + "\n";
                if (node->get_right()) 
                    output += "\t" + ptr_to_str(node) + " -> " + ptr_to_str(node->get_right()) + "\n";    
                output += "\t" + ptr_to_str(node) + "[label=\"" + key_to_str(key) + "\"]\n";
                level_output += ptr_to_str(node) + "; ";
            }

            level_output += "}";
            output += "\t" + level_output + "\n"; 
        }
        output += "}";
        return output;
    }
};