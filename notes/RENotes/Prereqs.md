# Prerequisites

To actually start Reverse Engineering in Linux, I would suggest installing Radare2 and Ghidra in addition to learning the basics of x86 assembly.

## Installation

### Radare2

Should be able to be installed from your prefered package manager:

```bash
# For Debian based systems
$ sudo apt-get install radare2

# For Arch based systems
$ sudo pacman -S radare2
```

This should be all that is needed to install Radare2

### Ghidra

Before doing anything else, make sure you have Java installed.

On a Debian and Arch based distrobutions:
1. Go <a href="https://github.com/NationalSecurityAgency/ghidra/releases">here</a> and download the most recent release.
2. Move the zip file to the /opt/ directory
3. Unzip the file and cd into the directory
4. run Ghidra with ```./ghidraRun```

Additionally, you could make a desktop shortcut by creating a file called Ghidra.desktop and putting the following inside of it:

```bash
[Desktop Entry]
Type=Application
Terminal=false
Icon=/home/username/.ghidra/Ghidra_logo.png
Exec=/opt/ghidra/ghidraRun
Name=Ghidra
```
Change Icon value to wherever the icon on your machine isand save this to /home/username/.local/share/applications

## Learn Basic x86 Assembly

- Take a look at [[The CPU]]
- Crash Course: <a href="https://www.youtube.com/watch?v=75gBFiFtAb8">here</a>
- Maybe reference: [[Registers]] or [[SimpleAssemblyExamples]]
