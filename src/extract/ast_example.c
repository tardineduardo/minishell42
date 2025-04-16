/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_example.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:27:39 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/15 11:29:02 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Node types
typedef enum {
    NODE_COMMAND,
    NODE_PIPELINE,
    NODE_LOGICAL,
    NODE_GROUP
} NodeType;

// Logical operators
typedef enum {
    OP_NONE,
    OP_AND,
    OP_OR
} LogicalOp;

// AST Node structure
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            char **argv;
            int argc;
        } command;
        struct {
            struct ASTNode **commands;
            int command_count;
        } pipeline;
        struct {
            LogicalOp op;
            struct ASTNode *left;
            struct ASTNode *right;
        } logical;
        struct {
            struct ASTNode *body;
        } group;
    } data;
} ASTNode;

// Token structure
typedef struct Token {
    char *value;
    struct Token *next;
} Token;

// Function prototypes
ASTNode *parse_expression(Token **tokens);
ASTNode *parse_logical_or(Token **tokens);
ASTNode *parse_logical_and(Token **tokens);
ASTNode *parse_pipeline(Token **tokens);
ASTNode *parse_command_or_group(Token **tokens);
ASTNode *parse_command(Token **tokens);
Token *tokenize(const char *input);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int depth);

// Create a new command node
ASTNode *create_command_node(char **argv, int argc) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_COMMAND;
    node->data.command.argv = argv;
    node->data.command.argc = argc;
    return node;
}

// Create a new pipeline node
ASTNode *create_pipeline_node(ASTNode **commands, int count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PIPELINE;
    node->data.pipeline.commands = commands;
    node->data.pipeline.command_count = count;
    return node;
}

// Create a new logical node
ASTNode *create_logical_node(LogicalOp op, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_LOGICAL;
    node->data.logical.op = op;
    node->data.logical.left = left;
    node->data.logical.right = right;
    return node;
}

// Create a new group node
ASTNode *create_group_node(ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_GROUP;
    node->data.group.body = body;
    return node;
}

// Parse an expression
ASTNode *parse_expression(Token **tokens) {
    return parse_logical_or(tokens);
}

// Parse logical OR (||)
ASTNode *parse_logical_or(Token **tokens) {
    ASTNode *node = parse_logical_and(tokens);
    
    while (*tokens && strcmp((*tokens)->value, "||") == 0) {
        *tokens = (*tokens)->next; // Consume "||"
        ASTNode *right = parse_logical_and(tokens);
        node = create_logical_node(OP_OR, node, right);
    }
    
    return node;
}

// Parse logical AND (&&)
ASTNode *parse_logical_and(Token **tokens) {
    ASTNode *node = parse_pipeline(tokens);
    
    while (*tokens && strcmp((*tokens)->value, "&&") == 0) {
        *tokens = (*tokens)->next; // Consume "&&"
        ASTNode *right = parse_pipeline(tokens);
        node = create_logical_node(OP_AND, node, right);
    }
    
    return node;
}

// Parse pipeline (|)
ASTNode *parse_pipeline(Token **tokens) {
    ASTNode *node = parse_command_or_group(tokens);
    
    while (*tokens && strcmp((*tokens)->value, "|") == 0) {
        *tokens = (*tokens)->next; // Consume "|"
        ASTNode *right = parse_command_or_group(tokens);
        
        if (node->type == NODE_PIPELINE) {
            // Extend existing pipeline
            int new_count = node->data.pipeline.command_count + 1;
            ASTNode **new_commands = realloc(node->data.pipeline.commands, 
                                           new_count * sizeof(ASTNode*));
            new_commands[new_count - 1] = right;
            node->data.pipeline.commands = new_commands;
            node->data.pipeline.command_count = new_count;
        } else {
            // Create new pipeline
            ASTNode **commands = malloc(2 * sizeof(ASTNode*));
            commands[0] = node;
            commands[1] = right;
            node = create_pipeline_node(commands, 2);
        }
    }
    
    return node;
}

// Parse either a command or a group
ASTNode *parse_command_or_group(Token **tokens) {
    if (*tokens && strcmp((*tokens)->value, "(") == 0) {
        *tokens = (*tokens)->next; // Consume "("
        ASTNode *node = parse_expression(tokens);
        if (!*tokens || strcmp((*tokens)->value, ")") != 0) {
            fprintf(stderr, "Error: Unclosed parenthesis\n");
            exit(1);
        }
        *tokens = (*tokens)->next; // Consume ")"
        return create_group_node(node);
    } else {
        return parse_command(tokens);
    }
}

// Parse a simple command
ASTNode *parse_command(Token **tokens) {
    char **argv = malloc(16 * sizeof(char*)); // Start with space for 16 args
    int argc = 0;
    int capacity = 16;
    
    while (*tokens && strcmp((*tokens)->value, "|") != 0 &&
                     strcmp((*tokens)->value, "&&") != 0 &&
                     strcmp((*tokens)->value, "||") != 0 &&
                     strcmp((*tokens)->value, ")") != 0) {
        if (argc >= capacity) {
            capacity *= 2;
            argv = realloc(argv, capacity * sizeof(char*));
        }
        argv[argc++] = strdup((*tokens)->value);
        *tokens = (*tokens)->next;
    }
    
    if (argc == 0) {
        fprintf(stderr, "Error: Empty command\n");
        exit(1);
    }
    
    argv = realloc(argv, argc * sizeof(char*)); // Trim to actual size
    return create_command_node(argv, argc);
}

// Tokenize the input string
Token *tokenize(const char *input) {
    Token *head = NULL;
    Token **current = &head;
    const char *p = input;
    
    while (*p) {
        // Skip whitespace
        while (isspace(*p)) p++;
        if (!*p) break;
        
        // Handle operators
        if (strchr("|&();<>", *p)) {
            // Check for multi-character operators
            if (*p == '|' && *(p+1) == '|') {
                *current = malloc(sizeof(Token));
                (*current)->value = strdup("||");
                (*current)->next = NULL;
                current = &(*current)->next;
                p += 2;
                continue;
            } else if (*p == '&' && *(p+1) == '&') {
                *current = malloc(sizeof(Token));
                (*current)->value = strdup("&&");
                (*current)->next = NULL;
                current = &(*current)->next;
                p += 2;
                continue;
            }
            
            // Single-character operator
            char op[2] = {*p, '\0'};
            *current = malloc(sizeof(Token));
            (*current)->value = strdup(op);
            (*current)->next = NULL;
            current = &(*current)->next;
            p++;
            continue;
        }
        
        // Handle words (commands and arguments)
        const char *start = p;
        while (*p && !isspace(*p) && !strchr("|&();<>", *p)) p++;
        
        char *word = strndup(start, p - start);
        *current = malloc(sizeof(Token));
        (*current)->value = word;
        (*current)->next = NULL;
        current = &(*current)->next;
    }
    
    return head;
}

// Free AST memory
void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_COMMAND:
            for (int i = 0; i < node->data.command.argc; i++) {
                free(node->data.command.argv[i]);
            }
            free(node->data.command.argv);
            break;
        case NODE_PIPELINE:
            for (int i = 0; i < node->data.pipeline.command_count; i++) {
                free_ast(node->data.pipeline.commands[i]);
            }
            free(node->data.pipeline.commands);
            break;
        case NODE_LOGICAL:
            free_ast(node->data.logical.left);
            free_ast(node->data.logical.right);
            break;
        case NODE_GROUP:
            free_ast(node->data.group.body);
            break;
    }
    
    free(node);
}

// Print AST for debugging
void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_COMMAND:
            printf("COMMAND:");
            for (int i = 0; i < node->data.command.argc; i++) {
                printf(" %s", node->data.command.argv[i]);
            }
            printf("\n");
            break;
        case NODE_PIPELINE:
            printf("PIPELINE:\n");
            for (int i = 0; i < node->data.pipeline.command_count; i++) {
                print_ast(node->data.pipeline.commands[i], depth + 1);
            }
            break;
        case NODE_LOGICAL:
            printf("LOGICAL %s:\n", 
                  node->data.logical.op == OP_AND ? "AND" : "OR");
            print_ast(node->data.logical.left, depth + 1);
            print_ast(node->data.logical.right, depth + 1);
            break;
        case NODE_GROUP:
            printf("GROUP:\n");
            print_ast(node->data.group.body, depth + 1);
            break;
    }
}

// Free tokens
void free_tokens(Token *tokens) {
    while (tokens) {
        Token *next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}

int main() {
    const char *input = "cmd1 && (cmd2 arg1 | cmd3) || cmd4 > out.txt";
    
    printf("Input: %s\n", input);
    
    // Tokenize
    Token *tokens = tokenize(input);
    
    // Parse
    ASTNode *ast = parse_expression(&tokens);
    
    if (tokens) {
        fprintf(stderr, "Error: Unexpected token '%s'\n", tokens->value);
        free_tokens(tokens);
        free_ast(ast);
        return 1;
    }
    
    // Print AST
    printf("\nAST:\n");
    print_ast(ast, 0);
    
    // Cleanup
    free_ast(ast);
    
    return 0;
}