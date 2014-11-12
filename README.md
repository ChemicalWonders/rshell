### rshell 

### Created for cs 100 at uc riverside

This is a basic shell written in C++ to help us understand how the shell works.
If there are any concerns with the shell, please let me know.

### Licensing
I am licensed under the MIT License. If you have any use for my code, then I just ask that you please credit me for what you do. If you are found to copy my code, and claim it as yourself, your school might take action against you. I will not be found liable for anything that happens.

###RSHELL: Bugs

* connectors (|| and &&) do not work at all
* when you put a comment in the middle of the command, it won't show.
* up arrow and down arrow does not show previous commands
* echo with quotes don't work, my program only parses whitespace
* changing directories does not work because it is a bash command
* typing 'what' in the the terminal make it do something weird. not sure what's going on
* sometimes the exit command needs to be entered a few times before exiting.
* tabbing does not autocomplete the file or command name.

###ls: Bugs

* When you try a different path, -R does not work.
* Spacing is different from original shell
* Colors are weird, but background color is not working
