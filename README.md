# DDS-DMX

## The DMX interface for the [Disco Death Star](https://github.com/greiginsydney/FastLED-DiscoDeathStar)

As referenced in [DHMG2019 – the Disco Death Star – Pt.2](https://greiginsydney.com/dhmg2019-dds-pt2/), the DMX interface for the DDS is a discrete controller, based on a [SparkFun ESP32 Thing Plus](https://www.sparkfun.com/products/14689) and its [DMX to LED Shield](https://www.sparkfun.com/products/15110). Special commendation here goes to [luksal](https://github.com/luksal) for their [ESP32-DMX-RX](https://github.com/luksal/ESP32-DMX-RX), which worked for me where the native SparkFun library would not.

My DMX interfacing needed to be primitive because I didn’t want to add a processing load to the Teensy, and so it uses 5+3+3 IO pins in a parallel arrangement to signal pattern, speed and brightness, respectively. All of this is the cluster of cables at the top of the board in the image above. Five of the connections are the top five bits of the eight bits of the first DMX channel, giving a possible 32 patterns, whilst the other six pins are two lots of three IOs that give eight possible speeds and eight brightnesses.

I've also tweaked the top values of the brightness channel to add a "panic button". If you take Brightness to a value of 251-254 inclusive, it will take an IO pin high, which in turn will reset the Teensy in the ball controller. This is purely on the off-chance that the ball locks up mid-show.

As a further protection against accidentally setting the ball at full intensity - potentially exceeding the power supply limits - the Brightness value of 255 will set the LOWEST intensity instead of the highest.

&nbsp;

\- G.
