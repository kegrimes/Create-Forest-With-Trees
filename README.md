# **Create-Forest-With-Trees**
This project is from COP3502, Computer Science 1

### Overview:
A forest, or a tree of trees, maintains records of specific types of "small monsters": animals, birds, fish, and fruits. Given the input file _"cop3502-as4-input.txt"_, the forest will be built first and then a list of commands following its construction will execute. An output file containing the results will be created in a testfile called _"cop3502-as4-output-grimes-katelyn.txt"_.  
  
*Note: the first line inside the input file is formatted  `%d %d %d`. The first integer represents the number of "small monster" types, or trees. The second integer represents the total number of items being added to the forest. Finally, the third integer represents the number of commands to be executed.  

### List of Commands for Tree:
1. `search<tree><item>` = searches for an _item_ in a specific _tree_
2. `item_before<tree><item>` = counts the number of items before _item_ in a specific _tree_
3. `height_balance<tree>` = prints out the heights of a tree's left and right subtrees, their difference, and whether or not the _tree_ is balanced (a tree is balanced if the height of its left and right subtrees differ >= 1)
4. `count<tree>` = prints the total number of items in a specific _tree_
5. `delete<tree><item>` = deletes an _item_ in a specific _tree_
6. `delete_tree<tree>` = deletes a _tree_

### GCC Compiler Commands:
```
gcc cop3502-as4-grimes-katelyn.c -o out \\compile
./out cop3502-as4-input.txt \\run
```

### Output:
In the terminal, the forest after it is constructed will be printed to the screen along with the updated forest after the list of commands are executed from the input file.      
The output file will be called _"cop3502-as4-output-grimes-katelyn.txt"_ and will print a smaller version of the forest and the results of the list of commands from the input file. 
