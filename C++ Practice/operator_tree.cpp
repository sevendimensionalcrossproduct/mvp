#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <cmath>

enum token_type { NUMBER, OPERATOR, VARIABLE };
enum associativity { LEFT, RIGHT };

int get_precedence(char some_operator){
    switch(some_operator){
        case '+': 
        case '-':  
            return 1;
        case '*': 
        case '/': 
            return 2;
        case '^':
            return 3;
        default: return 0;
    }
}

associativity get_associativity(char some_operator){
    switch(some_operator){
        case '+':case '-': case '*': case '/':
            return LEFT;
        case '^':
            return RIGHT;
        default:
            return LEFT;
    }
}

struct token {
    token_type type;
    double value;
    char opperateur;
    char variable;

    token(double number_value) : type(NUMBER), value(number_value), opperateur(0), variable(0) {}

    token(char input) {
        if (std::isalpha(input)) {
            type = VARIABLE;
            variable = input;
            opperateur = 0;
            value = 0;
        } else {
            type = OPERATOR;
            opperateur = input;
            variable = 0;
            value = 0;
        }
    }
};

struct node {
    enum node_type { NUMBER_NODE, OPERATOR_NODE, VARIABLE_NODE} type;
    double node_value;
    char node_operator;
    char node_variable;
    node* left_child;
    node* right_child;
    
    node(double value) : 
        type(NUMBER_NODE), node_value(value), node_operator(0), node_variable(0), left_child(nullptr), right_child(nullptr) {}
    node(char operador, node* lhs, node* rhs) : 
        type(OPERATOR_NODE), node_value(0), node_operator(operador), node_variable(0), left_child(lhs), right_child(rhs) {}
    node(char variable_name) : 
        type(VARIABLE_NODE), node_value(0), node_operator(0), node_variable(variable_name), left_child(nullptr), right_child(nullptr) {}

};

std::vector<token> tokenize(const std::string &expression){
    std::vector<token> tokens;
    std::istringstream iss(expression);
    char c;
    
    while (iss >> c){
        if (std::isdigit(c)) {
            iss.putback(c);

            double current_number;
            iss >> current_number;
            tokens.emplace_back(token(current_number));
        } else if (std::isalpha(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^') {
            tokens.emplace_back(token(c));
        } else {
            throw std::runtime_error("Unknown Token: " + std::string(1, c));
        }
    }
    return tokens;
}

void print_token(const token &some_token){
    if(some_token.type == NUMBER){
        std::cout << "Num:" << some_token.value;
    } else if (some_token.type == OPERATOR){
        std::cout << "Op:" << some_token.opperateur << " p=" << get_precedence(some_token.opperateur);
    } else if (some_token.type == VARIABLE){
        std::cout << "Var:" << some_token.variable;
    }
}

void print_tokens(const std::vector<token> &tokens){
    printf("<");
    for (size_t i = 0; i < tokens.size(); i++){
        print_token(tokens[i]);
        if (i < tokens.size() -1){
            printf(", ");
        }
    }
    printf(">");
}

node* rpl_tree(const std::vector<token> &tokens){
    std::stack<node*> tree;
    std::stack<token> operators;

    for(const token& current_token : tokens) {
        if (current_token.type == NUMBER){
            tree.push(new node(current_token.value));
        } else if (current_token.type == VARIABLE){
            tree.push(new node(current_token.variable));
        } else if (current_token.type == OPERATOR) {
            while (!operators.empty() &&
                (get_precedence(operators.top().opperateur) > get_precedence(current_token.opperateur) ||
                (get_precedence(operators.top().opperateur) == get_precedence(current_token.opperateur) &&
                get_associativity(current_token.opperateur) == LEFT))) {
                    char operation = operators.top().opperateur;
                    operators.pop();
                    node* right = tree.top(); tree.pop();
                    node* left = tree.top(); tree.pop();
                    tree.push(new node(operation, left, right));
            }
            operators.push(current_token);
        }
    }

    while(!operators.empty()){
        char operation = operators.top().opperateur;
        operators.pop();
        node* right = tree.top(); tree.pop();
        node* left = tree.top(); tree.pop();
        tree.push(new node(operation, left, right));
    }
    return tree.top();
}

std::vector<token> generate_rpl(node* root){
    std::vector<token> rpl;

    if (root) {
        if (root->type == node::NUMBER_NODE) {
            rpl.emplace_back(token(root->node_value));
        } else if (root->type == node::VARIABLE_NODE) {
            rpl.emplace_back(token(root->node_variable));
        } else if (root->type == node::OPERATOR_NODE) {
            std::vector<token> left_rpl = generate_rpl(root->left_child);
            rpl.insert(rpl.end(), left_rpl.begin(), left_rpl.end());       

            std::vector<token> right_rpl = generate_rpl(root->right_child);
            rpl.insert(rpl.end(), right_rpl.begin(), right_rpl.end());       

            rpl.emplace_back(token(root->node_operator));
        }
    }
    return rpl;
}

std::string show_expression(const std::vector<token> &tokens) {
    std::ostringstream expression;
    for (const token &current_token : tokens) {
        if (current_token.type == NUMBER) {
            expression << current_token.value << " ";
        } else if (current_token.type == OPERATOR) {
            expression << current_token.opperateur << " ";
        } else if (current_token.type == VARIABLE) {
            expression << current_token.variable << " ";
        }
    }
    return expression.str();
}


int main(){
    std::string input;
    std::cout << ("Expression: ");
    std::getline(std::cin, input);
    
    std::vector<token> tokens = tokenize(input);
    printf("Tokenized Input: ");
    print_tokens(tokens);

    std::vector<token> output_tokens = generate_rpl(rpl_tree(tokens));
    printf("\nTokenized Output: ");
    print_tokens(output_tokens);

    printf("\nExpression: ");
    std::cout << show_expression(output_tokens);

}


