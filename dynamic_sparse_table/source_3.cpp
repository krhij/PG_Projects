#include <cstdio>   //for scanf and printf
using namespace std;
const int EMPTY_KEY = -1; //this means the spot is empty

struct Node {
    int stored_key; //the number we save here
    Node** children; //points to more nodes, like branches
};

//makes a new node, but doesnt make its children array yet
Node* create_bare_node() {
    Node* node = new Node; //make space for a new node
    node->stored_key = EMPTY_KEY; //it's empty at first
    node->children = nullptr; //no children branches yet
    return node;
}

//looks for a key in the tree
//tree_root_ptr is the very top node
//rbf is how many children the root can have
//kbf is how many children other nodes can have
//returns true if we find it, false if not
bool lookup(Node* tree_root_ptr, int key_to_find, int rbf, int kbf) {
    if (!tree_root_ptr) return false; //no tree, no key
    if (tree_root_ptr->stored_key == key_to_find && tree_root_ptr->stored_key != EMPTY_KEY) return true; //found it at the root!

    //check the root's children
    if (rbf <= 0) return false; //root can't have children
    if (tree_root_ptr->children == nullptr) return false; //root has no children array

    int path_val = key_to_find; //this helps decide where to go
    int child_idx = path_val % rbf; //which child of the root to check
    path_val /= rbf; //update path_val for next time

    if (child_idx < 0 || child_idx >= rbf) return false; //bad index

    Node* current_node = tree_root_ptr->children[child_idx]; //go to that child

    //now look deeper
    while (current_node != nullptr) { //keep going if there's a node
        if (current_node->stored_key == key_to_find && current_node->stored_key != EMPTY_KEY) {
            return true; //found
        }
        //now check this current_node's children using kbf
        if (kbf <= 0) return false; //this node can't have children
        if (current_node->children == nullptr) return false; //this node has no children array

        child_idx = path_val % kbf; //which child of current_node
        path_val /= kbf; //update path_val

        if (child_idx < 0 || child_idx >= kbf) return false; //bad index
        current_node = current_node->children[child_idx]; //go to the next child
    }
    return false; //didnt find it
}

//puts a new key into the tree
//rbf and kbf are same as lookup
//it makes the children arrays when needed
void insert(Node* tree_root_ptr, int key_to_insert, int rbf, int kbf) {
    if (!tree_root_ptr) return; //no tree
    if (key_to_insert == EMPTY_KEY) return; //can't insert this special empty value

    if (lookup(tree_root_ptr, key_to_insert, rbf, kbf)) {
        printf("%d exist\n", key_to_insert); //already there
        return;
    }

    //if root is empty, put it there
    if (tree_root_ptr->stored_key == EMPTY_KEY) {
        tree_root_ptr->stored_key = key_to_insert;
        return;
    }

    //root is full, try to put in a child
    int value = key_to_insert; //for figuring out the path
    int child_idx_for_root;

    if (rbf <= 0) return; //root cant have children
    child_idx_for_root = value % rbf; //which child spot for root
    value /= rbf; //update value for later

    if (child_idx_for_root < 0 || child_idx_for_root >= rbf) return; //bad path

    //if root doesnt have its children array yet, make it
    if (tree_root_ptr->children == nullptr) {
        tree_root_ptr->children = new Node*[rbf]; //make space for rbf child pointers
        for (int k = 0; k < rbf; ++k) tree_root_ptr->children[k] = nullptr; //all spots empty at first
    }

    Node* current_node;
    //if the spot for this child is empty, make a new node there
    if (tree_root_ptr->children[child_idx_for_root] == nullptr) {
        tree_root_ptr->children[child_idx_for_root] = create_bare_node(); //make a new simple node
        current_node = tree_root_ptr->children[child_idx_for_root];
        current_node->stored_key = key_to_insert; //put the key in
        return;
    } else { //child already exists, go to it
        current_node = tree_root_ptr->children[child_idx_for_root];
    }

    //if this child node is empty, put the key here
    if (current_node->stored_key == EMPTY_KEY) {
        current_node->stored_key = key_to_insert;
        return;
    }

    //go deeper into the tree
    while (true) {
        if (kbf <= 0) return; //cant go deeper

        int child_idx_for_branch = value % kbf; //which spot in this nodes children
        value /= kbf; //update value

        if (child_idx_for_branch < 0 || child_idx_for_branch >= kbf) return; //bad path

        //if current_node doesnt have its children array, make it
        if (current_node->children == nullptr) {
            current_node->children = new Node*[kbf]; //make space for kbf child pointers
            for (int k = 0; k < kbf; ++k) current_node->children[k] = nullptr; //all empty
        }

        //if the spot for this branchs child is empty, make a new node
        if (current_node->children[child_idx_for_branch] == nullptr) {
            current_node->children[child_idx_for_branch] = create_bare_node();
            Node* new_child_node = current_node->children[child_idx_for_branch];
            new_child_node->stored_key = key_to_insert; //put key in new node
            return;
        } else { //go to the existing child
            current_node = current_node->children[child_idx_for_branch];
            //if this childs spot is empty, use it
            if (current_node->stored_key == EMPTY_KEY) {
                current_node->stored_key = key_to_insert;
                return;
            }
        }
    }
}

//prints keys in a part of the tree (goes left-self-right way)
//node is the current node were looking at
//kbf is how many children this node can have
//first_key_printed helps put spaces correctly
void print_branch_recursive(Node* node, int kbf, bool& first_key_printed) {
    if (node == nullptr) return; //nothing here, stop
    if (node->stored_key != EMPTY_KEY) { //if theres a key, print it
        if (first_key_printed) printf(" "); //put a space if its not the very first key
        printf("%d", node->stored_key);
        first_key_printed = true; //weve printed at least one key now
    }
    //do the same for all children
    if (kbf > 0 && node->children != nullptr) { //if it can have children and has the children array
        for (int i = 0; i < kbf; ++i) {
            print_branch_recursive(node->children[i], kbf, first_key_printed); //call self for each child
        }
    }
}

//prints all keys in the whole tree
//tree_root_ptr is the top of the tree
//rbf for roots children, kbf for others
void print_trie(Node* tree_root_ptr, int rbf, int kbf) {
    if (!tree_root_ptr) return; //no tree, nothing to print
    bool first_key_has_been_printed = false; //to manage spaces
    if (tree_root_ptr->stored_key != EMPTY_KEY) { //print roots key if it has one
        printf("%d", tree_root_ptr->stored_key);
        first_key_has_been_printed = true;
    }
    //print children of the root
    if (rbf > 0 && tree_root_ptr->children != nullptr) { //if root can have children and has array
        for (int j = 0; j < rbf; ++j) {
            //roots children use kbf for their own children
            print_branch_recursive(tree_root_ptr->children[j], kbf, first_key_has_been_printed);
        }
    }
    printf(" \n"); //space and newline at the end
}

//is this node a leaf - has no children
//node is the node we check
//branch_factor_of_this_node is how many children it could have
bool is_leaf(Node* node, int branch_factor_of_this_node) {
    if (node == nullptr) return true; //null is like a leaf
    if (branch_factor_of_this_node <= 0) return true; //cant have children
    if (node->children == nullptr) return true; //no children array made yet, so its a leaf
    //check if all child spots in the array are empty (nullptr)
    for (int i = 0; i < branch_factor_of_this_node; ++i) {
        if (node->children[i] != nullptr) return false; //found a child - not a leaf
    }
    return true; //no actual children found
}

//finds the very first leaf if you go down and left all the time
//used when deleting, to find a replacement key
//search_start_node is where we begin looking
//kbf is for children of nodes in this search
//out_parent_of_actual_leaf and out_child_idx_of_actual_leaf are about the leafs parent
Node* find_leftmost_leaf_descendant(
    Node* search_start_node, int kbf,
    Node** out_parent_of_actual_leaf, int* out_child_idx_of_actual_leaf)
{
    Node* iter_node = search_start_node; //start here
    *out_parent_of_actual_leaf = nullptr; //reset output info
    *out_child_idx_of_actual_leaf = -1;

    if (kbf <= 0) return iter_node; //can't have children, so its a leaf itself

    if (is_leaf(iter_node, kbf)) return iter_node; //its already a leaf

    //keep going down-left until we hit a leaf
    while (iter_node != nullptr && !is_leaf(iter_node, kbf)) {
        bool descended = false; //did it go down a level
        if (iter_node->children != nullptr) { //if it has a children array
            for (int i = 0; i < kbf; ++i) { //look for the first (leftmost) child
                if (iter_node->children[i] != nullptr) {
                    *out_parent_of_actual_leaf = iter_node; //current node is parent of next
                    *out_child_idx_of_actual_leaf = i; //this is the child's number
                    iter_node = iter_node->children[i]; //go to the child
                    descended = true;
                    break; //found the leftmost for this level
                }
            }
        }
        if (!descended) break; //couldnt go down, must be a leaf
    }
    return iter_node; //this is the leftmost leaf we found
}

//deletes a node and everything under it
//node_to_delete is the one to remove
//branch_factor_for_node_children is for its direct children
//kbf_for_all is for grandchildren and deeper
void delete_node_and_descendants_recursive(Node* node_to_delete, int branch_factor_for_node_children, int kbf_for_all) {
    if (node_to_delete == nullptr) return; //nothing to delete

    //if this node can have children and has the children array
    if (branch_factor_for_node_children > 0 && node_to_delete->children != nullptr) {
        for (int i = 0; i < branch_factor_for_node_children; ++i) {
            //children of this node use kbf_for_all for their children
            delete_node_and_descendants_recursive(node_to_delete->children[i], kbf_for_all, kbf_for_all);
        }
        delete[] node_to_delete->children; //delete the array of child pointers
        node_to_delete->children = nullptr; //important to set to null
    }
    delete node_to_delete; //delete the node itself
}

//helps delete a key from a branch (not the main root)
//current_node_ref is how parent points to this node, so we can change it
//value_for_path helps find the way
//original_key_to_delete is the target
//kbf is for branching in this part of tree
void delete_key_recursive_on_branch(
    Node*& current_node_ref, int value_for_path,
    int original_key_to_delete, int kbf)
{
    if (current_node_ref == nullptr) return; //key not here
    Node* p_node = current_node_ref; //the node were looking at

    //if this p_node has the key we want to delete
    if (p_node->stored_key == original_key_to_delete && p_node->stored_key != EMPTY_KEY) {
        if (is_leaf(p_node, kbf)) { //case 1: p_node is a leaf, easy delete
            delete_node_and_descendants_recursive(p_node, kbf, kbf); //delete it
            current_node_ref = nullptr; //tell parent its gone
        } else { //case 2: p_node is not a leaf, harder
            Node* q_node_parent = nullptr; int q_node_idx_in_q_parent = -1;
            //find a replacement key (q_node) from its leftmost leaf child
            Node* q_node = find_leftmost_leaf_descendant(p_node, kbf, &q_node_parent, &q_node_idx_in_q_parent);

            if (q_node != nullptr && q_node->stored_key != EMPTY_KEY) { //found a good replacement
                p_node->stored_key = q_node->stored_key; //copy q's key to p_node

                //now delete q_node from where it was
                if (q_node_parent != nullptr && q_node_idx_in_q_parent != -1) {
                    //q_node had a parent (q_node_parent) inside p_nodes branches
                    delete_node_and_descendants_recursive(q_node_parent->children[q_node_idx_in_q_parent], kbf, kbf);
                    q_node_parent->children[q_node_idx_in_q_parent] = nullptr;
                } else { //q_node was a direct child of p_node
                    bool detached_q = false;
                    if (p_node->children && q_node != p_node) { //q_node is not p_node
                        for(int i=0; i < kbf; ++i) { //find q among p's children
                            if(p_node->children[i] == q_node) {
                                delete_node_and_descendants_recursive(p_node->children[i], kbf, kbf);
                                p_node->children[i] = nullptr;
                                detached_q = true; break;
                            }
                        }
                    }
                    if (!detached_q) p_node->stored_key = EMPTY_KEY; //if failed, just empty p
                }
            } else { //no good replacement found, just empty p_node
                p_node->stored_key = EMPTY_KEY;
            }
        }
        return; //done with this key
    }

    //key not in p_node, go deeper
    if (kbf <= 0) return; //cant go deeper
    if (p_node->children == nullptr) return; //no children array to go into

    int next_child_idx = value_for_path % kbf; //next step in path
    value_for_path /= kbf; //update path value
    if (next_child_idx < 0 || next_child_idx >= kbf) return; //bad path

    delete_key_recursive_on_branch(p_node->children[next_child_idx], value_for_path, original_key_to_delete, kbf);
}

//deletes a key from the whole tree
//tree_root_ptr is the top
//rbf, kbf are branching factors
void delete_key(Node* tree_root_ptr, int key_to_delete, int rbf, int kbf) {
    if (!tree_root_ptr) return; //no tree
    if (key_to_delete == EMPTY_KEY) return; //cant delete this
    if (!lookup(tree_root_ptr, key_to_delete, rbf, kbf)) { //check if its even there
        printf("%d not exist\n", key_to_delete);
        return;
    }

    //case 1: key is in the root node itself
    if (tree_root_ptr->stored_key == key_to_delete && tree_root_ptr->stored_key != EMPTY_KEY) {
        if (is_leaf(tree_root_ptr, rbf)) { //root is a leaf, just empty it
            tree_root_ptr->stored_key = EMPTY_KEY;
        } else { //root is not a leaf, find replacement
            Node* q_node_parent_for_root_subtree = nullptr; int q_node_idx_in_parent_subtree = -1;
            Node* q_node_for_root = nullptr; int root_child_idx_leading_to_q = -1;

            if (tree_root_ptr->children != nullptr) { //root must have children if not a leaf (and rbf>0)
                for (int i = 0; i < rbf; ++i) { //look for replacement in roots children
                    if (tree_root_ptr->children[i] != nullptr) {
                        //children of root use kbf for their own childrens branching
                        q_node_for_root = find_leftmost_leaf_descendant(tree_root_ptr->children[i], kbf, &q_node_parent_for_root_subtree, &q_node_idx_in_parent_subtree);
                        if (q_node_for_root != nullptr && q_node_for_root->stored_key != EMPTY_KEY) {
                            root_child_idx_leading_to_q = i; //remember which child branch q came from
                            break; //found it
                        } else {
                            q_node_for_root = nullptr; //try next branch
                        }
                    }
                }
            }

            if (q_node_for_root != nullptr && q_node_for_root->stored_key != EMPTY_KEY) { //good replacement found
                tree_root_ptr->stored_key = q_node_for_root->stored_key; //copy key to root

                //delete q_node_for_root from where it was
                if (q_node_parent_for_root_subtree != nullptr && q_node_idx_in_parent_subtree != -1) {
                    //q was deeper down in one of root's child branches
                    delete_node_and_descendants_recursive(q_node_parent_for_root_subtree->children[q_node_idx_in_parent_subtree], kbf, kbf);
                    q_node_parent_for_root_subtree->children[q_node_idx_in_parent_subtree] = nullptr;
                } else if (root_child_idx_leading_to_q != -1 && tree_root_ptr->children != nullptr && q_node_for_root == tree_root_ptr->children[root_child_idx_leading_to_q]) {
                    //q was a direct child of the root
                    delete_node_and_descendants_recursive(tree_root_ptr->children[root_child_idx_leading_to_q], kbf, kbf);
                    tree_root_ptr->children[root_child_idx_leading_to_q] = nullptr;
                } else { //problem finding q's old spot, just empty root
                    tree_root_ptr->stored_key = EMPTY_KEY;
                }
            } else { //no good replacement
                tree_root_ptr->stored_key = EMPTY_KEY;
            }
        }
    }
    //case 2: key is not in root, so its in a deeper branch
    else {
        if (rbf > 0 && tree_root_ptr->children != nullptr) { //if root can have children and has array
            int value = key_to_delete; //for path
            int child_idx_at_root = value % rbf; //which child branch to go down
            value /= rbf; //update path value
            if (child_idx_at_root >= 0 && child_idx_at_root < rbf) { //valid path
                if (tree_root_ptr->children[child_idx_at_root] != nullptr) { //if that child branch exists
                    delete_key_recursive_on_branch(tree_root_ptr->children[child_idx_at_root], value, key_to_delete, kbf);
                }
            }
        }
    }
}

//cleans up all memory used by the tree
//tree_root_ptr is the top
//rbf, kbf for deleting correctly
void free_all(Node* tree_root_ptr, int rbf, int kbf) {
    if (!tree_root_ptr) return; //nothing to free
    //roots children array is size rbf; their children and deeper use kbf
    delete_node_and_descendants_recursive(tree_root_ptr, rbf, kbf);
}

int main(){
    int num_commands; //how many things to do
    scanf("%d", &num_commands);
    int min_val_range, max_val_range; //not really used here
    scanf("%d %d", &min_val_range, &max_val_range);
    int rbf_input, kbf_input; //branching factors from user
    scanf("%d %d", &rbf_input, &kbf_input);

    Node* actual_tree_root = create_bare_node(); //make the top of our tree

    char command_char;
    int key_value; //the number for the command
    for (int i = 0; i < num_commands; ++i) { //do all commands
        if (scanf(" %c", &command_char) != 1) return 1; //read command, check for error
        switch (command_char){
        case 'I': //insert
            if (scanf("%d", &key_value) != 1) return 1; //read number, check error
            insert(actual_tree_root, key_value, rbf_input, kbf_input);
            break;
        case 'L': //lookup
            if (scanf("%d", &key_value) != 1) return 1;
            if (lookup(actual_tree_root, key_value, rbf_input, kbf_input)) {
                printf("%d exist\n", key_value);
            } else {
                printf("%d not exist\n", key_value);
            }
            break;
        case 'D': //delete
            if (scanf("%d", &key_value) != 1) return 1;
            delete_key(actual_tree_root, key_value, rbf_input, kbf_input);
            break;
        case 'P': //print
            print_trie(actual_tree_root, rbf_input, kbf_input);
            break;
        default:
            //unknown command, just ignore
            break;
        }
    }

    free_all(actual_tree_root, rbf_input, kbf_input); //clean up memory
    return 0;
}