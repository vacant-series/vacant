# Instructions to Make a Vacant Display 1

### Tools

1. Soldering iron (with temp control).
2. Side cutters.
3. Tweezers.
4. Hot glue gun.
5. Soldering mat.
6. Hot air station (with temp and air flow control).
7. Solder sucker.
8. Soldering iron tip cleaner.
9. Toothbrush.
10. Fume extractor or fan.
11. ST-Link V2 (or a cheap clone).

### Supplies 

1. Solder (high quality leaded multicore with thin diameter).
2. Solder flux (AMTECH NC-559-V2-TF recommended).
3. Hot glue.
4. Nitrile gloves.
5. Isopropyl alcohol.
6. Wire.


### Parts

See the BOM at:

https://easyeda.com/nzimmerberg/vacant 

The LCD display: OD-358R (5V) or OD-357 (3V) availible at digikey, or another pin compatible LCD.

The PCB, the gerber files are availible through https://easyeda.com/nzimmerberg/vacant 

### Directions

#### Soldering

1. Solder the surface mount components, use lots of flux, and wear gloves.
2. Make sure the USB is fully soldered and won't fall off.
3. Solder the 3.3V regulator or short Vdd to 5V, depending on the voltage of the LCD.
3. Clean the flux residue with the toothbrush and isopropyl alcohol.
4. Solder the LCD display to the front in the correct direction.
5. Clip off the excess pin from the LCD.
6. Solder the screw terminal and clip excess pin.

#### Programming

Assuming linux OS

1. Install https://github.com/vdudouyt/stm8flash
2. Flash the program

On reset the program should turn on all connected segments for a second, so you can see if the display works.










