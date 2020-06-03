from picamera import PiCamera
from time import sleep

camera = PiCamera()

camera.start_preview(fullscreen=False,window=(100,200,300,400))
sleep(5)
camera.capture('/home/pi/Desktop/cynzim.jpg')
camera.stop_preview()