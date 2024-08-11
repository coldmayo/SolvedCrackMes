## Basic Usage

- r2 -d exec
	- Opens Radare2 for file named "exec" in debug mode
- aaaa
	- analyze executable
- pdf@function
	- shows disassembly at funtion, for example: pdf@main opens main
- db flag
	- Sets breakpoint at either address or function
- db - flag
	- Deletes breakpoint at the flag
- db
	- shows all breakpoints
- dc
	- run executable
- dr rip=flag
	- I use this command after a breakpoint is reached to skip to another address (or flag in this case). Once this is run, use the command dc to run the executable again with this implemented
- afl
	- show functions in executables

For more info, take a look at <a href="https://book.rada.re/">The Offical Radare2 Book</a>
