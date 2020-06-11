EESchema Schematic File Version 4
LIBS:input_board-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5EB793AF
P 5700 3650
F 0 "A1" H 5700 2561 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5700 2470 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 5850 2700 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5700 2650 50  0001 C CNN
	1    5700 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J2
U 1 1 5EB7A0A9
P 4750 2250
F 0 "J2" H 4800 2567 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 4800 2476 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Horizontal" H 4750 2250 50  0001 C CNN
F 3 "~" H 4750 2250 50  0001 C CNN
	1    4750 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5EB7B9D2
P 3950 3600
F 0 "J1" H 4030 3592 50  0000 L CNN
F 1 "Conn_01x04" H 4030 3501 50  0000 L CNN
F 2 "Connector_Phoenix_MSTB:PhoenixContact_MSTBA_2,5_4-G-5,08_1x04_P5.08mm_Horizontal" H 3950 3600 50  0001 C CNN
F 3 "~" H 3950 3600 50  0001 C CNN
	1    3950 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5EB7C9D3
P 4600 4450
F 0 "R1" H 4670 4496 50  0000 L CNN
F 1 "R" H 4670 4405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4530 4450 50  0001 C CNN
F 3 "~" H 4600 4450 50  0001 C CNN
	1    4600 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5EB7CC99
P 4600 4850
F 0 "R2" H 4670 4896 50  0000 L CNN
F 1 "R" H 4670 4805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4530 4850 50  0001 C CNN
F 3 "~" H 4600 4850 50  0001 C CNN
	1    4600 4850
	1    0    0    -1  
$EndComp
Text GLabel 5100 4350 0    50   Input ~ 0
SCK
Text GLabel 4450 4650 0    50   Input ~ 0
MISO
Text GLabel 5100 4050 0    50   Input ~ 0
CS
$Comp
L power:+5V #PWR?
U 1 1 5EB7F793
P 4300 2350
F 0 "#PWR?" H 4300 2200 50  0001 C CNN
F 1 "+5V" H 4315 2523 50  0000 C CNN
F 2 "" H 4300 2350 50  0001 C CNN
F 3 "" H 4300 2350 50  0001 C CNN
	1    4300 2350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EB80B43
P 5750 4950
F 0 "#PWR?" H 5750 4700 50  0001 C CNN
F 1 "GND" H 5755 4777 50  0000 C CNN
F 2 "" H 5750 4950 50  0001 C CNN
F 3 "" H 5750 4950 50  0001 C CNN
	1    5750 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5EB80E29
P 5900 2550
F 0 "#PWR?" H 5900 2400 50  0001 C CNN
F 1 "+5V" H 5915 2723 50  0000 C CNN
F 2 "" H 5900 2550 50  0001 C CNN
F 3 "" H 5900 2550 50  0001 C CNN
	1    5900 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EB83439
P 4600 5100
F 0 "#PWR?" H 4600 4850 50  0001 C CNN
F 1 "GND" H 4605 4927 50  0000 C CNN
F 2 "" H 4600 5100 50  0001 C CNN
F 3 "" H 4600 5100 50  0001 C CNN
	1    4600 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EB85294
P 5400 2350
F 0 "#PWR?" H 5400 2100 50  0001 C CNN
F 1 "GND" H 5405 2177 50  0000 C CNN
F 2 "" H 5400 2350 50  0001 C CNN
F 3 "" H 5400 2350 50  0001 C CNN
	1    5400 2350
	0    -1   -1   0   
$EndComp
Text GLabel 4500 2150 0    50   Input ~ 0
CS
Text GLabel 5100 2250 2    50   Input ~ 0
MISO
Text GLabel 4500 2250 0    50   Input ~ 0
SCK
Wire Wire Line
	5050 2250 5100 2250
Wire Wire Line
	4550 2150 4500 2150
Wire Wire Line
	4550 2250 4500 2250
Wire Wire Line
	4550 2350 4300 2350
Wire Wire Line
	5050 2350 5400 2350
Wire Wire Line
	4600 5000 4600 5100
Wire Wire Line
	4600 4600 4600 4650
Wire Wire Line
	4450 4650 4600 4650
Connection ~ 4600 4650
Wire Wire Line
	4600 4650 4600 4700
Wire Wire Line
	5100 4050 5200 4050
Wire Wire Line
	5100 4350 5200 4350
Wire Wire Line
	5900 2550 5900 2650
Wire Wire Line
	5800 4650 5800 4900
Wire Wire Line
	5800 4900 5750 4900
Wire Wire Line
	5750 4900 5750 4950
Wire Wire Line
	5700 4650 5700 4900
Wire Wire Line
	5700 4900 5750 4900
Connection ~ 5750 4900
$Comp
L power:+5V #PWR?
U 1 1 5EB9136F
P 4300 3050
F 0 "#PWR?" H 4300 2900 50  0001 C CNN
F 1 "+5V" H 4315 3223 50  0000 C CNN
F 2 "" H 4300 3050 50  0001 C CNN
F 3 "" H 4300 3050 50  0001 C CNN
	1    4300 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EB9224C
P 4200 3850
F 0 "#PWR?" H 4200 3600 50  0001 C CNN
F 1 "GND" H 4205 3677 50  0000 C CNN
F 2 "" H 4200 3850 50  0001 C CNN
F 3 "" H 4200 3850 50  0001 C CNN
	1    4200 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3350 4350 3350
Wire Wire Line
	4350 3350 4350 3500
Wire Wire Line
	4350 3500 4150 3500
Wire Wire Line
	4150 3600 4450 3600
Wire Wire Line
	4450 3600 4450 3450
Wire Wire Line
	4450 3450 5200 3450
Wire Wire Line
	4300 3050 4300 3400
Wire Wire Line
	4300 3400 4150 3400
Wire Wire Line
	4200 3850 4200 3700
Wire Wire Line
	4200 3700 4150 3700
Wire Wire Line
	5200 4250 4600 4250
Wire Wire Line
	4600 4250 4600 4300
Text GLabel 5100 2150 2    50   Input ~ 0
MOSI
Wire Wire Line
	5050 2150 5100 2150
Text GLabel 5100 4150 0    50   Input ~ 0
MOSI
Wire Wire Line
	5100 4150 5200 4150
$EndSCHEMATC
