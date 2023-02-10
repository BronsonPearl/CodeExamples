This is an automated woodcutting bot for the online browser based game RuneScape.
This bot is trained to only cut willow trees in game. In it's current state there are some false positives depending on camera angle and lighting in game.

This game holds a spot in my heart as it was one of my childhood favourites before i was introduced into world of warcraft.
now that im an adult i felt i dont have the time to grind away on skills in game so i thought i would use this as an opportunity to explore OpenCV and Machine Learning
to create a bot for the game.

** disclaimer botting in RuneScape is against the rules and will get you banned.
This was created solely as a personal project to learn and test compitency of the subject material **

This project uses OpenCV v 3.4.11
It will work with later versions for screencapturing and things like template matching however the object detection is a trained model using the HAAR cascade classifier from OpenCV 3.4.11

The Bot is written in python and contains multi-threading to increase performance.
if all of this was written to use one thread it would operate at around 6 fps.  We currently run at around 240fps.
written with Python 3.10

the positive image input, negative image input, and the cascade training output have been omitted from this repository to save space and time uploading as there is a file limit.

this is version 0.9 of this bot.  In it's current state it has just stub routines for the pyautogui mouse utility

Controls for the bot such as mouse movement and clicking are in the bot.py

Cascadeutils.py is just a method to create the negative text file for our negative data input images. We use this file for training the cascade classifier  

detection.py has the requied methods for implementing the object detection for the program

edgefilter.py contains a method to implement a canny edge filter

hsvfitler.py contains a method to implement a hsv colour filter

eyeballs.py contains all the methods to do image processing for different filters such as canny edge and hsv filtering. 
this class also contains the information to draw the rectangles around the object and return points of interest for our mouse to click on.

windowcapture.py is self titled. This contains the methods to capture the image of the game window.  We have specified that this will only capture the Old School RuneScape 
game client.  Then it will store the image as bitmap. Then convert it to a np array for OpenCV to use. Lastly it will crop the game window to only show what's on the game screen
no pesky windowed mode bars

main.py contains instructions on how to use the cascade classifier and how to use the executables to train it.  It runs in a forever loop until canceled with an input of q.
