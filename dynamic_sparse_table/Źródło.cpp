#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct RootNode {
	int stored_key;
	bool is_key_present;
	struct BranchNode** children;
	int root_branch_factor; //how many children are connected to the root
	int k_branch_factor; //the same for every branch
};
typedef struct BranchNode {
	int stored_key;
	bool is_key_present;
	BranchNode** children;
};

void initialize_root_with_children(RootNode* root_node_ptr, int n_rb_val, int k_val) {
    if (!root_node_ptr) return;

    root_node_ptr->root_branch_factor = n_rb_val;
    root_node_ptr->k_branch_factor = k_val;
    root_node_ptr->is_key_present = false;
    root_node_ptr->stored_key = -1; //mark empty
    root_node_ptr->children = nullptr;

    //only allocate if n_rb_val > 0
    if (n_rb_val > 0) {
        //allocate memory for the branchnode pointers
        root_node_ptr->children = (BranchNode**)calloc(root_node_ptr->root_branch_factor, sizeof(BranchNode*)); //instead of malloc because calloc puts 0 in everythin
    }
}



BranchNode* create_branch_node(RootNode* root_node_ptr) {

    if (!root_node_ptr) {
        return nullptr;
    }
    BranchNode* b_node = (BranchNode*)malloc(sizeof(BranchNode));
    if (!b_node) {
        return nullptr;
    }

    b_node->is_key_present = false;
    b_node->stored_key = -1; //mark empty
    b_node->children = nullptr;

    //only allocate children if k > 0
    if (root_node_ptr->k_branch_factor > 0) {
        //allocate children for branch node with k_branch_factor from the root
        b_node->children = (BranchNode**)calloc(root_node_ptr->k_branch_factor, sizeof(BranchNode*));
        if (!b_node->children) {
            free(b_node);
            return nullptr;
        }
    }



    return b_node;
}

bool lookup_key(RootNode* root, int key_to_find) {
    if (!root) return false;
    if (root->is_key_present && root->stored_key == key_to_find) return true;

    if (root->root_branch_factor <= 0) return false;
    if (root->children == nullptr && root->root_branch_factor > 0) return false;

    int path_val = key_to_find;

    // Level 1: Root's children
    int child_idx = path_val % root->root_branch_factor;
    path_val /= root->root_branch_factor;

    if (child_idx < 0 || child_idx >= root->root_branch_factor) return false;
    if (root->children == nullptr) return false;

    BranchNode* current_node = root->children[child_idx];

    // Subsequent levels
    while (current_node != nullptr) {
        // Check 1: Is the current node the one we are looking for?
        if (current_node->is_key_present && current_node->stored_key == key_to_find) {
            return true;
        }
        if (root->k_branch_factor <= 0) {
            return false; // Cannot go deeper from any node at this level
        }
        if (current_node->children == nullptr && root->k_branch_factor > 0) {
            return false; // Current node should have children but doesn't
        }

        // Determine next child based on current path_val
        child_idx = path_val % root->k_branch_factor; // If path_val is 0, child_idx becomes 0
        path_val /= root->k_branch_factor;       // If path_val was 0, it remains 0

        if (child_idx < 0 || child_idx >= root->k_branch_factor) return false;

        current_node = current_node->children[child_idx]; // Move to next node
    }

    return false; // current_node became nullptr, path ended
}

void enter_key(RootNode* root, int key_to_insert) {
    //checks
    if (!root) return;

    //first key goes into the root
    bool is_root_empty = !root->is_key_present;

    if (is_root_empty && root->children != nullptr && root->root_branch_factor > 0) {
        for (int i = 0; i < root->root_branch_factor; ++i) {
            if (root->children[i] != nullptr) {
                is_root_empty = false; //found a child, so not empty
                break;
            }
        }
    }

    if (is_root_empty) {
        root->stored_key = key_to_insert;
        root->is_key_present = true;
        return; //inserted into the root, move on
    }

    //check if key we want to put in already exists
    if (lookup_key(root, key_to_insert)) {
        cout << key_to_insert << " exist" << endl;
        return; //key exists
    }


    //if root is full and the key isnt present elsewhere, move on to putting it in

    int value = key_to_insert;

    //roots child
    int child_idx_at_root = value % root->root_branch_factor;
    value /= root->root_branch_factor;

    if (child_idx_at_root < 0 || child_idx_at_root >= root->root_branch_factor) return; //invalid path

    BranchNode* current_node;

    //roots children
    if (root->children[child_idx_at_root] == nullptr) {
        root->children[child_idx_at_root] = create_branch_node(root);
        current_node = root->children[child_idx_at_root];
        current_node->stored_key = key_to_insert; //store key in new created node
        current_node->is_key_present = true;
        return; //key inserted
    } else {
        current_node = root->children[child_idx_at_root];
    }

    //if current_node (first level child of the root) exists but is empty and path ends here
    if (value == 0 && !current_node->is_key_present) { //path calculation terminates at this current_node
        if (!current_node->is_key_present) {
            current_node->stored_key = key_to_insert;
            current_node->is_key_present = true;
            return; //key inserted
        }
    }

    //branchnode to branchnode
    while (true) {
        if (root->k_branch_factor <= 0) return; //cannot go deeper
        if (current_node->children == nullptr && root->k_branch_factor > 0) return; //no children - cant go deeper

        int child_idx_at_branch = value % root->k_branch_factor;
        value /= root->k_branch_factor;

        //incorrect path
        if (child_idx_at_branch < 0 || child_idx_at_branch >= root->k_branch_factor) return;

        if (current_node->children[child_idx_at_branch] == nullptr) {
            current_node->children[child_idx_at_branch] = create_branch_node(root);
            if (current_node->children[child_idx_at_branch] == nullptr) {
                return;
            }
            BranchNode* new_child_node = current_node->children[child_idx_at_branch];
            new_child_node->stored_key = key_to_insert; //new node stores the key
            new_child_node->is_key_present = true;
            return; //key inserted
        }
        else {
            current_node = current_node->children[child_idx_at_branch];

            if (value == 0) {
                if (!current_node->is_key_present) {
                    current_node->stored_key = key_to_insert;
                    current_node->is_key_present = true;
                    return; //key inserted
                }
            }
        }


    }
}


void print_branch_recursive(BranchNode* node, int k_factor, bool& first_key_printed) {
    if (node == nullptr) {
        return;
    }

    //print the current nodess key if present
    if (node->is_key_present) {
        if (!first_key_printed) {
            cout << node->stored_key;
            first_key_printed = true;
        } else {
            cout << " " << node->stored_key;
        }
    }

    //recursive print all children if they exist
    if (k_factor > 0 && node->children != nullptr) {
        for (int i = 0; i < k_factor; ++i) { //iterate from child 0 to k_factor-1
            print_branch_recursive(node->children[i], k_factor, first_key_printed);
        }
    }
}


bool is_node_leaf(BranchNode* node, int k_factor) {
    if (node == nullptr) return true; //null node is a leaf
    if (node->children == nullptr) return true; //no children array allocated

    for (int i = 0; i < k_factor; ++i) {
        if (node->children[i] != nullptr) {
            return false; //found a child
        }
    }
    return true; //all child pointers are nullptr
}

BranchNode* find_leftmost_leaf_descendant(
    BranchNode* current_search_node, //node to start search from
    int k_factor,
    BranchNode** out_parent_of_actual_leaf, //parent of found leaf q
    int* out_child_idx_of_actual_leaf)      //index of q in its parent
{
    BranchNode* iter_node = current_search_node;
    BranchNode* iter_parent = nullptr; //parent of iter_node during descent
    int iter_child_idx = -1;           //index of iter_node in iter_parent

    if (is_node_leaf(iter_node, k_factor)) {
        *out_parent_of_actual_leaf = nullptr;
        *out_child_idx_of_actual_leaf = -1;
        return iter_node; //current_search_node itself is the leaf
    }

    iter_parent = current_search_node; //first potential parent

    while (true) {
        bool descended = false;
        if (iter_node->children == nullptr || k_factor <= 0) {
            break;
        }
        for (int i = 0; i < k_factor; ++i) { //find the leftmost not null child
            if (iter_node->children[i] != nullptr) {
                iter_parent = iter_node;
                iter_child_idx = i;
                iter_node = iter_node->children[i];
                descended = true;
                break;
            }
        }
        if (!descended || is_node_leaf(iter_node, k_factor)) {
            break; //stop if no descent or if new iter_node is a leaf
        }
    }

    *out_parent_of_actual_leaf = iter_parent;
    *out_child_idx_of_actual_leaf = iter_child_idx;
    return iter_node; //the leftmost leaf descendant
}


void delete_recursive(
    BranchNode* current_node_in_path,       //current node on path to p
    BranchNode* parent_of_current_in_path,  //parent of current_node_in_path
    int idx_of_current_in_parent,           //index of current_node_in_path
    int value_for_path,                     //remaining value to find p
    int original_key_to_delete,
    RootNode* root_config)
{
    if (current_node_in_path == nullptr) {
        return; //p not found down this path
    }

    //check if current_node_in_path is node p
    if (current_node_in_path->is_key_present && current_node_in_path->stored_key == original_key_to_delete) {
        BranchNode* p_node = current_node_in_path; //found p

        if (is_node_leaf(p_node, root_config->k_branch_factor)) {
            //case 1: p is a leaf
            p_node->is_key_present = false;
            p_node->stored_key = -1;

            if (parent_of_current_in_path != nullptr) { //p had BranchNode parent
                parent_of_current_in_path->children[idx_of_current_in_parent] = nullptr;
            } else { //p was direct child of RootNode
                if (idx_of_current_in_parent >=0 && idx_of_current_in_parent < root_config->root_branch_factor) {
                     root_config->children[idx_of_current_in_parent] = nullptr;
                }
            }
            if (p_node->children) {
                free(p_node->children);
            }
            free(p_node); //delete node p
        } else {
            //case 2: p is internal, find replacement q
            BranchNode* q_node_parent = nullptr;
            int q_node_idx = -1;
            BranchNode* q_node = find_leftmost_leaf_descendant(p_node, root_config->k_branch_factor, &q_node_parent, &q_node_idx);

            if (q_node != nullptr && q_node->is_key_present) {
                //suitable replacement q found
                p_node->stored_key = q_node->stored_key; //copy q's key to p

                q_node->is_key_present = false; //mark q as empty
                q_node->stored_key = -1;

                if (q_node_parent != nullptr && q_node_idx != -1) {
                    q_node_parent->children[q_node_idx] = nullptr; //detach q
                } else {
                    //q might be direct child of p, try to find and detach
                    bool detached_q = false;
                    if (p_node->children) {
                        for(int i=0; i<root_config->k_branch_factor; ++i) {
                            if(p_node->children[i] == q_node) {
                                p_node->children[i] = nullptr;
                                detached_q = true;
                                break;
                            }
                        }
                    }
                }
                if (q_node->children) free(q_node->children);
                free(q_node); //delete node q
            } else {
                //no suitable q found, just mark p as empty
                p_node->is_key_present = false;
                p_node->stored_key = -1;
            }
        }
        return; //key processed
    }

    //navigate deeper to find p
    if (value_for_path == 0 && !(current_node_in_path->is_key_present && current_node_in_path->stored_key == original_key_to_delete)) {
        //path for key ends here, but this node isn't it.
        //enter_key would have pushed to child 0 if this node was occupied.
        //so, for deletion, path continues via child 0.
    }


    if (root_config->k_branch_factor <= 0) return;
    if (current_node_in_path->children == nullptr && root_config->k_branch_factor > 0) return;

    int next_child_idx = value_for_path % root_config->k_branch_factor; //if value_for_path is 0, next_child_idx is 0
    value_for_path /= root_config->k_branch_factor;

    if (next_child_idx < 0 || next_child_idx >= root_config->k_branch_factor) return;
    if (current_node_in_path->children == nullptr) return;

    delete_recursive(
        current_node_in_path->children[next_child_idx],
        current_node_in_path, //current is parent for next call
        next_child_idx,       //index of child
        value_for_path,       //new remaining value
        original_key_to_delete,
        root_config);
}



int main(){
    int num_commands;
    cin >> num_commands;

    int min_val_range, max_val_range;
    cin >> min_val_range >> max_val_range;

    //root branch factor and k branch factor
    int rbf_input, kbf_input;
    cin >> rbf_input >> kbf_input;

    RootNode root;
    initialize_root_with_children(&root, rbf_input, kbf_input);


    char command_char;
    int key_value;
    for (int i = 0; i < num_commands; ++i) {
        cin >> command_char;
        switch (command_char){

        case 'I': {
            cin >> key_value;
            enter_key(&root, key_value); //printing built into the fucntion
            break;
        }
        case 'L': {
            cin >> key_value;
            if (lookup_key(&root, key_value)) {
                cout << key_value << " exist" << endl;
            }
            else {
                cout << key_value << " not exist" << endl;
            }
            break;
        }
        // --- START OF REVISED 'D' case in main ---
        case 'D': {
            cin >> key_value;
            if (!lookup_key(&root, key_value)) {
                cout << key_value << " not exist" << endl;
                break;
            }

            //case 1: key is in RootNode
            if (root.is_key_present && root.stored_key == key_value) {
                bool root_is_structural_leaf = true;
                if (root.root_branch_factor > 0 && root.children != nullptr) {
                    for (int j = 0; j < root.root_branch_factor; ++j) {
                        if (root.children[j] != nullptr) {
                            root_is_structural_leaf = false;
                            break;
                        }
                    }
                }

                if (root_is_structural_leaf) {
                    root.is_key_present = false; //root is leaf, just empty it
                    root.stored_key = -1;
                } else {
                    //root is internal, find replacement q from descendants
                    BranchNode* q_node_parent_for_root = nullptr;
                    int q_node_idx_for_root = -1;
                    BranchNode* q_node_for_root = nullptr;

                    for (int i = 0; i < root.root_branch_factor; ++i) { //search children subtrees
                        if (root.children != nullptr && root.children[i] != nullptr) {
                            q_node_for_root = find_leftmost_leaf_descendant(
                                root.children[i],
                                root.k_branch_factor,
                                &q_node_parent_for_root, //parent of q
                                &q_node_idx_for_root     //index of q
                            );
                            if (q_node_for_root != nullptr && q_node_for_root->is_key_present) {
                                break; //found suitable q
                            } else {
                                q_node_for_root = nullptr; //reset if not suitable
                            }
                        }
                    }

                    if (q_node_for_root != nullptr && q_node_for_root->is_key_present) {
                        root.stored_key = q_node_for_root->stored_key; //copy q's key to root

                        q_node_for_root->is_key_present = false; //mark q empty
                        q_node_for_root->stored_key = -1;

                        if (q_node_parent_for_root != nullptr && q_node_idx_for_root != -1) {
                            q_node_parent_for_root->children[q_node_idx_for_root] = nullptr; //detach q
                        } else {
                             //q might be direct child of root, search and detach
                            bool q_detached = false;
                            for(int i=0; i<root.root_branch_factor; ++i) {
                                if(root.children && root.children[i] == q_node_for_root) {
                                    root.children[i] = nullptr;
                                    q_detached = true;
                                    break;
                                }
                            }
                        }
                        if (q_node_for_root->children) free(q_node_for_root->children);
                        free(q_node_for_root); //delete node q
                    } else {
                        //no suitable q found, just empty root
                        root.is_key_present = false;
                        root.stored_key = -1;
                    }
                }
            }
            //case 2: key is in BranchNode subtree
            else {
                if (root.root_branch_factor > 0 && root.children != nullptr) {
                    int value = key_value; //value for path calculation
                    int child_idx_at_root_for_del = value % root.root_branch_factor;
                    value /= root.root_branch_factor; //remaining value

                    if (child_idx_at_root_for_del >= 0 && child_idx_at_root_for_del < root.root_branch_factor) {
                        delete_recursive(
                            root.children[child_idx_at_root_for_del],
                            nullptr, //parent is root
                            child_idx_at_root_for_del,
                            value,
                            key_value,
                            &root);
                    }
                }
            }
            break;
        }
        case 'P': {
            bool first_key_has_been_printed = false;

            //print roots key if present
            if (root.is_key_present) {
                cout << root.stored_key;
                first_key_has_been_printed = true;
            }

            //print children subtrees
            if (root.children != nullptr && root.root_branch_factor > 0) {
                for (int j = 0; j < root.root_branch_factor; ++j) {
                    //the k_factor for children of branchnodes is root.k_branch_factor
                    print_branch_recursive(root.children[j], root.k_branch_factor, first_key_has_been_printed);
                }
            }
        }
        }
    }

}