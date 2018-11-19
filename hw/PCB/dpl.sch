EESchema Schematic File Version 4
LIBS:FOD-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L H-Bridge:SN754410 U3
U 1 1 5BEA29D2
P 5650 3450
F 0 "U3" H 5650 4065 50  0000 C CNN
F 1 "SN754410" H 5650 3974 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_LongPads" H 5500 3550 50  0001 C CNN
F 3 "" H 5500 3550 50  0001 C CNN
	1    5650 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5BEA225C
P 4750 3500
F 0 "R3" H 4681 3454 50  0000 R CNN
F 1 "7.15" H 4681 3545 50  0000 R CNN
F 2 "Resistor_FOD:RCL12257R15FKEG" V 4680 3500 50  0001 C CNN
F 3 "~" H 4750 3500 50  0001 C CNN
	1    4750 3500
	1    0    0    1   
$EndComp
Wire Wire Line
	6050 3100 6250 3100
Text Label 6250 3100 2    50   ~ 0
5V0
Text Label 4900 3200 0    50   ~ 0
DPL_SIG
Text Label 4900 3100 0    50   ~ 0
DPL_EN
Text Label 4900 3300 0    50   ~ 0
DPL_OUT
Wire Wire Line
	5250 3100 4900 3100
Wire Wire Line
	4900 3200 5250 3200
Wire Wire Line
	5250 3400 4900 3400
Wire Wire Line
	4900 3500 5250 3500
Wire Wire Line
	6050 3400 6250 3400
Wire Wire Line
	6250 3500 6050 3500
Text Label 4900 3400 0    50   ~ 0
GND
Text Label 4900 3500 0    50   ~ 0
GND
Text Label 6250 3400 2    50   ~ 0
GND
Text Label 6250 3500 2    50   ~ 0
GND
Wire Wire Line
	4750 3300 4750 3350
Wire Wire Line
	4750 3300 5250 3300
Wire Wire Line
	4750 3650 4750 3700
Wire Wire Line
	4750 3700 4900 3700
Wire Wire Line
	4900 3700 4900 3500
Wire Wire Line
	4750 3700 4750 3750
Connection ~ 4750 3700
$Comp
L power:GND #PWR0112
U 1 1 5BEA3F1A
P 4750 3750
F 0 "#PWR0112" H 4750 3500 50  0001 C CNN
F 1 "GND" H 4755 3577 50  0000 C CNN
F 2 "" H 4750 3750 50  0001 C CNN
F 3 "" H 4750 3750 50  0001 C CNN
	1    4750 3750
	1    0    0    -1  
$EndComp
Text HLabel 4900 3100 0    50   Input ~ 0
DPL_EN
Text HLabel 4900 3200 0    50   Input ~ 0
DPL_SIG
Text HLabel 4700 3700 0    50   Input ~ 0
GND
Wire Wire Line
	4750 3700 4700 3700
NoConn ~ 6050 3200
NoConn ~ 6050 3300
NoConn ~ 6050 3600
NoConn ~ 6050 3700
NoConn ~ 6050 3800
NoConn ~ 5250 3600
NoConn ~ 5250 3700
NoConn ~ 5250 3800
Text HLabel 6250 3100 2    50   Input ~ 0
5V0
$EndSCHEMATC
