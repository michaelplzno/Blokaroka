# Current Build v0.2.02 Downloadable Here: [Blokaroka](https://blokaroka.com)
One of my first projects: an attempt to make bricks appear on top of the UI in Windows. This will help you pass the time while your program compiles in the background, or if you are at work and need to keep your brain active during a dull stretch. Playing with blocks improves creativity and dynamic thinking, so having this app running might even make you more effective at work, or help you cut your stress!

If the project catches on, hopefully we can work together to make it a popular app.

![Screenshot on Windows 10](https://raw.githubusercontent.com/michaelplzno/DesktopBricks/master/Blokaroka.PNG)

Hit the ESC key to terminate the program, or right click the icon in the start bar and hit CLOSE, or now there is a System Tray icon that when right clicked creates a menu that allows you to change your blok colors or exit. The app is designed to appear on top of the Windows UI, so it has no window frame or X in the corner to close it. 

When the program is working correctly it will spawn a bunch of little bricks on the desktop you can play with virtually by dragging them and connecting them. Also it will save the layout of the bricks into a gamestate so if you run it again your setup will be reloaded. 

Check the issues section for a list of current known issues, and let me know if you would like to fix them. Thanks to our first contributor the game compiles on Visual Studio 2019, but there are more issues on the list! If you would like to contribute, just let me know!

This project is now officially Open Source under GPL V2.0, It is my hope that we can build this up into something bigger, maybe even an open source engine for games played on top of the operating system UI. 

"Let this be a kiss to the whole world!"

# Overhaul 1/9/2025 v0.2.0 -
1) Build now works with version 3.1.0 of Box2D
2) Add cmake files, you may need to install cmake to build but you can now build with visual studio code.
3) Rename icons that were spelled wrong.
4) add .vscode files for launching and building.
5) add resource.rc which was missing.
6) Fix small issues with turning gravity on and off.
7) Add debug drawing of physics and option to enable it in menu.
8) Fix small bug with how bottom bloks are attached.

# 1/18/2025 v0.2.02 - 
Small fixes to brick attaching.
