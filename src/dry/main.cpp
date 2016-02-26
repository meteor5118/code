/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <atomic>
#include <string>
#include <algorithm>
#include <typeinfo>

#include <getopt.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <queue>

using namespace std;

class TreeNode {
public:
    int val;
    TreeNode *left, *right;
    TreeNode(int val) {
        this->val = val;
        this->left = this->right = NULL;
    }
};

TreeNode *build(vector<int>::iterator pre_begin,
           vector<int>::iterator pre_end,
           vector<int>::iterator in_begin,
           vector<int>::iterator in_end) {
    if (pre_begin >= pre_end)
        return nullptr;

    TreeNode *root = new TreeNode(*pre_begin);
    root->left = build(pre_begin + 1, pre_begin + (std::find(in_begin, in_end, *pre_begin) - in_begin),
                    in_begin, std::find(in_begin, in_end, *pre_begin));
    root->right = build(pre_begin + 1 + (std::find(in_begin, in_end, *pre_begin) - in_begin), pre_end,
                    std::find(in_begin, in_end, *pre_begin) + 1, in_end);

    return root;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    return build(preorder.begin(), preorder.end(), inorder.begin(), inorder.end());
}

int main() {
    vector<int> preorder{1, 2};
    vector<int> inorder{1, 2};
    TreeNode *root = buildTree(preorder, inorder);

    return 0;
}

