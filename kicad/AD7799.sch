EESchema Schematic File Version 2  date Sat 16 Nov 2013 05:35:54 AM PST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "16 nov 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 4900 3350 0    60   ~ 0
AD7799\n
Wire Wire Line
	4050 4350 4050 4250
Wire Wire Line
	4050 4250 4450 4250
Wire Notes Line
	5650 2600 4450 2600
Wire Notes Line
	5650 2600 5650 4350
Wire Notes Line
	5650 4350 4450 4350
Wire Notes Line
	4450 4350 4450 2600
Wire Wire Line
	5650 2700 5750 2700
$Comp
L GND #PWR?
U 1 1 52877102
P 4050 4350
F 0 "#PWR?" H 4050 4350 30  0001 C CNN
F 1 "GND" H 4050 4280 30  0001 C CNN
	1    4050 4350
	1    0    0    -1  
$EndComp
$Comp
L +3.3VP #PWR?
U 1 1 52876FDE
P 5750 2700
F 0 "#PWR?" H 5800 2730 20  0001 C CNN
F 1 "+3.3VP" H 5800 2800 30  0000 C CNN
	1    5750 2700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
