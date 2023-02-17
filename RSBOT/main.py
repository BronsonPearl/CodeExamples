import cv2
import numpy as np
import pyautogui
from windowcapture import WindowCapture
from eyeballs import Eyes
from time import time, sleep
from threading import Thread
from detection import objDetection
from bot import RSBot, BotState


windowcap = WindowCapture('Old School RuneScape')
detect = objDetection('Y:\\opencv projects\\ScapeEyesML\\cascadeOutput\\cascade.xml')
compeyes = Eyes(None)

bot = RSBot((windowcap.offset_x, windowcap.offset_y))

clock = time()

bot_in_action = False

windowcap.start()
detect.start()
bot.start()

while True:

    clock = time()

    # if we don't have a screenshot yet, don't run the code below this point yet
    if windowcap.screenshot is None:
        continue

    # give detector the current screenshot to search for objects in
    detect.update(windowcap.screenshot)

    # update the bot with the data it needs right now
    if bot.state == BotState.INITIALIZING:
        # while bot is waiting to start, go ahead and start giving it some targets to work
        # on right away when it does start
        targets = compeyes.get_click_points(detect.rectangles)
        bot.update_targets(targets)
    elif bot.state == BotState.SEARCHING:
        # when searching for something to click on next, the bot needs to know what the click
        # points are for the current detection results. it also needs an updated screenshot
        # to verify the hover tooltip once it has moved the mouse to that position
        targets = compeyes.get_click_points(detect.rectangles)
        bot.update_targets(targets)
        bot.update_screenshot(windowcap.screenshot)
    elif bot.state == BotState.MOVING:
        # when moving, we need fresh screenshots to determine when we've stopped moving
        bot.update_screenshot(windowcap.screenshot)
    elif bot.state == BotState.CUTTING:
        # nothing is needed while we wait for the cutting to finish
        pass

    #if DEBUG:
    # draw the detection results onto the original image
    detection_image = compeyes.draw_rectangles(windowcap.screenshot, detect.rectangles)
    # display the images
    cv2.imshow('Matches', detection_image)

    # press 'q' with the output window focused to exit.
    # waits 1 ms every loop to process key presses
    key = cv2.waitKey(1)
    if key == ord('q'):
        windowcap.stop()
        detect.stop()
        bot.stop()
        cv2.destroyAllWindows()
        break
    elif key == ord('f'):
        cv2.imwrite('ScapeEyesML\positiveInput\{}.jpg'.format(clock), windowcap.screenshot)
    elif key == ord('d'):
        cv2.imwrite('ScapeEyesML\\negativeInput\{}.jpg'.format(clock), windowcap.screenshot)

'''
make neg text file by cd into directory with project andrunning python in terminal and giving these commands
from cascadeutils import generate_negative_description_file
generate_negative_description_file()
exit()

generate positive text file by using 
Y:\\opencv-3.4.11\\opencv\\build\\x64\\vc15\\bin\\opencv_annotation.exe --annotations=pos.txt --images=positiveInput/
Press 'c' to confirm.
Or 'd' to undo the previous confirmation.
When done, click 'n' to move to the next image.
Press 'esc' to exit.
Will exit automatically when you've annotated all of the images

create pos.vec
Y:\\opencv-3.4.11\\opencv\\build\\x64\\vc15\\bin\\opencv_createsamples.exe -info pos.txt -w 24 -h 24 -num 1000 -vec pos.vec

train with:
Y:\\opencv-3.4.11\\opencv\\build\\x64\\vc15\\bin\\opencv_traincascade.exe -data cascadeOutput/ -vec pos.vec -bg neg.txt -w 24 -h 24 -numPos 250 -numNeg 1000 -numStages 8 -minHitRate 0.95 -precalcValBufSize 8192

*** w and h must match from vec file ***

-precalcValBufSize <precalculated_vals_buffer_size_in_Mb> : Size of buffer for precalculated feature values (in Mb). The more memory you assign the faster the training process 
however keep in mind that -precalcValBufSize and -precalcIdxBufSize combined should not exceed you available system memory.

-precalcIdxBufSize <precalculated_idxs_buffer_size_in_Mb> : Size of buffer for precalculated feature indices (in Mb). The more memory you assign the faster the training process however
 keep in mind that -precalcValBufSize and -precalcIdxBufSize combined should not exceed you available system memory.

-acceptanceRatioBreakValue <break_value> : This argument is used to determine how precise your model should keep learning and when to stop.
 A good guideline is to train not further than 10e-5, to ensure the model does not overtrain on your training data. By default this value is set to -1 to disable this feature.

source:
https://docs.opencv.org/4.2.0/dc/d88/tutorial_traincascade.html
'''
