EESchema Schematic File Version 4
LIBS:controller_board-cache
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
L Azure_Sphere:Avnet_Board U1
U 1 1 5EAA6142
P 6950 3650
F 0 "U1" H 6950 4425 50  0000 C CNN
F 1 "Avnet_Board" H 6950 4334 50  0000 C CNN
F 2 "Azure_Sphere:Avnet_Azure_Sphere" H 6950 3750 50  0001 C CNN
F 3 "" H 6950 3750 50  0001 C CNN
	1    6950 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5EAA9D95
P 7700 4050
F 0 "#PWR0102" H 7700 3800 50  0001 C CNN
F 1 "GND" V 7705 3922 50  0000 R CNN
F 2 "" H 7700 4050 50  0001 C CNN
F 3 "" H 7700 4050 50  0001 C CNN
	1    7700 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 3950 7700 3950
Wire Wire Line
	7600 4050 7700 4050
$Comp
L Regulator_Linear:L7805 U2
U 1 1 5EAAB145
P 8050 5250
F 0 "U2" H 8050 5492 50  0000 C CNN
F 1 "L7805" H 8050 5401 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8075 5100 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 8050 5200 50  0001 C CNN
	1    8050 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5EAAE522
P 8050 5650
F 0 "#PWR0101" H 8050 5400 50  0001 C CNN
F 1 "GND" H 8055 5477 50  0000 C CNN
F 2 "" H 8050 5650 50  0001 C CNN
F 3 "" H 8050 5650 50  0001 C CNN
	1    8050 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 5550 8050 5650
Wire Wire Line
	8350 5250 8400 5250
Wire Wire Line
	8400 5250 8400 5150
Wire Wire Line
	7750 5250 7700 5250
Wire Wire Line
	7700 5250 7700 5150
$Comp
L Regulator_Linear:L7805 U5
U 1 1 5EAB2D71
P 9050 5250
F 0 "U5" H 9050 5492 50  0000 C CNN
F 1 "L7805" H 9050 5401 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9075 5100 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 9050 5200 50  0001 C CNN
	1    9050 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5EAB2D7B
P 9050 5650
F 0 "#PWR0103" H 9050 5400 50  0001 C CNN
F 1 "GND" H 9055 5477 50  0000 C CNN
F 2 "" H 9050 5650 50  0001 C CNN
F 3 "" H 9050 5650 50  0001 C CNN
	1    9050 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 5550 9050 5650
Wire Wire Line
	9350 5250 9400 5250
Wire Wire Line
	9400 5250 9400 5150
Wire Wire Line
	8750 5250 8700 5250
Wire Wire Line
	8700 5250 8700 5150
$Comp
L power:+3.3V #PWR0104
U 1 1 5EABCC59
P 6150 4150
F 0 "#PWR0104" H 6150 4000 50  0001 C CNN
F 1 "+3.3V" V 6165 4278 50  0000 L CNN
F 2 "" H 6150 4150 50  0001 C CNN
F 3 "" H 6150 4150 50  0001 C CNN
	1    6150 4150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 4150 6150 4150
$Comp
L 74xx:74HC595 U4
U 1 1 5EAC15A9
P 2650 4500
F 0 "U4" H 2850 5250 50  0000 C CNN
F 1 "74HC595" H 2950 5150 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_LongPads" H 2650 4500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 2650 4500 50  0001 C CNN
	1    2650 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 5EAC15B3
P 2650 3800
F 0 "#PWR0105" H 2650 3650 50  0001 C CNN
F 1 "+3.3V" H 2665 3973 50  0000 C CNN
F 2 "" H 2650 3800 50  0001 C CNN
F 3 "" H 2650 3800 50  0001 C CNN
	1    2650 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5EAC15BD
P 2650 5300
F 0 "#PWR0106" H 2650 5050 50  0001 C CNN
F 1 "GND" H 2655 5127 50  0000 C CNN
F 2 "" H 2650 5300 50  0001 C CNN
F 3 "" H 2650 5300 50  0001 C CNN
	1    2650 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5200 2650 5300
Wire Wire Line
	2650 3800 2650 3900
$Comp
L power:+3.3V #PWR0107
U 1 1 5EAC918B
P 1800 4400
F 0 "#PWR0107" H 1800 4250 50  0001 C CNN
F 1 "+3.3V" V 1815 4528 50  0000 L CNN
F 2 "" H 1800 4400 50  0001 C CNN
F 3 "" H 1800 4400 50  0001 C CNN
	1    1800 4400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 4400 2150 4400
$Comp
L Device:R R3
U 1 1 5EACA492
P 2000 4400
F 0 "R3" V 2100 4350 50  0000 C CNN
F 1 "R" V 2100 4500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1930 4400 50  0001 C CNN
F 3 "~" H 2000 4400 50  0001 C CNN
	1    2000 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	1850 4400 1800 4400
$Comp
L Device:R R4
U 1 1 5EACCE04
P 2000 4700
F 0 "R4" V 2100 4650 50  0000 C CNN
F 1 "R" V 2100 4800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1930 4700 50  0001 C CNN
F 3 "~" H 2000 4700 50  0001 C CNN
	1    2000 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5EACD39A
P 1800 4700
F 0 "#PWR0108" H 1800 4450 50  0001 C CNN
F 1 "GND" V 1805 4572 50  0000 R CNN
F 2 "" H 1800 4700 50  0001 C CNN
F 3 "" H 1800 4700 50  0001 C CNN
	1    1800 4700
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 4700 2150 4700
Wire Wire Line
	1850 4700 1800 4700
$Comp
L 74xx:74HC595 U3
U 1 1 5EAD082C
P 2650 2450
F 0 "U3" H 2850 3200 50  0000 C CNN
F 1 "74HC595" H 2950 3100 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_LongPads" H 2650 2450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 2650 2450 50  0001 C CNN
	1    2650 2450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0109
U 1 1 5EAD0836
P 2650 1750
F 0 "#PWR0109" H 2650 1600 50  0001 C CNN
F 1 "+3.3V" H 2665 1923 50  0000 C CNN
F 2 "" H 2650 1750 50  0001 C CNN
F 3 "" H 2650 1750 50  0001 C CNN
	1    2650 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5EAD0840
P 2650 3250
F 0 "#PWR0110" H 2650 3000 50  0001 C CNN
F 1 "GND" H 2655 3077 50  0000 C CNN
F 2 "" H 2650 3250 50  0001 C CNN
F 3 "" H 2650 3250 50  0001 C CNN
	1    2650 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3150 2650 3250
Wire Wire Line
	2650 1750 2650 1850
$Comp
L power:+3.3V #PWR0111
U 1 1 5EAD084C
P 1800 2350
F 0 "#PWR0111" H 1800 2200 50  0001 C CNN
F 1 "+3.3V" V 1815 2478 50  0000 L CNN
F 2 "" H 1800 2350 50  0001 C CNN
F 3 "" H 1800 2350 50  0001 C CNN
	1    1800 2350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 2350 2150 2350
$Comp
L Device:R R1
U 1 1 5EAD0857
P 2000 2350
F 0 "R1" V 2100 2300 50  0000 C CNN
F 1 "R" V 2100 2450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1930 2350 50  0001 C CNN
F 3 "~" H 2000 2350 50  0001 C CNN
	1    2000 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	1850 2350 1800 2350
$Comp
L Device:R R2
U 1 1 5EAD0862
P 2000 2650
F 0 "R2" V 2100 2600 50  0000 C CNN
F 1 "R" V 2100 2750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1930 2650 50  0001 C CNN
F 3 "~" H 2000 2650 50  0001 C CNN
	1    2000 2650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5EAD086C
P 1800 2650
F 0 "#PWR0112" H 1800 2400 50  0001 C CNN
F 1 "GND" V 1805 2522 50  0000 R CNN
F 2 "" H 1800 2650 50  0001 C CNN
F 3 "" H 1800 2650 50  0001 C CNN
	1    1800 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2650 2150 2650
Wire Wire Line
	1850 2650 1800 2650
Wire Wire Line
	3050 2950 3150 2950
Wire Wire Line
	3150 2950 3150 3500
Wire Wire Line
	2150 3500 2150 4100
Wire Wire Line
	2150 4100 2250 4100
Wire Wire Line
	2150 3500 3150 3500
Wire Wire Line
	2250 2550 1400 2550
Wire Wire Line
	1400 2550 1400 4600
Wire Wire Line
	1400 4600 2250 4600
Wire Wire Line
	2250 2250 2000 2250
Wire Wire Line
	1300 2250 1300 4300
Wire Wire Line
	1300 4300 2250 4300
Wire Wire Line
	2250 2050 2100 2050
Wire Wire Line
	2100 2050 2100 1600
Wire Wire Line
	2000 2250 2000 1600
Connection ~ 2000 2250
Wire Wire Line
	2000 2250 1300 2250
Wire Wire Line
	1400 2550 1400 2050
Wire Wire Line
	1400 2050 1900 2050
Wire Wire Line
	1900 2050 1900 1600
Connection ~ 1400 2550
Text GLabel 6200 3950 0    50   Input ~ 0
LED_REGISTER_CLOCK
Text GLabel 2000 1600 1    50   Input ~ 0
LED_REGISTER_CLOCK
Text GLabel 6200 3200 0    50   Input ~ 0
LED_REGISTER_LATCH
Text GLabel 1900 1600 1    50   Input ~ 0
LED_REGISTER_LATCH
Text GLabel 2100 1600 1    50   Input ~ 0
LED_REGISTER_DATA
Text GLabel 7700 3850 2    50   Input ~ 0
LED_REGISTER_DATA
Wire Wire Line
	6200 3200 6300 3200
Wire Wire Line
	6200 3950 6300 3950
Wire Wire Line
	7600 3850 7700 3850
Wire Wire Line
	7600 3200 7850 3200
Wire Wire Line
	7600 3100 7650 3100
Wire Wire Line
	8050 3100 8050 2800
Wire Wire Line
	8050 2800 8250 2800
$Comp
L Device:R R21
U 1 1 5EB05735
P 7650 2800
F 0 "R21" V 7750 2750 50  0000 C CNN
F 1 "R" V 7750 2900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7580 2800 50  0001 C CNN
F 3 "~" H 7650 2800 50  0001 C CNN
	1    7650 2800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R22
U 1 1 5EB066DB
P 7850 2800
F 0 "R22" V 7950 2750 50  0000 C CNN
F 1 "R" V 7950 2900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7780 2800 50  0001 C CNN
F 3 "~" H 7850 2800 50  0001 C CNN
	1    7850 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 3100 7650 2950
Connection ~ 7650 3100
Wire Wire Line
	7650 3100 8050 3100
Wire Wire Line
	7850 3200 7850 2950
Connection ~ 7850 3200
Wire Wire Line
	7850 3200 8250 3200
$Comp
L power:+3.3V #PWR0113
U 1 1 5EB08B06
P 7650 2550
F 0 "#PWR0113" H 7650 2400 50  0001 C CNN
F 1 "+3.3V" H 7665 2723 50  0000 C CNN
F 2 "" H 7650 2550 50  0001 C CNN
F 3 "" H 7650 2550 50  0001 C CNN
	1    7650 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0114
U 1 1 5EB0991D
P 7850 2550
F 0 "#PWR0114" H 7850 2400 50  0001 C CNN
F 1 "+3.3V" H 7865 2723 50  0000 C CNN
F 2 "" H 7850 2550 50  0001 C CNN
F 3 "" H 7850 2550 50  0001 C CNN
	1    7850 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 2650 7650 2550
Wire Wire Line
	7850 2650 7850 2550
$Comp
L power:GND #PWR0115
U 1 1 5EB0BE7D
P 8700 3000
F 0 "#PWR0115" H 8700 2750 50  0001 C CNN
F 1 "GND" V 8705 2872 50  0000 R CNN
F 2 "" H 8700 3000 50  0001 C CNN
F 3 "" H 8700 3000 50  0001 C CNN
	1    8700 3000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 5EB0FA50
P 8450 2800
F 0 "J5" H 8530 2792 50  0000 L CNN
F 1 "Conn_SP_BUTTON" H 8530 2701 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8450 2800 50  0001 C CNN
F 3 "~" H 8450 2800 50  0001 C CNN
	1    8450 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 5EB102EC
P 8450 3100
F 0 "J6" H 8530 3092 50  0000 L CNN
F 1 "Conn_IP_BUTTON" H 8530 3001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8450 3100 50  0001 C CNN
F 3 "~" H 8450 3100 50  0001 C CNN
	1    8450 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2900 8200 2900
Wire Wire Line
	8200 2900 8200 3000
Wire Wire Line
	8200 3000 8700 3000
Wire Wire Line
	8250 3100 8200 3100
Wire Wire Line
	8200 3100 8200 3000
Connection ~ 8200 3000
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 5EB15C4E
P 7800 4550
F 0 "J4" V 7672 4630 50  0000 L CNN
F 1 "Conn_BATTERY" V 7763 4630 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 4550 50  0001 C CNN
F 3 "~" H 7800 4550 50  0001 C CNN
	1    7800 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	7600 4150 7700 4150
Wire Wire Line
	7700 4150 7700 4350
$Comp
L power:GND #PWR0116
U 1 1 5EB17FDC
P 8000 4300
F 0 "#PWR0116" H 8000 4050 50  0001 C CNN
F 1 "GND" V 8005 4172 50  0000 R CNN
F 2 "" H 8000 4300 50  0001 C CNN
F 3 "" H 8000 4300 50  0001 C CNN
	1    8000 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7800 4350 7800 4300
Wire Wire Line
	7800 4300 8000 4300
$Comp
L power:+5P #PWR0118
U 1 1 5EB1E83C
P 8400 5150
F 0 "#PWR0118" H 8400 5000 50  0001 C CNN
F 1 "+5P" H 8415 5323 50  0000 C CNN
F 2 "" H 8400 5150 50  0001 C CNN
F 3 "" H 8400 5150 50  0001 C CNN
	1    8400 5150
	1    0    0    -1  
$EndComp
$Comp
L power:+12L #PWR0119
U 1 1 5EB2019E
P 7700 5150
F 0 "#PWR0119" H 7700 5000 50  0001 C CNN
F 1 "+12L" H 7715 5323 50  0000 C CNN
F 2 "" H 7700 5150 50  0001 C CNN
F 3 "" H 7700 5150 50  0001 C CNN
	1    7700 5150
	1    0    0    -1  
$EndComp
$Comp
L power:+5C #PWR0120
U 1 1 5EB21125
P 9400 5150
F 0 "#PWR0120" H 9400 5000 50  0001 C CNN
F 1 "+5C" H 9415 5323 50  0000 C CNN
F 2 "" H 9400 5150 50  0001 C CNN
F 3 "" H 9400 5150 50  0001 C CNN
	1    9400 5150
	1    0    0    -1  
$EndComp
$Comp
L power:+12L #PWR0121
U 1 1 5EB21BBB
P 8700 5150
F 0 "#PWR0121" H 8700 5000 50  0001 C CNN
F 1 "+12L" H 8715 5323 50  0000 C CNN
F 2 "" H 8700 5150 50  0001 C CNN
F 3 "" H 8700 5150 50  0001 C CNN
	1    8700 5150
	1    0    0    -1  
$EndComp
$Comp
L power:+5C #PWR0122
U 1 1 5EB21F94
P 7700 3950
F 0 "#PWR0122" H 7700 3800 50  0001 C CNN
F 1 "+5C" V 7715 4078 50  0000 L CNN
F 2 "" H 7700 3950 50  0001 C CNN
F 3 "" H 7700 3950 50  0001 C CNN
	1    7700 3950
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0124
U 1 1 5EB257EA
P 6150 4250
F 0 "#PWR0124" H 6150 4100 50  0001 C CNN
F 1 "+5V" V 6165 4378 50  0000 L CNN
F 2 "" H 6150 4250 50  0001 C CNN
F 3 "" H 6150 4250 50  0001 C CNN
	1    6150 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 4250 6150 4250
$Comp
L power:GND #PWR0125
U 1 1 5EB31878
P 6750 2250
F 0 "#PWR0125" H 6750 2000 50  0001 C CNN
F 1 "GND" V 6755 2122 50  0000 R CNN
F 2 "" H 6750 2250 50  0001 C CNN
F 3 "" H 6750 2250 50  0001 C CNN
	1    6750 2250
	0    -1   -1   0   
$EndComp
$Comp
L power:+5P #PWR0126
U 1 1 5EB31882
P 6750 2350
F 0 "#PWR0126" H 6750 2200 50  0001 C CNN
F 1 "+5P" V 6765 2478 50  0000 L CNN
F 2 "" H 6750 2350 50  0001 C CNN
F 3 "" H 6750 2350 50  0001 C CNN
	1    6750 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	6650 2250 6750 2250
Wire Wire Line
	6650 2350 6750 2350
$Comp
L Connector_Generic:Conn_02x03_Counter_Clockwise J3
U 1 1 5EB3188E
P 6350 2350
F 0 "J3" H 6400 2667 50  0000 C CNN
F 1 "Conn_02x03_Counter_Clockwise" H 6400 2576 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Horizontal" H 6350 2350 50  0001 C CNN
F 3 "~" H 6350 2350 50  0001 C CNN
	1    6350 2350
	1    0    0    -1  
$EndComp
Text GLabel 6150 2250 0    50   Input ~ 0
PERIPH_OUT_CS
Text GLabel 6650 2450 2    50   Input ~ 0
PERIPH_MISO
Text GLabel 6150 2350 0    50   Input ~ 0
PERIPH_MOSI
Text GLabel 6150 2450 0    50   Input ~ 0
PERIPH_SCK
Text GLabel 6200 4050 0    50   Input ~ 0
PERIPH_OUT_CS
Text GLabel 6200 3300 0    50   Input ~ 0
PERIPH_IN_CS
Text GLabel 6200 3500 0    50   Input ~ 0
PERIPH_MISO
Text GLabel 6200 3600 0    50   Input ~ 0
PERIPH_MOSI
Text GLabel 6200 3400 0    50   Input ~ 0
PERIPH_SCK
Wire Wire Line
	6200 3300 6300 3300
Wire Wire Line
	6200 3400 6300 3400
Wire Wire Line
	6200 3500 6300 3500
Wire Wire Line
	6200 3600 6300 3600
Wire Wire Line
	6200 4050 6300 4050
$Comp
L Device:R R5
U 1 1 5EB46CFA
P 3300 2050
F 0 "R5" V 3400 2000 50  0000 C CNN
F 1 "R" V 3400 2150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2050 50  0001 C CNN
F 3 "~" H 3300 2050 50  0001 C CNN
	1    3300 2050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R6
U 1 1 5EB477FC
P 3300 2150
F 0 "R6" V 3400 2100 50  0000 C CNN
F 1 "R" V 3400 2250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2150 50  0001 C CNN
F 3 "~" H 3300 2150 50  0001 C CNN
	1    3300 2150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 5EB47ACF
P 3300 2250
F 0 "R7" V 3400 2200 50  0000 C CNN
F 1 "R" V 3400 2350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2250 50  0001 C CNN
F 3 "~" H 3300 2250 50  0001 C CNN
	1    3300 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R8
U 1 1 5EB47D7F
P 3300 2350
F 0 "R8" V 3400 2300 50  0000 C CNN
F 1 "R" V 3400 2450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2350 50  0001 C CNN
F 3 "~" H 3300 2350 50  0001 C CNN
	1    3300 2350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 5EB4809C
P 3300 2450
F 0 "R9" V 3400 2400 50  0000 C CNN
F 1 "R" V 3400 2550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2450 50  0001 C CNN
F 3 "~" H 3300 2450 50  0001 C CNN
	1    3300 2450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R10
U 1 1 5EB483F1
P 3300 2550
F 0 "R10" V 3400 2500 50  0000 C CNN
F 1 "R" V 3400 2650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2550 50  0001 C CNN
F 3 "~" H 3300 2550 50  0001 C CNN
	1    3300 2550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5EB486FA
P 3300 2650
F 0 "R11" V 3400 2600 50  0000 C CNN
F 1 "R" V 3400 2750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2650 50  0001 C CNN
F 3 "~" H 3300 2650 50  0001 C CNN
	1    3300 2650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5EB48A5C
P 3300 2750
F 0 "R12" V 3400 2700 50  0000 C CNN
F 1 "R" V 3400 2850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 2750 50  0001 C CNN
F 3 "~" H 3300 2750 50  0001 C CNN
	1    3300 2750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R13
U 1 1 5EB48DD3
P 3300 4100
F 0 "R13" V 3400 4050 50  0000 C CNN
F 1 "R" V 3400 4200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4100 50  0001 C CNN
F 3 "~" H 3300 4100 50  0001 C CNN
	1    3300 4100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 5EB499BE
P 3300 4200
F 0 "R14" V 3400 4150 50  0000 C CNN
F 1 "R" V 3400 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4200 50  0001 C CNN
F 3 "~" H 3300 4200 50  0001 C CNN
	1    3300 4200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5EB49CE3
P 3300 4300
F 0 "R15" V 3400 4250 50  0000 C CNN
F 1 "R" V 3400 4400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4300 50  0001 C CNN
F 3 "~" H 3300 4300 50  0001 C CNN
	1    3300 4300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5EB4A02A
P 3300 4400
F 0 "R16" V 3400 4350 50  0000 C CNN
F 1 "R" V 3400 4500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4400 50  0001 C CNN
F 3 "~" H 3300 4400 50  0001 C CNN
	1    3300 4400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R17
U 1 1 5EB4A3E5
P 3300 4500
F 0 "R17" V 3400 4450 50  0000 C CNN
F 1 "R" V 3400 4600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4500 50  0001 C CNN
F 3 "~" H 3300 4500 50  0001 C CNN
	1    3300 4500
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x06_Odd_Even J1
U 1 1 5EB4A809
P 4350 3250
F 0 "J1" H 4400 3667 50  0000 C CNN
F 1 "Conn_02x06_Odd_Even" H 4400 3576 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x06_P2.54mm_Vertical" H 4350 3250 50  0001 C CNN
F 3 "~" H 4350 3250 50  0001 C CNN
	1    4350 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 2050 3950 2050
Wire Wire Line
	3950 2050 3950 3050
Wire Wire Line
	3950 3050 4050 3050
Wire Wire Line
	3450 2150 3900 2150
Wire Wire Line
	3900 2150 3900 3150
Wire Wire Line
	3450 2250 3850 2250
Wire Wire Line
	3850 2250 3850 3250
Wire Wire Line
	3450 2350 3800 2350
Wire Wire Line
	3800 2350 3800 3350
Wire Wire Line
	3450 2450 3750 2450
Wire Wire Line
	3750 2450 3750 3450
Wire Wire Line
	3450 2550 3700 2550
Wire Wire Line
	3700 2550 3700 3550
Wire Wire Line
	3450 2650 4650 2650
Wire Wire Line
	4650 2650 4650 3050
Wire Wire Line
	4650 3050 4550 3050
Wire Wire Line
	3450 2750 4700 2750
Wire Wire Line
	4700 2750 4700 3150
Wire Wire Line
	4700 3150 4550 3150
Wire Wire Line
	3450 4100 4650 4100
Wire Wire Line
	4650 4100 4650 3250
Wire Wire Line
	4650 3250 4550 3250
Wire Wire Line
	3450 4200 4700 4200
Wire Wire Line
	4700 4200 4700 3350
Wire Wire Line
	4700 3350 4550 3350
$Comp
L power:GND #PWR0127
U 1 1 5EB69947
P 4800 3550
F 0 "#PWR0127" H 4800 3300 50  0001 C CNN
F 1 "GND" V 4805 3422 50  0000 R CNN
F 2 "" H 4800 3550 50  0001 C CNN
F 3 "" H 4800 3550 50  0001 C CNN
	1    4800 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4550 3550 4600 3550
Wire Wire Line
	4550 3450 4600 3450
Wire Wire Line
	4600 3450 4600 3550
Connection ~ 4600 3550
Wire Wire Line
	4600 3550 4800 3550
Wire Wire Line
	3050 2050 3150 2050
Wire Wire Line
	3050 2150 3150 2150
Wire Wire Line
	3050 2250 3150 2250
Wire Wire Line
	3050 2350 3150 2350
Wire Wire Line
	3050 2450 3150 2450
Wire Wire Line
	3050 2550 3150 2550
Wire Wire Line
	3050 2650 3150 2650
Wire Wire Line
	3050 2750 3150 2750
Wire Wire Line
	3050 4100 3150 4100
Wire Wire Line
	3050 4200 3150 4200
Wire Wire Line
	3050 4300 3150 4300
Wire Wire Line
	3050 4400 3150 4400
Wire Wire Line
	3050 4500 3150 4500
$Comp
L Device:R R18
U 1 1 5EBD4FE2
P 3300 4600
F 0 "R18" V 3400 4550 50  0000 C CNN
F 1 "R" V 3400 4700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4600 50  0001 C CNN
F 3 "~" H 3300 4600 50  0001 C CNN
	1    3300 4600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R19
U 1 1 5EBD51C5
P 3300 4700
F 0 "R19" V 3400 4650 50  0000 C CNN
F 1 "R" V 3400 4800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4700 50  0001 C CNN
F 3 "~" H 3300 4700 50  0001 C CNN
	1    3300 4700
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R20
U 1 1 5EBD551E
P 3300 4800
F 0 "R20" V 3400 4750 50  0000 C CNN
F 1 "R" V 3400 4900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 4800 50  0001 C CNN
F 3 "~" H 3300 4800 50  0001 C CNN
	1    3300 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 4600 3150 4600
Wire Wire Line
	3050 4700 3150 4700
Wire Wire Line
	3050 4800 3150 4800
Text GLabel 7700 3600 2    50   Input ~ 0
EEPROM_SDA
Text GLabel 7700 3500 2    50   Input ~ 0
EEPROM_SCL
Wire Wire Line
	7600 3500 7700 3500
Wire Wire Line
	7600 3600 7700 3600
$Comp
L Connector_Generic:Conn_01x04 J7
U 1 1 5EC1308D
P 9400 3400
F 0 "J7" H 9480 3392 50  0000 L CNN
F 1 "Conn_DISPLAY" H 9480 3301 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9400 3400 50  0001 C CNN
F 3 "~" H 9400 3400 50  0001 C CNN
	1    9400 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3300 9200 3300
Wire Wire Line
	7600 3400 9200 3400
$Comp
L power:+5V #PWR0128
U 1 1 5EC25922
P 9100 3500
F 0 "#PWR0128" H 9100 3350 50  0001 C CNN
F 1 "+5V" V 9115 3628 50  0000 L CNN
F 2 "" H 9100 3500 50  0001 C CNN
F 3 "" H 9100 3500 50  0001 C CNN
	1    9100 3500
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 5EC260CD
P 9100 3600
F 0 "#PWR0129" H 9100 3350 50  0001 C CNN
F 1 "GND" V 9105 3472 50  0000 R CNN
F 2 "" H 9100 3600 50  0001 C CNN
F 3 "" H 9100 3600 50  0001 C CNN
	1    9100 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 3500 9100 3500
Wire Wire Line
	9200 3600 9100 3600
Text Label 7950 3300 0    50   ~ 0
CLOCK
Text Label 7950 3400 0    50   ~ 0
DATA
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 5EAC69FF
P 6250 5400
F 0 "J8" H 6330 5392 50  0000 L CNN
F 1 "Conn_POWER" H 6330 5301 50  0000 L CNN
F 2 "Connector_Phoenix_MSTB:PhoenixContact_MSTBA_2,5_2-G_1x02_P5.00mm_Horizontal" H 6250 5400 50  0001 C CNN
F 3 "~" H 6250 5400 50  0001 C CNN
	1    6250 5400
	0    1    1    0   
$EndComp
$Comp
L power:+12L #PWR0130
U 1 1 5EAC75A2
P 6150 5100
F 0 "#PWR0130" H 6150 4950 50  0001 C CNN
F 1 "+12L" H 6165 5273 50  0000 C CNN
F 2 "" H 6150 5100 50  0001 C CNN
F 3 "" H 6150 5100 50  0001 C CNN
	1    6150 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0131
U 1 1 5EAC7C87
P 6400 5100
F 0 "#PWR0131" H 6400 4850 50  0001 C CNN
F 1 "GND" H 6405 4927 50  0000 C CNN
F 2 "" H 6400 5100 50  0001 C CNN
F 3 "" H 6400 5100 50  0001 C CNN
	1    6400 5100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 5200 6250 5150
Wire Wire Line
	6250 5150 6400 5150
Wire Wire Line
	6400 5150 6400 5100
Wire Wire Line
	6150 5200 6150 5100
Wire Wire Line
	3750 3450 4050 3450
Wire Wire Line
	3800 3350 4050 3350
Wire Wire Line
	3850 3250 4050 3250
Wire Wire Line
	3900 3150 4050 3150
Wire Wire Line
	3700 3550 4050 3550
Text GLabel 6150 1850 0    50   Input ~ 0
PERIPH_SCK
Text GLabel 6150 1750 0    50   Input ~ 0
PERIPH_MOSI
Text GLabel 6650 1850 2    50   Input ~ 0
PERIPH_MISO
Text GLabel 6150 1650 0    50   Input ~ 0
PERIPH_IN_CS
Wire Wire Line
	6650 1750 6750 1750
$Comp
L power:+5P #PWR0123
U 1 1 5EB22626
P 6750 1750
F 0 "#PWR0123" H 6750 1600 50  0001 C CNN
F 1 "+5P" V 6765 1878 50  0000 L CNN
F 2 "" H 6750 1750 50  0001 C CNN
F 3 "" H 6750 1750 50  0001 C CNN
	1    6750 1750
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5EB1D5EB
P 6750 1650
F 0 "#PWR0117" H 6750 1400 50  0001 C CNN
F 1 "GND" V 6755 1522 50  0000 R CNN
F 2 "" H 6750 1650 50  0001 C CNN
F 3 "" H 6750 1650 50  0001 C CNN
	1    6750 1650
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Counter_Clockwise J2
U 1 1 5EB2B304
P 6350 1750
F 0 "J2" H 6400 2067 50  0000 C CNN
F 1 "Conn_02x03_Counter_Clockwise" H 6400 1976 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x03_P2.54mm_Horizontal" H 6350 1750 50  0001 C CNN
F 3 "~" H 6350 1750 50  0001 C CNN
	1    6350 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1650 6750 1650
$Comp
L Connector_Generic:Conn_01x03 J15
U 1 1 5EAB6B3D
P 9100 1100
F 0 "J15" H 9180 1142 50  0000 L CNN
F 1 "Conn_Knob" H 9180 1051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9100 1100 50  0001 C CNN
F 3 "~" H 9100 1100 50  0001 C CNN
	1    9100 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R23
U 1 1 5EAB8968
P 8600 1300
F 0 "R23" V 8700 1250 50  0000 C CNN
F 1 "R" V 8700 1400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8530 1300 50  0001 C CNN
F 3 "~" H 8600 1300 50  0001 C CNN
	1    8600 1300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R24
U 1 1 5EAB8CF1
P 8600 1700
F 0 "R24" V 8700 1650 50  0000 C CNN
F 1 "R" V 8700 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8530 1700 50  0001 C CNN
F 3 "~" H 8600 1700 50  0001 C CNN
	1    8600 1700
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0132
U 1 1 5EAB9925
P 8850 900
F 0 "#PWR0132" H 8850 750 50  0001 C CNN
F 1 "+3.3V" H 8865 1073 50  0000 C CNN
F 2 "" H 8850 900 50  0001 C CNN
F 3 "" H 8850 900 50  0001 C CNN
	1    8850 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 5EABA077
P 8850 1300
F 0 "#PWR0133" H 8850 1050 50  0001 C CNN
F 1 "GND" V 8855 1172 50  0000 R CNN
F 2 "" H 8850 1300 50  0001 C CNN
F 3 "" H 8850 1300 50  0001 C CNN
	1    8850 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1200 8850 1200
Wire Wire Line
	8850 1200 8850 1300
Wire Wire Line
	8900 1000 8850 1000
Wire Wire Line
	8850 1000 8850 900 
Wire Wire Line
	8900 1100 8600 1100
Wire Wire Line
	8600 1100 8600 1150
Wire Wire Line
	8600 1450 8600 1500
$Comp
L power:GND #PWR0134
U 1 1 5EAE3241
P 8600 1950
F 0 "#PWR0134" H 8600 1700 50  0001 C CNN
F 1 "GND" V 8605 1822 50  0000 R CNN
F 2 "" H 8600 1950 50  0001 C CNN
F 3 "" H 8600 1950 50  0001 C CNN
	1    8600 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1850 8600 1950
Text GLabel 8400 1500 0    50   Input ~ 0
Knob
Wire Wire Line
	8400 1500 8600 1500
Connection ~ 8600 1500
Wire Wire Line
	8600 1500 8600 1550
Text GLabel 6200 3100 0    50   Input ~ 0
Knob
Wire Wire Line
	6200 3100 6300 3100
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 5EAF810D
P 4300 4300
F 0 "J9" H 4380 4292 50  0000 L CNN
F 1 "Conn_SP_LED" H 4380 4201 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 4300 50  0001 C CNN
F 3 "~" H 4300 4300 50  0001 C CNN
	1    4300 4300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J10
U 1 1 5EAF8D7F
P 4300 4600
F 0 "J10" H 4380 4592 50  0000 L CNN
F 1 "Conn_Knob_LED" H 4380 4501 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 4600 50  0001 C CNN
F 3 "~" H 4300 4600 50  0001 C CNN
	1    4300 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J11
U 1 1 5EAF91A4
P 4300 4900
F 0 "J11" H 4380 4892 50  0000 L CNN
F 1 "Conn_IP_LED" H 4380 4801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 4900 50  0001 C CNN
F 3 "~" H 4300 4900 50  0001 C CNN
	1    4300 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 5EB0956E
P 4000 6000
F 0 "#PWR0135" H 4000 5750 50  0001 C CNN
F 1 "GND" V 4005 5872 50  0000 R CNN
F 2 "" H 4000 6000 50  0001 C CNN
F 3 "" H 4000 6000 50  0001 C CNN
	1    4000 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5000 4000 5000
Wire Wire Line
	4100 4700 4000 4700
Wire Wire Line
	4000 4700 4000 5000
Connection ~ 4000 5000
Wire Wire Line
	4100 4400 4000 4400
Wire Wire Line
	4000 4400 4000 4700
Connection ~ 4000 4700
Wire Wire Line
	4100 4300 3450 4300
Wire Wire Line
	3450 4400 3900 4400
Wire Wire Line
	3900 4400 3900 4600
Wire Wire Line
	3900 4600 4100 4600
Wire Wire Line
	3450 4500 3850 4500
Wire Wire Line
	3850 4900 4100 4900
Wire Wire Line
	3850 4500 3850 4900
$Comp
L Connector_Generic:Conn_01x02 J12
U 1 1 5EB3F3CC
P 4300 5200
F 0 "J12" H 4380 5192 50  0000 L CNN
F 1 "Conn_Cloud_ERR" H 4380 5101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 5200 50  0001 C CNN
F 3 "~" H 4300 5200 50  0001 C CNN
	1    4300 5200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J13
U 1 1 5EB3F819
P 4300 5500
F 0 "J13" H 4380 5492 50  0000 L CNN
F 1 "Conn_Network_ERR" H 4380 5401 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 5500 50  0001 C CNN
F 3 "~" H 4300 5500 50  0001 C CNN
	1    4300 5500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J14
U 1 1 5EB3FD1B
P 4300 5800
F 0 "J14" H 4380 5792 50  0000 L CNN
F 1 "Conn_MQTT_ERR" H 4380 5701 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 5800 50  0001 C CNN
F 3 "~" H 4300 5800 50  0001 C CNN
	1    4300 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 5000 4000 5300
Wire Wire Line
	4100 5300 4000 5300
Connection ~ 4000 5300
Wire Wire Line
	4000 5300 4000 5600
Wire Wire Line
	4100 5600 4000 5600
Connection ~ 4000 5600
Wire Wire Line
	4000 5600 4000 5900
Wire Wire Line
	4100 5900 4000 5900
Connection ~ 4000 5900
Wire Wire Line
	4000 5900 4000 6000
Wire Wire Line
	4100 5200 3800 5200
Wire Wire Line
	3800 5200 3800 4600
Wire Wire Line
	3800 4600 3450 4600
Wire Wire Line
	4100 5500 3750 5500
Wire Wire Line
	3750 5500 3750 4700
Wire Wire Line
	3750 4700 3450 4700
Wire Wire Line
	4100 5800 3700 5800
Wire Wire Line
	3700 5800 3700 4800
Wire Wire Line
	3700 4800 3450 4800
$Comp
L Memory_EEPROM:24LC01 U6
U 1 1 5EB92880
P 9350 2200
F 0 "U6" H 9500 2550 50  0000 C CNN
F 1 "24LC01" H 9600 2450 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 9350 2200 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21711J.pdf" H 9350 2200 50  0001 C CNN
	1    9350 2200
	1    0    0    -1  
$EndComp
Text GLabel 10300 2200 2    50   Input ~ 0
EEPROM_SCL
Text GLabel 10300 2100 2    50   Input ~ 0
EEPROM_SDA
$Comp
L power:GND #PWR0136
U 1 1 5EB949D5
P 9350 2600
F 0 "#PWR0136" H 9350 2350 50  0001 C CNN
F 1 "GND" V 9355 2472 50  0000 R CNN
F 2 "" H 9350 2600 50  0001 C CNN
F 3 "" H 9350 2600 50  0001 C CNN
	1    9350 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0137
U 1 1 5EB94DF5
P 9350 1800
F 0 "#PWR0137" H 9350 1650 50  0001 C CNN
F 1 "+3.3V" H 9365 1973 50  0000 C CNN
F 2 "" H 9350 1800 50  0001 C CNN
F 3 "" H 9350 1800 50  0001 C CNN
	1    9350 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0138
U 1 1 5EB9566D
P 9850 2500
F 0 "#PWR0138" H 9850 2250 50  0001 C CNN
F 1 "GND" V 9855 2372 50  0000 R CNN
F 2 "" H 9850 2500 50  0001 C CNN
F 3 "" H 9850 2500 50  0001 C CNN
	1    9850 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 2300 9850 2300
Wire Wire Line
	9850 2300 9850 2500
Wire Wire Line
	9350 2500 9350 2600
Wire Wire Line
	9350 1900 9350 1800
$Comp
L Device:R R26
U 1 1 5EBE2A74
P 10200 1700
F 0 "R26" V 10300 1650 50  0000 C CNN
F 1 "R" V 10300 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 10130 1700 50  0001 C CNN
F 3 "~" H 10200 1700 50  0001 C CNN
	1    10200 1700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R25
U 1 1 5EBE3FE8
P 9950 1700
F 0 "R25" V 10050 1650 50  0000 C CNN
F 1 "R" V 10050 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9880 1700 50  0001 C CNN
F 3 "~" H 9950 1700 50  0001 C CNN
	1    9950 1700
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0139
U 1 1 5EBF005F
P 10050 1400
F 0 "#PWR0139" H 10050 1250 50  0001 C CNN
F 1 "+3.3V" H 10065 1573 50  0000 C CNN
F 2 "" H 10050 1400 50  0001 C CNN
F 3 "" H 10050 1400 50  0001 C CNN
	1    10050 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 1550 10200 1500
Wire Wire Line
	10200 1500 10050 1500
Wire Wire Line
	10050 1500 10050 1400
Wire Wire Line
	9950 1550 9950 1500
Wire Wire Line
	9950 1500 10050 1500
Connection ~ 10050 1500
Wire Wire Line
	10300 2100 10200 2100
Wire Wire Line
	10200 1850 10200 2100
Wire Wire Line
	9750 2100 10200 2100
Connection ~ 10200 2100
Wire Wire Line
	10300 2200 9950 2200
Wire Wire Line
	9950 1850 9950 2200
Wire Wire Line
	9750 2200 9950 2200
Connection ~ 9950 2200
$EndSCHEMATC
