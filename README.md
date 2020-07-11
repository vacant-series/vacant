# vacant
Check if a room is unoccupied.
Our JSON configuration filelet’s take a look at our conf.json  file:

vacant with the Raspberry Pi, Python, and OpenCV
{
	"delta_thresh": 20,
	"resolution": [
		640,
		480
	],
	"detection_speed":10,
	"fps": 16,
	"min_area": 500,
	"timer_pin":23
}
This JSON configuration file stores a bunch of important variables. Let’s look at each of them:
delta_thresh : The minisum absolute value difference between our current frame and averaged frame for a given pixel to be “triggered” as motion. Smaller values will lead to more motion being detected, larger values to less motion detected.
resolution : The width and height of the video frame from our Raspberry Pi camera.
fps : The desired Frames Per Second from our Raspberry Pi camera.
min_area : The minimum area size of an image (in pixels) for a region to be considered motion or not. Smaller values will lead to more areas marked as motion, whereas higher values of min_area  will only mark larger regions as motion.
timer_pin : The pin number to cnnect the timer to
detection_speed: The rate at which images are taken to comapare with the average image of the room. Larger values would lead to slower detection rate

command to run  the sript: python3 /home/pi/vacant/ObjectDetection/camera.py --conf /home/pi/vacant/ObjectDetection/conf.json 

set up:
plug a usb power source to the two radio display units
Then plug the three cables coming out of the raspberrypi into the transmitter radio by putting ash cable in the label 1, white cable in label two and black cabel in label 3 on the screw terminal in the transmiter radio
Power up the raspberrypi
