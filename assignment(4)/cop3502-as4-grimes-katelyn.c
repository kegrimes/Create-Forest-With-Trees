/*  Katelyn Grimes
    August 1, 2022
    COP 3502
    The purpose of this program is to add items into a tree and
    search, delete, count, etc. the tree.
    It will read from an input file, create the tree, call
    basic command functions based on the input file, and write
    it to the output file. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "leak_detector_c.h"

struct item_node_struct typedef item_node;

struct tree_name_node_struct
{
    char treeName[32];
    struct tree_name_node_struct *parent;
    struct tree_name_node_struct *left;
    struct tree_name_node_struct *right;
    item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

struct item_node_struct
{
    char name[32];
    int count;
    struct item_node_struct *iparent;
    struct item_node_struct *left;
    struct item_node_struct *right;
    struct tree_name_node_struct *tparent;
};
typedef struct item_node_struct item_node;

/* Tree Name Constructor:
    Allocates memory for a tree name for the main tree. String
    copies treeName and also sets parent, right child, left
    child, and pointer to item node pointers to null. Returns
    the tree name node. */

tree_name_node *new_tree_name_node(char *treeName)
{
    tree_name_node *t = malloc(sizeof(tree_name_node));

    strcpy(t->treeName, treeName);
    t->parent = NULL;
    t->right = NULL;
    t->left = NULL;
    t->theTree = NULL;

    return t;
}

/* Item Node Constructor:
    Allocates memory for an item node. Takes in name, count,
    and tparent to construct in the item i. Also, sets iparent,
    right child, and left child to null. Returns the item node. */

item_node *new_item_node(char *name, int count, tree_name_node *tparent)
{
    item_node *i = malloc(sizeof(item_node));

    strcpy(i->name, name);
    i->count = count;
    i->iparent = NULL;
    i->right = NULL;
    i->left = NULL;
    i->tparent = tparent;

    return i;
}

/* Dispose Item Node:
    Takes in an item node, and frees it. */

void dispose_item_node(item_node *i)
{
    free(i);
}

/* Dispose Tree Name:
    Takes in a tree name, and frees it. */

void dispose_tree_name(tree_name_node *t)
{
    free(t);
}

/* Class Library Function:
    remove_crlf() will remove any new line characters and carriage returns */
void remove_crlf(char *s)
{
    char *end = s + strlen(s);
    end--;

    while ((end >= s) && (*end == '\n' || *end == '\r'))
    {
        *end = '\0';
        end--;
    }
}

/* Class Library Function:
    get_next_nonblank_line() will read the next nonblank line from an open file */
int get_next_nonblank_line (FILE *input, char *ifileBuf, int max_length)
{
    ifileBuf[0] = '\0';
    while (!feof(input) && (ifileBuf[0] == '\0'))
    {
        fgets(ifileBuf, max_length, input);
        remove_crlf(ifileBuf);
    }
    if (ifileBuf[0] == '\0')
        return 0;
    else
        return 1;
}

/* Class Library Function:
    print_indents() prints an indent for every level in the tree. */
void print_indents(int indent_level)
{
    for(int i = 0; i < indent_level; i++) {
        printf("   ");
    }
}

/* Print Individual Items in Subtree:
    Prints the name and count of the item in the subtree */

void print_item_node(item_node *i, int level)
{
    print_indents(level);
    printf("Name: %s Count: %d\n", i->name, i->count);
}

/* Print Items in Subtree:
    Calls print_indents() and print_item_node() - to print each
    item in the subtree. Recursively calls this function if the root
    has left children and prints them. Then recursively calls
    this function if the root has right children and prints them. */

void print_items(item_node *i, int level)
{
    print_indents(level);
    print_item_node(i, level);

    if(i->left != NULL)
    {
        print_items(i->left, level + 1);
    }

    if(i->right != NULL)
    {
        print_items(i->right, level + 1);
    }
}

/* Print Individual Tree Nodes:
    Prints the tree name it was passed and calls print_items()
    to print the subtree of each tree name. */

void print_tree_node(tree_name_node *t, int level)
{
    if(t)
    {
        printf("Tree: %s\n", t->treeName);
    }
    if(t->theTree)
    {
        print_items(t->theTree, level);
    }
    printf("\n");
}

/* Prints Tree in PREORDER:
    Calls print_indents() and print_tree_node() - to print each
    item in the tree. Recursively calls this function if the root
    has left children and prints them. Then recursively calls
    this function if the root has right children and prints them. */
void print_tree(tree_name_node *t, int level)
{
    print_indents(level);
    print_tree_node(t, level);

    if(t->left != NULL)
    {
        print_tree(t->left, level + 1);
    }

    if(t->right != NULL)
    {
        print_tree(t->right, level + 1);
    }

}

/* Add Tree Name:
    Takes in the root and tree name being added. First checks if the
    item being added is null. If it is, then it sets the new item. Else,
    string compare the root and item being added. If return value is
    greater than 0, then add to the left. If return value is less than
    0, then add to the right. Return the root. */

tree_name_node *add_tree_name(tree_name_node *t, tree_name_node *root)
{
    //base case
    if(root == NULL)
    {
        root = t;
        return root;
    }
    else
    {
        int testValue = strcmp(root->treeName, t->treeName);

        if(testValue > 0)
        {
            root->left = add_tree_name(t, root->left);
            root->left->parent = root;
            return root;

        }
        else
        {
            root->right = add_tree_name(t, root->right);
            root->right->parent = root;
            return root;
        }
    }

}

/* Get Tree Name:
    Takes in the input file, file buffer, and root. Reads the tree name
    being added to the tree and constructs a new tree node called treeName.
    Then it sends treeName to add_tree_name() to add it to the tree. */

tree_name_node *get_tree_names(FILE *ifp, char *ifileBuf, tree_name_node *root)
{
    char nameBuf[256];

    get_next_nonblank_line(ifp, ifileBuf, 255);
    sscanf(ifileBuf, "%s ", nameBuf);

    tree_name_node *treeName = new_tree_name_node(nameBuf);

    //add to the "top of the tree"
    treeName = add_tree_name(treeName, root);

    return treeName;
}

/* Output Tree Names:
    Takes in the output file to write to and the root to access its members.
    Recursively calls this function if there is a left child and if there is
    a right child. It will write the name of the tree node to the output file.  */

void output_tree_name(FILE *ofp, tree_name_node *root)
{
    if(root->left != NULL)
    {
        output_tree_name(ofp, root->left);
    }

    fprintf(ofp, "%s ", root->treeName);

    if(root->right != NULL)
    {
        output_tree_name(ofp, root->right);
    }
}


/* Find Tree Name:
    String compares the root name and category buffer. If the return
    value is greater than or less than 0, then it will recursively call
    this function with the left and right child of the root. If the
    return value is equal to 0, then the tree node was found. Returns
    that tree node.

    *search_in_name_node() function */

tree_name_node *find_tree_name(tree_name_node *root, char *categoryBuf)
{
    //base case: if tree is empty
    if(root == NULL)
    {
        return NULL;
    }
    else if(strcmp(root->treeName, categoryBuf) > 0)
    {
        return find_tree_name(root->left, categoryBuf);
    }
    else if(strcmp(root->treeName, categoryBuf) < 0)
    {
        return find_tree_name(root->right, categoryBuf);
    }
    //string is equal, so returns correct category
    else
    {
        return root;
    }
}

/* Find Item Node in Subtree:
    String compares the item name and type buffer. If the return
    value is greater than or less than 0, then it will recursively call
    this function with the left and right child of the item. If the
    return value is equal to 0, then the item node was found. Returns
    that item node.

    *search_for_name_node() function */

item_node *find_item(item_node *tree, char *typeBuf)
{
    //base case: if tree is empty
    if(tree == NULL)
    {
        return NULL;
    }
    else if(strcmp(tree->name, typeBuf) > 0)
    {
        return find_item(tree->left, typeBuf);
    }
    else if(strcmp(tree->name, typeBuf) < 0)
    {
        return find_item(tree->right, typeBuf);
    }
    //string is equal, so returns correct category
    else
    {
        return tree;
    }
}

/* Add Item Node to Subtree:
    Takes in the item root and node being added. First checks if the
    item being added is null. If it is, then it sets the new item. Else,
    string compare the item root and item being added. If return value is
    greater than 0, then add to the left. If return value is less than
    0, then add to the right. Also sets the item parent. Return the item.*/

item_node *add_item_node(item_node *new_node, item_node *item_root)
{
    if(item_root == NULL)
    {
        item_root = new_node;
        return item_root;
    }
    else
    {
        int testValue = strcmp(item_root->name, new_node->name);

        if(testValue > 0)
        {
            item_root->left = add_item_node(new_node, item_root->left);
            item_root->left->iparent = item_root;
            return item_root;

        }
        else
        {
            item_root->right = add_item_node(new_node, item_root->right);
            item_root->right->iparent = item_root;
            return item_root;
        }
    }
}

/* Get Items:
    Takes in the input file, file buffer, and root. Reads the item
    tparent, name, and count that is being added to the tree and
    constructs a new item node called i. It finds the correct tree
    to add the item to first, then sends it to constructor call.
    Then it sends currentTree's subtree to add_item_node() to add
    it to the subtree.*/

void get_tree_items(FILE *ifp, char *ifileBuf, tree_name_node *root)
{
    char categoryBuf[256];
    char typeBuf[256];
    int count;

    get_next_nonblank_line(ifp, ifileBuf, 255);
    sscanf(ifileBuf, "%s %s %d ", categoryBuf, typeBuf, &count);

    tree_name_node *temp_root = NULL;
    temp_root = root;

    //compares the input name with tree names
    tree_name_node *currentTree = find_tree_name(temp_root, categoryBuf);

    item_node *i = new_item_node(typeBuf, count, currentTree);

    currentTree->theTree = add_item_node(i, currentTree->theTree);
}

/* Output Total Items:
    Takes in the output file to write to and the item node to
    access its members. If it has a left child, it will
    recursively call this function. If it has a right child,
    it will recursively call this function. Writes the item's
    name to the output file. */

void output_total_items(FILE *ofp, item_node *i)
{
    if(i->left != NULL)
    {
        output_total_items(ofp, i->left);
    }

    fprintf(ofp, "%s ", i->name);

    if(i->right != NULL)
    {
        output_total_items(ofp, i->right);
    }
}

/* Output Each Category:
    Takes in the output file to write to and the root to
    access its members. It will print the category name
    and then call the output_total_items() to print the
    items contained within each category. */

void output_category_name(FILE *ofp, tree_name_node *t)
{
    fprintf(ofp, "===%s===\n", t->treeName);

    output_total_items(ofp, t->theTree);
}

/* Output Tree and Items:
    Takes in the output file and the root of the tree. If there
    is a left child, then it recursively calls this function. If
    there is a right child, then it recursively calls this function.
    Calls output_category_name() to write to the output file. */

void output_trees_and_items(FILE *ofp, tree_name_node *t)
{
    if(t->left != NULL)
    {
        output_trees_and_items(ofp, t->left);
    }

    output_category_name(ofp, t);
    fprintf(ofp, "\n");

    if(t->right != NULL)
    {
        output_trees_and_items(ofp, t->right);
    }
}

/* Search Tree:
    Takes in the output file, root, category buffer, and type buffer.
    First, it finds the correct subtree to search. If that tree is null,
    then the tree doesn't exist. If it does, then it will find the item
    in the tree and return it. If that item is null, then it doesn't
    exist. If it is not null, then write the count to the output file. */

void search_tree(FILE *ofp, tree_name_node *t, char *typeBuf, char *categoryBuf)
{
    tree_name_node *tree = find_tree_name(t, categoryBuf);

    if(tree == NULL)
    {
        fprintf(ofp, "%s does not exist\n", categoryBuf);
        return;
    }

    item_node *item = find_item(tree->theTree, typeBuf);

    if(item == NULL)
    {
        fprintf(ofp, "%s not found in %s\n", typeBuf, categoryBuf);
    }
    else
    {
        fprintf(ofp, "%d %s found in %s\n", item->count, item->name, tree->treeName);
    }

}

/* Find Item Before In Subtree:
    Takes in the subtree and type buffer. String compares the
    item name and the type buffer. Each time the function is
    called recursively, it adds one accounting for an item
    before. Returns the count. */

int find_item_before(item_node *tree, char *typeBuf)
{
    //base case: if tree is empty
    if(tree == NULL)
    {
        return 0;
    }
    else if(strcmp(tree->name, typeBuf) > 0)
    {
        return find_item_before(tree->left, typeBuf) + 1;
    }
    else if(strcmp(tree->name, typeBuf) < 0)
    {
        return find_item_before(tree->right, typeBuf) + 1;
    }
    //string is equal, so returns correct category
    else
    {
        return 0;
    }
}

/* Find Item Before In Main Tree:
    Takes in the main tree and category buffer. String compares the
    tree name and the category buffer. Each time the function is
    called recursively, it adds one accounting for an item
    before. Returns the count. */

int find_item_before_main(tree_name_node *tree, char *categoryBuf)
{
    //base case: if tree is empty
    if(tree == NULL)
    {
        return 0;
    }
    else if(strcmp(tree->treeName, categoryBuf) > 0)
    {
        return find_item_before_main(tree->left, categoryBuf) + 1;
    }
    else if(strcmp(tree->treeName, categoryBuf) < 0)
    {
        return find_item_before_main(tree->right, categoryBuf) + 1;
    }
    //string is equal, so returns correct category
    else
    {
        return 0;
    }
}

/* Output Item Before:
    Takes in the output file, root, category buffer, and type buffer.
    Finds the correct subtree and checks if the tree is null. If not,
    it calls find_item_before() that returns the count of the items
    before that item. It write is to the output file. */

void item_before(FILE *ofp, tree_name_node *t, char *categoryBuf, char *typeBuf)
{
    int count = 0;
    tree_name_node *tree = find_tree_name(t, categoryBuf);

    if(tree == NULL)
    {
        fprintf(ofp, "%s does not exist\n", categoryBuf);
        return;
    }

    //find items before in subtree
    count = find_item_before(tree->theTree, typeBuf);

    //find item before in main tree - plus 1 because account for the root
    count = count + find_item_before_main(t, categoryBuf) + 1;

    fprintf(ofp, "item before %s: %d\n", typeBuf, count);
}

/* Find Height Count:
    Takes in the item subtree root. First checks if the root
    is null. If it is, return -1. If not, it recursively calls
    this function for the left and right children, adding one
    for every item counted. Returns the count. */

int find_height_count(item_node *root)
{
    int count = 0;
    if(root == NULL)
    {
        return -1;
    }

    if(root->left)
    {
        count = find_height_count(root->left) + 1;
    }

    if(root->right)
    {
        count = find_height_count(root->right) + 1;
    }

    return count;
}

/* Output Height Balance:
    Takes in the output file, root, and category buffer. First checks
    if the correct subtree found is null. If not, then it calls a
    function called find_height_count() twice, one for the left side
    of the tree and another for the right side of the tree. It takes
    the absolute value of the difference between them. If the difference
    is equal to 0, then the tree is balanced. Writes information to the
    output file. */

void height_balance(FILE *ofp, tree_name_node *t, char *categoryBuf)
{
    int left = 0;
    int right = 0;
    tree_name_node *tree = find_tree_name(t, categoryBuf);

    if(tree == NULL)
    {
        fprintf(ofp, "%s does not exist\n", categoryBuf);
    }

    item_node *temp = NULL;
    temp = tree->theTree;

    left = find_height_count(temp->left);
    right = find_height_count(temp->right);

    int difference = abs(left - right);

    if(difference != 0)
    {
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", categoryBuf, left, right, difference);
    }
    else
    {
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", categoryBuf, left, right, difference);
    }
}

/* Find Total Count in Tree:
    Takes in an item from the subtree. First checks if the item
    is null. If not, then it calls find_total_count() for the left
    child and find_total_count() for the right child and adds them
    both to the count of the item. Returns the total count. */

int find_total_count(item_node *root)
{
    int total;
    //base case: if tree is empty
    if(root == NULL)
    {
        return 0;
    }

    total = find_total_count(root->left) + find_total_count(root->right) + root->count;

    return total;

}

/* Output Total Count:
    Takes in the output file, root, and category buffer. First checks
    if the correct tree is null. If not, then it calls find_total_count()
    to initialize total variable. Then it writes to the output file. */

void total_count(FILE *ofp, tree_name_node *t, char *categoryBuf)
{
    int total;
    tree_name_node *tree = find_tree_name(t, categoryBuf);

    if(tree == NULL)
    {
        fprintf(ofp, "%s does not exist\n", categoryBuf);
    }

    total = find_total_count(tree->theTree);

    fprintf(ofp, "%s count %d\n", categoryBuf, total);
}

/* Find the leftmost, right Descendant:
    Takes in the item that is being deleted right child. Finds
    the leftmost child from the item's right child and returns
    that item. */

item_node *find_leftmost_right_descendant(item_node *item)
{
    if(item->left != NULL)
    {
        return find_leftmost_right_descendant(item->left);
    }

    return item;
}

/* Find What Child Item Is:
    Takes in an item. First checks if the parent of item has one
    child. If it's right child is null, then return true (1). String
    compares the name of the node with the parents children. Returns
    true if it is the left child. */

int is_left(item_node *i)
{
    if(i->iparent->right == NULL)
    {
        return 1;
    }
    else if(i->iparent->left == NULL)
    {
        return 0;
    }
    else if(strcmp(i->iparent->left->name, i->name) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Delete Item:
    Takes in the output file, root, category buffer, and type buffer.
    First checks if the root is null. If not, then finds the item that
    is being removed from the tree. First case deletes a leaf, second
    case deleted a parent with one child, and third case deletes a
    parent with two children. */

void delete_item(FILE *ofp, item_node *item, char *categoryBuf, char *typeBuf)
{
    //leaf case:
    if(item->left == NULL && item->right == NULL)
    {
        fprintf(ofp, "%s deleted from %s\n", item->name, item->tparent->treeName);
        if(item->iparent)
        {
            if(item->iparent->left == NULL)
            {
                item->iparent->right = NULL;
            }
            if(item->iparent->right == NULL)
            {
                item->iparent->left = NULL;
            }
        }
        dispose_item_node(item);

    }
    //one child case:
    else if(item->left == NULL || item->right == NULL)
    {
        fprintf(ofp, "%s deleted from %s\n", item->name, item->tparent->treeName);

        if(item->left == NULL)
        {
            //checks if item is a right or left child
            if(is_left(item) == 1)
            {
                strcpy(item->name, item->right->name);
                item->count = item->right->count;
                item->iparent->left = item->right;
                item->right->iparent = NULL;
            }
            else
            {
                strcpy(item->name, item->right->name);
                item->count = item->right->count;
                item->iparent->right = item->right;
                item->right->iparent = NULL;
            }

            dispose_item_node(item);

        }
        if(item->right == NULL)
        {
            if(is_left(item) == 1)
            {
                strcpy(item->name, item->left->name);
                item->count = item->left->count;
                item->iparent->left = item->left;
                item->left->iparent = NULL;
            }
            else
            {
                strcpy(item->name, item->left->name);
                item->count = item->left->count;
                item->iparent->right = item->left;
                item->left->iparent = NULL;
            }

            dispose_item_node(item);

        }
    }
    //two child case:
    else
    {
        item_node *descendant = find_leftmost_right_descendant(item->right);

        fprintf(ofp, "%s deleted from %s\n", item->name, item->tparent->treeName);

        //copy descendant into item being removed
        strcpy(item->name, descendant->name);
        item->count = descendant->count;

        delete_item(ofp, descendant, categoryBuf, typeBuf);

    }
}

/* Delete Tree: All
    Takes in an item node. Deletes the items recursively in
    the tree from left to right, then finally deletes the root.*/

void delete_tree_all(item_node *item)
{
    if(item == NULL)
        return;

    if(item->left != NULL)
    {
        delete_tree_all(item->left);
    }

    if(item->right != NULL)
    {
        delete_tree_all(item->right);
    }

    //dispose of the item
    if(item->iparent)
    {
        if(item->iparent->left == NULL)
        {
            item->iparent->right = NULL;
        }
        if(item->iparent->right == NULL)
        {
            item->iparent->left = NULL;
        }
    }
    dispose_item_node(item);

}

/* Delete Tree:
    Takes in the output file, root, and category buffer. First
    checks if the root is null. If not, it calls the delete_tree
    _all() function that will delete an entire tree. */

void delete_tree(FILE *ofp, tree_name_node *t, char *categoryBuf)
{
    tree_name_node *tree = find_tree_name(t, categoryBuf);

    if(tree == NULL)
    {
        fprintf(ofp, "%s does not exist\n", categoryBuf);
    }

    delete_tree_all(tree->theTree);
    fprintf(ofp, "%s deleted\n", tree->treeName);

    tree->theTree = NULL;
}

/* Processing Commands:
    Takes in the file buffer, output file, command name, and root. It
    will string compare the command name to each possible command type.
    In each comparison, it will read the input file and set buffers. Then
    it will call the function pertaining to each command. */

void process_command(char *ifileBuf, FILE *ofp, char *commandName, tree_name_node *t)
{
    char categoryBuf[256];
    char typeBuf[256];
    char temp[256];

    if(strcmp(commandName, "search") == 0)
    {
        sscanf(ifileBuf, "%s %s %s ", temp, categoryBuf, typeBuf);
        search_tree(ofp, t, typeBuf, categoryBuf);
    }
    if(strcmp(commandName, "item_before") == 0)
    {
        sscanf(ifileBuf, "%s %s %s ", temp, categoryBuf, typeBuf);
        item_before(ofp, t, categoryBuf, typeBuf);
    }
    if(strcmp(commandName, "height_balance") == 0)
    {
        sscanf(ifileBuf, "%s %s ", temp, categoryBuf);
        height_balance(ofp, t, categoryBuf);
    }
    if(strcmp(commandName, "count") == 0)
    {
        sscanf(ifileBuf, "%s %s ", temp, categoryBuf);
        total_count(ofp, t, categoryBuf);
    }
    if(strcmp(commandName, "delete") == 0)
    {
        sscanf(ifileBuf, "%s %s %s", temp, categoryBuf, typeBuf);

        tree_name_node *tree = find_tree_name(t, categoryBuf);
        if(tree == NULL)
        {
            fprintf(ofp, "%s does not exist\n", categoryBuf);
        }
        item_node *item = find_item(tree->theTree, typeBuf);

        delete_item(ofp, item, categoryBuf, typeBuf);

    }
    if(strcmp(commandName, "delete_tree") == 0)
    {
        sscanf(ifileBuf, "%s %s ", temp, categoryBuf);
        delete_tree(ofp, t, categoryBuf);
    }
}

/* Delete Whole Tree:
    Takes in the root of the tree. Recursively calls this function
    if there is a left or right child in the main tree. Disposes
    each main tree. */

void delete_every_tree(tree_name_node *t)
{
    if(t == NULL)
        return;

    if(t->left)
    {
        delete_every_tree(t->left);
    }
    if(t->right)
    {
        delete_every_tree(t->right);
    }

    delete_tree_all(t->theTree);
    if(t->parent)
    {
        if(t->parent->left == NULL)
        {
            t->parent->right = NULL;
        }
        if(t->parent->right == NULL)
        {
            t->parent->left = NULL;
        }
    }
    dispose_tree_name(t);
}

int main()
{
    //atexit(report_mem_leak);

    FILE *ifp = fopen("cop3502-as4-input.txt", "r");
    FILE *ofp = fopen("cop3502-as4-output-grimes-katelyn", "w");

    char ifileBuf[256];
    char commandName[256];
    int numTree, numItems, numCommands;
    tree_name_node *root = NULL;

    bool myeof = false;

    while(!feof(ifp) && myeof == false)
    {
        get_next_nonblank_line(ifp, ifileBuf, 255);
        sscanf(ifileBuf, "%d %d %d ", &numTree, &numItems, &numCommands);

        for(int i = 0; i < numTree; i++)
        {
            root = get_tree_names(ifp, ifileBuf, root);
        }

        output_tree_name(ofp, root);
        fprintf(ofp, "\n");

        for(int j = 0; j < numItems; j++)
        {
            get_tree_items(ifp, ifileBuf, root);
        }

        output_trees_and_items(ofp, root);

        print_tree(root, 0);

        fprintf(ofp, "=====Processing Commands=====\n");

        for(int k = 0; k < numCommands; k++)
        {
            get_next_nonblank_line(ifp, ifileBuf, 255);
            sscanf(ifileBuf, "%s ", commandName);

            process_command(ifileBuf, ofp, commandName, root);
        }

        myeof = true;
    }

    print_tree(root, 0);

    delete_every_tree(root);

    fclose(ifp);
    fclose(ofp);

    return 0;
}
