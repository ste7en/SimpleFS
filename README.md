# SimpleFS
A simple hierarchical file system based on UNIX FS syntax implemented in `C99` with only the standard library (`libc`), as final project of API – Algorithms course of **`Politecnico di Milano`**. 

The project is a mono module and its source code is in SimpleFS directory with its instructions. You can use .in files in /SimpleFS/test directory with cat command from your terminal to test the program with public inputs and compare the results with .out files.

Here are the commands:
(Reminder: paths have Unix-like syntax, like /dir/dir1/dir2/file.)

• `create <file-path>`

  creates a file in specified directory
  
• `create_dir <dir-path>`

  creates a directory in specified directory

• `write <file-path> "insert text here"`

  writes a string in a existing file; the string is between two quotation marks

• `read <file-path>`

  reads a file whose string is precedeed by 'contenuto '

• `find <name>`
  
  find files and directories in the filesystem

• `delete <file-path or dir-path>`
  
  deletes a file or a directory without sons

• `delete_r <dir-path>`
  
  recoursive delete of a directory

• `exit`
  
  exit from the program
