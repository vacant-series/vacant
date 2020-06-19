import picamera 
from picamera.array import PiRGBArray
import imutils
from time import sleep
import cv2
import warnings
import numpy as np
from gpiozero import LED,MotionSensor, DigitalOutputDevice, DigitalInputDevice
from threading import Thread,Lock
warnings.filterwarnings("ignore")

class Vacant():
    def __init__(self):
        self.ms  = MotionSensor(15, pull_up=True,queue_len=20)
        self.avg = None
        self.camera = picamera.PiCamera()
        self.camera.resolution = (640,480)
        self.camera.framerate = 16
        self.isPerson = False
        self.camera_lock =Lock()
        self.rawCapture = PiRGBArray(self.camera, size=(640,480))
        self.timer = DigitalInputDevice(14)
        #self.time =  time()
#        self.capture = PiRGBArray(self.camera, size=(640,480))
#         self.averager_thread = Thread(target=self.room_averager, args=())
#         self.averager_thread.start()
#         self.detector_thread = Thread(target=self.human_detector(), args=())
#         self.detector_thread.start()
     
    def human_detector(self,gray):
        
        #sleep(5)
#         with self.camera_lock:
#         self.camera.start_preview()
#         self.camera.capture(self.rawCapture, format="bgr")
#         frame = self.rawCapture.array
#         # resize the frame, convert it to grayscale, and blur it
#         frame = imutils.resize(frame, width=500)
#         gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#         gray = cv2.GaussianBlur(gray, (21, 21), 0)
        
        # accumulate the weighted average between the current frame and
        # previous frames, then compute the difference between the current
        # frame and running average
        frameDelta = cv2.absdiff(gray, cv2.convertScaleAbs(self.avg))
        
        # threshold the delta image, dilate the thresholded image to fill
        # in holes, then find contours on thresholded image
        thresh = cv2.threshold(frameDelta, 20, 255,
            cv2.THRESH_BINARY)[1]
        thresh = cv2.dilate(thresh, None, iterations=2)
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)  # loop over the contours
        num_of_min_cnts = 0
        
        for c in cnts:
            # if the contour is too small, ignore it
#             print(cv2.contourArea(c))
            if cv2.contourArea(c) < 5000:
                num_of_min_cnts+=1
                continue
            self.hold_clock()
        self.rawCapture.truncate(0)
        self.camera.stop_preview()
        #self.camera.close()                
            
    def room_averager(self):
        # initialize the camera and grab a reference to the raw camera capture
        print("[INFO] warming up...")
        sleep(2.5)
    
#         with self.camera_lock:

        # capture frames from the camera
        for f in self.camera.capture_continuous(self.rawCapture, format="bgr", use_video_port=True):
            # grab the raw NumPy array representing the image and initialize
            # the timestamp and occupied/unoccupied text
            self.reset_clock()
            frame = f.array
            # resize the frame, convert it to grayscale, and blur it
            frame = imutils.resize(frame, width=500)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            gray = cv2.GaussianBlur(gray, (21, 21), 0)

            # if the average frame is None, initialize it
            if self.avg is None:
                print("[INFO] starting background model...")
                self.avg = gray.copy().astype("float")
                self.rawCapture.truncate(0)
                continue
            # accumulate the weighted average between the current frame and
            # previous frames
            cv2.accumulateWeighted(gray, self.avg, 0.5)
            # clear the stream in preparation for the next frame
            self.rawCapture.truncate(0)
            #self.camera.close()
            #print(self.ms.motion_detected)
            if self.ms.motion_detected:
                self.human_detector(gray)
                
    def reset_clock(self):
        self.timer.close()
        self.timer = DigitalInputDevice(14)
        
    
    def hold_clock(self):
        self.timer.close()
        self.timer = DigitalOutputDevice(14)
    


def main():
    vacant = Vacant()
    while True:
        vacant.room_averager()
    



if __name__ == "__main__":
    main()
    

        


