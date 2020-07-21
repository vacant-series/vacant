import picamera
from picamera.array import PiRGBArray
import imutils
from time import sleep
import time
import cv2
import warnings
from gpiozero import MotionSensor
import  RPi.GPIO as GPIO
warnings.filterwarnings("ignore")
GPIO.setmode(GPIO.BCM)

class Vacant():
    def __init__(self):
        #self.ms  = MotionSensor(15, pull_up=True,queue_len=20)
        self.avg = None
        self.camera = picamera.PiCamera()
        self.camera.resolution = (640,480)
        self.camera.framerate = 16
        self.rawCapture = PiRGBArray(self.camera, size=(640,480))
        self.timerpin=23
        GPIO.setup(self.timerpin,GPIO.IN)
        self.detection_speed = 10
        self.last_motion_timestamp= None#Time stamp from time.time()

    def human_detector(self,gray):
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
            if cv2.contourArea(c) < 20:
                num_of_min_cnts+=1
                continue
            self.hold_clock()
            self.last_motion_timestamp= time.time()
            print("[INFO] motion detected")

    def room_averager(self):
        # initialize the camera and grab a reference to the raw camera capture
        print("[INFO] warming up...")
        sleep(2.5)
        count = 0
        # capture frames from the camera
        for f in self.camera.capture_continuous(self.rawCapture, format="bgr", use_video_port=True):
            # grab the raw NumPy array representing the image and initialize
            # the timestamp and occupied/unoccupied text
            # Only reset if it has been a few seconds since last motion detection.
            if (self.last_motion_timestamp is None) or (time.time() > self.last_motion_timestamp+2):
                self.reset_clock()
            frame = f.array
            # resize the frame, convert it to grayscale, and blur it
            frame = imutils.resize(frame, width=500)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            gray = cv2.GaussianBlur(gray, (21, 21), 0)
            count +=1

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
            if count == self.detection_speed:
                count = 0
                self.human_detector(gray)

    def reset_clock(self):
        GPIO.setup(self.timerpin,GPIO.IN)

    def hold_clock(self):
        GPIO.setup(self.timerpin,GPIO.OUT)

def main():
    vacant = Vacant()
    vacant.room_averager()

if __name__ == "__main__":
    main()
