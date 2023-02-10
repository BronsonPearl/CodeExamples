This is a simulated weather station that uses the raspberry pi sense hat to display data in different colours. 

This was one of the first projects I worked on.  Most of the documentation for the project is contained in the PDF.  
in summary:

this program was written in C on the raspberry pi and then compiled use a gcc makefile.

the simulated weather data comes in as if it was being taken from a sensor.  Data is then stored in a struct.
depending on the value will determine a certain state for the machine on a high or low reading.
this state will determine the colour displayed on the sense hat. 
