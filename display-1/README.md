# Basic Operation

https://youtu.be/9w9oqg-CDX8

You can get a simple magnet switch from amazon, and the PIR motion sensor from sparkfun https://www.sparkfun.com/products/13285

The Alarm pin is pulled up to Vdd, shorting it to ground will reset the timer. This enables many open drain motion sensors to be connected in parallel, and if any detect motion, any connected timers will be reset.
