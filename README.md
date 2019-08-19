# DesktopBricks
One of my first projects: an attempt to make bricks appear on top of the UI in Windows. The code is over 10 years old, and was designed for an older version of windows. (Sort of works on Windows 10)

![Screenshot on Windows 10](https://raw.githubusercontent.com/michaelplzno/DesktopBricks/master/DesktopBricks.PNG)

Hit the ESC key to terminate the program, it is designed to appear on top of the Windows UI, so it has no window frame or X in the corner to close it.

When the program is working correctly it will spawn a bunch of little bricks on the desktop you can play with virtually by dragging them and connecting them. Also it will save the layout of the bricks into a gamestate so if you run it again your setup will be reloaded. 

Known Windows 10 Issues: 
1) The brick sounds have to be in the right directory to play otherwise you get a ding instead.
2) The mouse code seems to have an offset that makes your click register in the wrong place. (This may be due to the window scaling that happens on high res monitors.)
3) The game does not create a large enough virtual window for a large 4k monitor. (Moniters were a lot smaller when this code was written.)

This is intended to give a bit of a sense of how I coded back in the day. If you would like to update the code to work better for Windows 10 and update the projects to work in the current visual studio, just let me know!

This project is now officially Open Source under GPL V2.0, It is my hope that we can build this up into something bigger, maybe even an open source engine for games played on top of the operating system UI. 

"Let this be a kiss to the whole world!"

