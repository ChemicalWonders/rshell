### rshell 

### Created for cs 100 at uc riverside

This is a basic shell written in C++ to help us understand how the shell works.
If there are any concerns with the shell, please let me know.

### Licensing
I am licensed under the MIT License. If you have any use for my code, then I just ask that you please credit me for what you do. If you are found to copy my code, and claim it as yourself, your school might take action against you. I will not be found liable for anything that happens.

###RSHELL: Bugs

* connectors (|| and &&) do not work at all
* when you put a comment in the middle of the command, it will deletes everything from that comment.
* up arrow and down arrow does not show previous commands
* echo with quotes work, but the include the quotes.
* changing directories does not work because it is a built in bash
* tabbing does not autocomplete the file or command name
* going up and down with arrow keys don't work 

### ls: Bugs

* When you try a different path, -R does not work.
* Spacing is different from original shell
* Colors are weird, but background color is not working
* When you put option flags in certain areas, it will work, but does not give desired output
* path name is output on the top when you use a different directory.


###RSHELL with piping: Bugs
* Sometimes you need to press enter once to regain the shell, flushing problem
* Spacing is not always correct when the first bug occurs
* Some built in commands don't always work, i.e. history
* when you enter |, < , >, or <<, you need to make sure that each of them is entered a space inbetween
other wise the program thinks that it's part of the last argument.
* some commands will stall on the hammer server, if you try ps aux | grep kchan039, it will work on my copy of linux, but it stalls on hammer.
* when you try running commands, sometimes, an extra enter is required for the commandline to print again. I think it's a problem with my program.
* when you run rshell, it will output a file error with <.


###RSHELL with signals: Bugs
* When you typed in ^C, you have to press enter to create a new prompt
* idk
