# omx-control
omx-control is a very simple control application for omxplayer on the Raspberry Pi over ssh, specifically for youtube-dl supported video links.

### Rationale
This applicaiton is basically a way for me to play a Youtube or Twitch video on the Raspberry Pi remotely without using a keyboard/mouse, or having to manually ssh in and play the video. It will login to the Pi, pass a URL (which uses youtube-dl to decode the URL), and call omxplayer to play the video.

### Dependencies
Project requires the following dependancies to compile:
- cmake
- Qt 5.6 or higher development libraries
- Your distribution's build tools and C++ compiler
Under Ubuntu, you can install them using the following command:
```sh
$ sudo apt-get install cmake build-essential qt5-default
```
The project also requires the following to use on the controlling machine:
- youtube-dl (In order to decode the Youtube or Twitch URL)
- The same Qt libraries used to compile the program
This application was created with Linux in mind, I haven't tested (and it probably doesn't work) under Windows.

### How to compile
Use the build script:
```sh
$ ./build.sh
```
The script basically calls cmake, and then make. It makes a small wrapper script to load omx-control as well. The binary and script will be created in the build folder.

### Disclaimer
I made this project as a hobby really. If I had made it with a serious development mind, I would have used the libssh library instead of the actual ssh executable, and wouldn't print out the console output to the main application. I just wanted to make something quick and dirty to remote control omx via ssh. As usual, use at your own risk. Feel free to fork and modify.
