import picamera 
from picamera.array import PiRGBArray
import imutils
from time import sleep
import cv2
import numpy as np
#import detector
from gpiozero import LED,MotionSensor

ms  = MotionSensor(15, pull_up=True,queue_len=5)

avg = None

def RedOff():
    led = LED(17)
    led.off()
    print("Fuck Nathan")

def RedOn():
    led = LED(17)
    led.on()
    sleep(10)
    print("Fuck Nathan")
    
    
def main():
    sleep(5)
    ms.wait_for_motion()
    global avg 
    with picamera.PiCamera() as camera:
        camera.resolution = (640,480)
        camera.framerate = 16
        rawCapture = np.empty((640,480,3),dtype=np.uint8)
        camera.start_preview()
        rawCapture = np.empty((640,480,3),dtype=np.uint8)
        camera.capture(rawCapture,format="bgr")
        #print(f)
        #frame = f.array
        isPerson = False
        #frame = imutils.resize(f, width=500)
        f =rawCapture.reshape((480,640,3))
        gray = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (21, 21), 0)

        # if the average frame is None, initialize it
        if avg is None:
            print("[INFO] starting background model...")
            avg = gray.copy().astype("float")
        
        
        # accumulate the weighted average between the current frame and
        # previous frames, then compute the difference between the current
        # frame and running average
        cv2.accumulateWeighted(gray, avg, 0.5)
        frameDelta = cv2.absdiff(gray, cv2.convertScaleAbs(avg))
        
        # threshold the delta image, dilate the thresholded image to fill
        # in holes, then find contours on thresholded image
        thresh = cv2.threshold(frameDelta, 5, 255,
            cv2.THRESH_BINARY)[1]
        thresh = cv2.dilate(thresh, None, iterations=2)
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)  # loop over the contours
        for c in cnts:
            # if the contour is too small, ignore it
            if cv2.contourArea(c) > 5000:
                isPerson =True
                print("Occupied")
            else:
                print("unOccupied")
        
        camera.stop_preview()

        

if __name__ == "__main__":
    while True:
        main()
    

        


