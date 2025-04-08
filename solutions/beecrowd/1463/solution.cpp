#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cctype>
using namespace std;

struct Node {
    char value;
    Node* left;
    Node* right;
    Node(char val) : value(val), left(nullptr), right(nullptr) {}
    Node(char val, Node* l, Node* r) : value(val), left(l), right(r) {}
};

class Solution {
private:
    Node* buildNode(char op, Node* right, Node* left) {
        // ordem correta é op(direita, esquerda)
        return new Node(op, left, right);
    }

    // retorna true se op1 for operador e a prioridade(op1) >= prioridade(op2)
    bool compare(char op1, char op2) {
        if (op1 == '(' || op1 == ')') return false;
        return op1 == '*' || op1 == '/' || op2 == '+' || op2 == '-';
    }

    char pop(stack<char>& ops) {
        const char op = ops.top();
        ops.pop();
        return op;
    }

    Node* pop(stack<Node*>& nodes) {
        Node* node = nodes.top();
        nodes.pop();
        return node;
    }

public:
    Node* expTree(string s) {
        stack<Node*> nodes;  // stack para nós
        stack<char> ops;     // stack para operadores e parênteses

        for (const char c : s) {
            if (c == ' ') continue; // pula espaços

            if (isalnum(c)) { // se é letra ou dígito
                nodes.push(new Node(c));
            }
            else if (c == '(') {
                ops.push(c);
            }
            else if (c == ')') {
                while (!ops.empty() && ops.top() != '(')
                    nodes.push(buildNode(pop(ops), pop(nodes), pop(nodes)));
                if (!ops.empty()) ops.pop();  // remove '('
            }
            else if (c == '+' || c == '-' || c == '*' || c == '/') {
                while (!ops.empty() && ops.top() != '(' && compare(ops.top(), c))
                    nodes.push(buildNode(pop(ops), pop(nodes), pop(nodes)));
                ops.push(c);
            }
        }

        while (!ops.empty())
            nodes.push(buildNode(pop(ops), pop(nodes), pop(nodes)));

        return nodes.empty() ? nullptr : nodes.top();
    }

    void printLevelByLevel(Node* root) {
        if (!root) return;

        queue<Node*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int size = q.size();
            cout << "Nivel " << level << ": ";

            for (int i = 0; i < size; i++) {
                Node* current = q.front();
                q.pop();

                cout << current->value;

                if (current->left)
                    q.push(current->left);
                if (current->right)
                    q.push(current->right);
            }

            cout << endl;
            level++;
        }
        cout << endl;
    }

    void freeTree(Node* root) {
        if (root) {
            freeTree(root->left);
            freeTree(root->right);
            delete root;
        }
    }
};

int main() {
    Solution solution;
    string expressions[] = {
        "4 * a - ( 6 + b ) + 8 / ( 9 - 7 )",
        "a + b",
        "( a + b * c ) * a - 4 * 5 - 6 + 1 + c * 3"
    };

    for (const string& expr : expressions) {
        Node* root = solution.expTree(expr);
        solution.printLevelByLevel(root);
        solution.freeTree(root);
    }

    return 0;
}