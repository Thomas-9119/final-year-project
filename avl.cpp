#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

class Node {
public:
    int value;
    Node* left;
    Node* right;
    int height;

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node) {
        if (node)
            node->height = 1 + max(height(node->left), height(node->right));
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);

        if (val < node->value)
            node->left = insert(node->left, val);
        else if (val > node->value)
            node->right = insert(node->right, val);
        else
            return node; // Duplicate not allowed

        updateHeight(node);
        int bf = balanceFactor(node);

        // Left Left Case
        if (bf > 1 && val < node->left->value)
            return rightRotate(node);

        // Right Right Case
        if (bf < -1 && val > node->right->value)
            return leftRotate(node);

        // Left Right Case
        if (bf > 1 && val > node->left->value) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (bf < -1 && val < node->right->value) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* node, int val) {
        if (!node) return node;

        if (val < node->value)
            node->left = deleteNode(node->left, val);
        else if (val > node->value)
            node->right = deleteNode(node->right, val);
        else {
            // Node found
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            // Two children
            Node* temp = minValueNode(node->right);
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->value);
        }

        updateHeight(node);
        int bf = balanceFactor(node);

        // Left Left
        if (bf > 1 && balanceFactor(node->left) >= 0)
            return rightRotate(node);

        // Left Right
        if (bf > 1 && balanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right
        if (bf < -1 && balanceFactor(node->right) <= 0)
            return leftRotate(node);

        // Right Left
        if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorder(Node* node) {
        if (node) {
            inorder(node->left);
            cout << node->value << " ";
            inorder(node->right);
        }
    }

    void preorder(Node* node) {
        if (node) {
            cout << node->value << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }

    void postorder(Node* node) {
        if (node) {
            postorder(node->left);
            postorder(node->right);
            cout << node->value << " ";
        }
    }

    void printTree(Node* node, int space = 0, int height = 10) {
        if (!node) return;
        space += height;
        printTree(node->right, space, height);
        cout << endl;
        for (int i = height; i < space; i++) cout << " ";
        cout << node->value << "(BF:" << balanceFactor(node) << ")" << endl;
        printTree(node->left, space, height);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int val) {
        root = insert(root, val);
    }

    void remove(int val) {
        root = deleteNode(root, val);
    }

    void inorderTraversal() {
        cout << "Inorder: ";
        inorder(root);
        cout << endl;
    }

    void preorderTraversal() {
        cout << "Preorder: ";
        preorder(root);
        cout << endl;
    }

    void postorderTraversal() {
        cout << "Postorder: ";
        postorder(root);
        cout << endl;
    }

    void displayTree() {
        cout << "\nAVL Tree Structure (with Balance Factor):\n";
        printTree(root);
        cout << endl;
    }

    bool isEmpty() {
        return root == nullptr;
    }
};

int main() {
    AVLTree tree;
    int choice, value;

    cout << "=== AVL Tree Implementation in C++ ===\n";

    do {
        cout << "\n---------------- MENU ----------------\n";
        cout << "1. Insert a number\n";
        cout << "2. Delete a number\n";
        cout << "3. Inorder Traversal\n";
        cout << "4. Preorder Traversal\n";
        cout << "5. Postorder Traversal\n";
        cout << "6. Display Tree Structure\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter number to insert: ";
                cin >> value;
                tree.insert(value);
                cout << value << " inserted successfully.\n";
                break;

            case 2:
                cout << "Enter number to delete: ";
                cin >> value;
                tree.remove(value);
                cout << "Attempted to delete " << value << ".\n";
                break;

            case 3:
                tree.inorderTraversal();
                break;

            case 4:
                tree.preorderTraversal();
                break;

            case 5:
                tree.postorderTraversal();
                break;

            case 6:
                if (tree.isEmpty())
                    cout << "Tree is empty!\n";
                else
                    tree.displayTree();
                break;

            case 7:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}