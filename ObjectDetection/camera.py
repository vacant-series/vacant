import picamera 
from picamera.array import PiRGBArray
import imutils
from time import sleep
import cv2
import numpy as np
from gpiozero import LED,MotionSensor
from threading import Thread, Lock

ms  = MotionSensor(15, pull_up=True,queue_len=5)
camera_lock = Lock()
avg = None
    
    
def detect_human():
    global avg 
    with picamera.PiCamera() as camera:
        camera.resolution = (640,480)
        camera.framerate = 16
        rawCapture = np.empty((640,480,3),dtype=np.uint8)
        camera.start_preview()
        rawCapture = np.empty((640,480,3),dtype=np.uint8)
        camera.capture(rawCapture,format="bgr")

        isPerson = False
        #frame = imutils.resize(f, width=500)
        f =rawCapture.reshape((480,640,3))
        gray = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (21, 21), 0)

        
        # accumulate the weighted average between the current frame and
        # previous frames, then compute the difference between the current
        # frame and running average
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

        
def room_averager():
    global avg
    # initialize the camera and grab a reference to the raw camera capture
    with picamera.PiCamera() as camera:
        camera.resolution = (640,480)
        camera.framerate = 16
        rawCapture = PiRGBArray(camera, size=(640,480))

        print("[INFO] warming up...")
        time.sleep(2.5)

        # capture frames from the camera
        for f in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
            # grab the raw NumPy array representing the image and initialize
            # the timestamp and occupied/unoccupied text
            frame = f.array
            # resize the frame, convert it to grayscale, and blur it
            frame = imutils.resize(frame, width=500)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            gray = cv2.GaussianBlur(gray, (21, 21), 0)

            # if the average frame is None, initialize it
            if avg is None:
                print("[INFO] starting background model...")
                avg = gray.copy().astype("float")
                rawCapture.truncate(0)
                continue
            # accumulate the weighted average between the current frame and
            # previous frames
            cv2.accumulateWeighted(gray, avg, 0.5)
            # clear the stream in preparation for the next frame
            rawCapture.truncate(0)
            ms.when_motion(self.human_detector())


def main():
    room_averager()



if __name__ == "__main__":
    main()
    

        


