#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct BranchNode {
    int stored_key;
    bool is_key_present;
    struct BranchNode** children;
} BranchNode;

typedef struct RootNode {
    int stored_key;
    bool is_key_present;
    BranchNode** children;
    int root_branch_factor;
    int k_branch_factor;
} RootNode;

void init_root(RootNode* root, int n, int k) {
    if (!root) return;
    root->root_branch_factor = n;
    root->k_branch_factor = k;
    root->is_key_present = false;
    root->stored_key = -1;
    root->children = nullptr;
    if (n > 0) {
        root->children = (BranchNode**)calloc(n, sizeof(BranchNode*));
    }
}

BranchNode* create_branch_node(int k) {
    BranchNode* b_node = (BranchNode*)malloc(sizeof(BranchNode));
    if (!b_node) {
        return nullptr;
    }
    b_node->is_key_present = false;
    b_node->stored_key = -1;
    b_node->children = nullptr;
    if (k > 0) {
        b_node->children = (BranchNode**)calloc(k, sizeof(BranchNode*));
        if (!b_node->children) {
            free(b_node);
            return nullptr;
        }
    }
    return b_node;
}

bool is_root_empty(RootNode* root) {
    if (!root) return false;
    if (root->is_key_present) {
        return false;
    }
    if (root->root_branch_factor <= 0 || root->children == nullptr) {
        return true;
    }
    for (int i = 0; i < root->root_branch_factor; ++i) {
        if (root->children[i] != nullptr) {
            return false;
        }
    }
    return true;
}

bool lookup_key(RootNode* root, int key_to_find) {
    if (!root) return false;
    if (root->is_key_present && root->stored_key == key_to_find) {
        return true;
    }
    if (root->root_branch_factor <= 0 || root->children == nullptr) {
        return false;
    }
    int path_val = key_to_find;
    int child_idx = path_val % root->root_branch_factor;
    path_val /= root->root_branch_factor;
    if (child_idx < 0 || child_idx >= root->root_branch_factor) {
        return false;
    }
    BranchNode* current = root->children[child_idx];
    while (current != nullptr) {
        if (current->is_key_present && current->stored_key == key_to_find) {
            return true;
        }
        if (root->k_branch_factor <= 0) {
            break;
        }
        if (current->children == nullptr) {
            break;
        }
        int next_child_idx = path_val % root->k_branch_factor;
        path_val /= root->k_branch_factor;
        if (next_child_idx < 0 || next_child_idx >= root->k_branch_factor) {
            break;
        }
        current = current->children[next_child_idx];
    }
    return false;
}

void insert_key(RootNode* root, int key) {
    if (!root) return;
    if (lookup_key(root, key)) {
        cout << key << " exist" << endl;
        return;
    }
    if (is_root_empty(root)) {
        root->stored_key = key;
        root->is_key_present = true;
        return;
    }
    int path_val = key;
    int child_idx = path_val % root->root_branch_factor;
    path_val /= root->root_branch_factor;
    if (child_idx < 0 || child_idx >= root->root_branch_factor) {
        return;
    }
    if (root->children == nullptr) {
        return;
    }
    if (root->children[child_idx] == nullptr) {
        BranchNode* new_node = create_branch_node(root->k_branch_factor);
        if (!new_node) {
            return;
        }
        new_node->stored_key = key;
        new_node->is_key_present = true;
        root->children[child_idx] = new_node;
        return;
    }
    BranchNode* current = root->children[child_idx];
    while (true) {
        if (root->k_branch_factor <= 0) {
            return;
        }
        if (current->children == nullptr) {
            return;
        }
        int next_idx = path_val % root->k_branch_factor;
        path_val /= root->k_branch_factor;
        if (next_idx < 0 || next_idx >= root->k_branch_factor) {
            return;
        }
        if (current->children[next_idx] == nullptr) {
            BranchNode* new_node = create_branch_node(root->k_branch_factor);
            if (!new_node) {
                return;
            }
            new_node->stored_key = key;
            new_node->is_key_present = true;
            current->children[next_idx] = new_node;
            return;
        } else {
            current = current->children[next_idx];
            if (path_val == 0 && !current->is_key_present) {
                current->stored_key = key;
                current->is_key_present = true;
                return;
            }
        }
    }
}

bool is_leaf(BranchNode* node, int k) {
    if (node == nullptr) return true;
    if (node->children == nullptr) return true;
    for (int i = 0; i < k; ++i) {
        if (node->children[i] != nullptr) {
            return false;
        }
    }
    return true;
}

BranchNode* find_leftmost_leaf_descendant(BranchNode* start_node, int k, BranchNode** out_parent, int* out_index) {
    if (start_node == nullptr) {
        *out_parent = nullptr;
        *out_index = -1;
        return nullptr;
    }
    if (is_leaf(start_node, k)) {
        *out_parent = nullptr;
        *out_index = -1;
        return start_node;
    }
    BranchNode* current = start_node;
    BranchNode* parent = nullptr;
    int index_in_parent = -1;
    while (true) {
        bool found_child = false;
        for (int i = 0; i < k; ++i) {
            if (current->children != nullptr && current->children[i] != nullptr) {
                parent = current;
                index_in_parent = i;
                current = current->children[i];
                found_child = true;
                break;
            }
        }
        if (!found_child || is_leaf(current, k)) {
            break;
        }
    }
    *out_parent = parent;
    *out_index = index_in_parent;
    return current;
}

void delete_recursive(BranchNode* current_node, BranchNode* parent_node, int index_in_parent, int path_value, int key, RootNode* root) {
    if (current_node == nullptr) {
        return;
    }
    if (current_node->is_key_present && current_node->stored_key == key) {
        if (is_leaf(current_node, root->k_branch_factor)) {
            current_node->is_key_present = false;
            current_node->stored_key = -1;
            if (parent_node != nullptr) {
                parent_node->children[index_in_parent] = nullptr;
            } else {
                if (index_in_parent >= 0 && index_in_parent < root->root_branch_factor) {
                    root->children[index_in_parent] = nullptr;
                }
            }
            if (current_node->children != nullptr) {
                free(current_node->children);
            }
            free(current_node);
        } else {
            BranchNode* q_parent = nullptr;
            int q_index = -1;
            BranchNode* q_node = find_leftmost_leaf_descendant(current_node, root->k_branch_factor, &q_parent, &q_index);
            if (q_node != nullptr && q_node->is_key_present) {
                current_node->stored_key = q_node->stored_key;
                q_node->is_key_present = false;
                q_node->stored_key = -1;
                if (q_parent != nullptr) {
                    q_parent->children[q_index] = nullptr;
                } else {
                    if (current_node->children != nullptr) {
                        for (int i = 0; i < root->k_branch_factor; ++i) {
                            if (current_node->children[i] == q_node) {
                                current_node->children[i] = nullptr;
                                break;
                            }
                        }
                    }
                }
                if (q_node->children != nullptr) {
                    free(q_node->children);
                }
                free(q_node);
            } else {
                current_node->is_key_present = false;
                current_node->stored_key = -1;
            }
        }
        return;
    }
    if (root->k_branch_factor <= 0 || current_node->children == nullptr) {
        return;
    }
    int next_child_idx = path_value % root->k_branch_factor;
    path_value /= root->k_branch_factor;
    if (next_child_idx < 0 || next_child_idx >= root->k_branch_factor) {
        return;
    }
    delete_recursive(current_node->children[next_child_idx], current_node, next_child_idx, path_value, key, root);
}

void delete_key(RootNode* root, int key) {
    if (!root) return;
    if (root->is_key_present && root->stored_key == key) {
        bool root_has_children = false;
        if (root->root_branch_factor > 0 && root->children != nullptr) {
            for (int i = 0; i < root->root_branch_factor; ++i) {
                if (root->children[i] != nullptr) {
                    root_has_children = true;
                    break;
                }
            }
        }
        if (!root_has_children) {
            root->is_key_present = false;
            root->stored_key = -1;
        } else {
            BranchNode* q_parent = nullptr;
            int q_index = -1;
            BranchNode* q_node = nullptr;
            for (int i = 0; i < root->root_branch_factor; ++i) {
                if (root->children != nullptr && root->children[i] != nullptr) {
                    q_node = find_leftmost_leaf_descendant(root->children[i], root->k_branch_factor, &q_parent, &q_index);
                    if (q_node != nullptr && q_node->is_key_present) {
                        break;
                    }
                    q_node = nullptr;
                }
            }
            if (q_node != nullptr) {
                root->stored_key = q_node->stored_key;
                q_node->is_key_present = false;
                if (q_parent != nullptr) {
                    q_parent->children[q_index] = nullptr;
                } else {
                    for (int i = 0; i < root->root_branch_factor; ++i) {
                        if (root->children[i] == q_node) {
                            root->children[i] = nullptr;
                            break;
                        }
                    }
                }
                if (q_node->children != nullptr) {
                    free(q_node->children);
                }
                free(q_node);
            } else {
                root->is_key_present = false;
                root->stored_key = -1;
            }
        }
    } else {
        if (root->root_branch_factor > 0 && root->children != nullptr) {
            int path_val = key;
            int child_idx = path_val % root->root_branch_factor;
            path_val /= root->root_branch_factor;
            if (child_idx >= 0 && child_idx < root->root_branch_factor && root->children[child_idx] != nullptr) {
                delete_recursive(root->children[child_idx], nullptr, child_idx, path_val, key, root);
            }
        }
    }
}

void print_branch_recursive(BranchNode* node, int k, bool& first_printed) {
    if (node == nullptr) {
        return;
    }
    if (node->is_key_present) {
        if (!first_printed) {
            cout << node->stored_key;
            first_printed = true;
        } else {
            cout << " " << node->stored_key;
        }
    }
    if (k > 0 && node->children != nullptr) {
        for (int i = 0; i < k; ++i) {
            print_branch_recursive(node->children[i], k, first_printed);
        }
    }
}

void print_tree(RootNode* root) {
    bool first_printed = false;
    if (root->is_key_present) {
        cout << root->stored_key;
        first_printed = true;
    }
    if (root->children != nullptr && root->root_branch_factor > 0) {
        for (int i = 0; i < root->root_branch_factor; ++i) {
            print_branch_recursive(root->children[i], root->k_branch_factor, first_printed);
        }
    }
    cout << ' '<<endl;
}

int main() {
    int num_commands;
    cin >> num_commands;
    int min_val, max_val;
    cin >> min_val >> max_val;
    int rbf, kbf;
    cin >> rbf >> kbf;
    RootNode root;
    init_root(&root, rbf, kbf);
    char command;
    int key;
    for (int i = 0; i < num_commands; ++i) {
        cin >> command;
        switch (command) {
            case 'I': {
                cin >> key;
                insert_key(&root, key);
                break;
            }
            case 'L': {
                cin >> key;
                if (lookup_key(&root, key)) {
                    cout << key << " exist" << endl;
                } else {
                    cout << key << " not exist" << endl;
                }
                break;
            }
            case 'D': {
                cin >> key;
                if (lookup_key(&root, key)) {
                    delete_key(&root, key);
                } else {
                    cout << key << " not exist" << endl;
                }
                break;
            }
            case 'P': {
                print_tree(&root);
                break;
            }
        }
    }
    return 0;
}