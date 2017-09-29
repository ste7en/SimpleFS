# SimpleFS
A simple FileSystem based on UNIX FS syntax written in C as final project of Algorithms course of Politecnico di Milano.

The project is a mono module and its source code is in SimpleFS directory with its instructions.

Here are the commands:

Reminder: paths have Unix-like syntax, like /dir/dir1/dir2/file.

• create <file-path>

  creates a file in specified directory
  
• create_dir <dir-path>

  creates a directory in specified directory

• write <file-path> "insert text here"

  writes a string in a existing file; the string is between two quotation marks

• read <file-path>

  reads a file whose string is precedeed by 'contenuto '

• find <name>
  
  find files and directories in the filesystem

• delete <file-path or dir-path>
  
  deletes a file or a directory without sons

• delete_r <dir-path>
  
  recoursive delete of a directory

• exit
  
  exit from the program
