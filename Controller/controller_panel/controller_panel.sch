EESchema Schematic File Version 4
LIBS:controller_panel-cache
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
L Device:LED D1
U 1 1 5EB0DBE3
P 6500 3450
F 0 "D1" H 6493 3666 50  0000 C CNN
F 1 "LED" H 6493 3575 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 6500 3450 50  0001 C CNN
F 3 "~" H 6500 3450 50  0001 C CNN
	1    6500 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5EB0E466
P 6500 3850
F 0 "D2" H 6493 4066 50  0000 C CNN
F 1 "LED" H 6493 3975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 6500 3850 50  0001 C CNN
F 3 "~" H 6500 3850 50  0001 C CNN
	1    6500 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 5EB0ED53
P 6500 4250
F 0 "D3" H 6493 4466 50  0000 C CNN
F 1 "LED" H 6493 4375 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 6500 4250 50  0001 C CNN
F 3 "~" H 6500 4250 50  0001 C CNN
	1    6500 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 5EB0F4DC
P 7100 3450
F 0 "D4" H 7093 3666 50  0000 C CNN
F 1 "LED" H 7093 3575 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 7100 3450 50  0001 C CNN
F 3 "~" H 7100 3450 50  0001 C CNN
	1    7100 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D5
U 1 1 5EB0F4E6
P 7100 3850
F 0 "D5" H 7093 4066 50  0000 C CNN
F 1 "LED" H 7093 3975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 7100 3850 50  0001 C CNN
F 3 "~" H 7100 3850 50  0001 C CNN
	1    7100 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D6
U 1 1 5EB0F4F0
P 7100 4250
F 0 "D6" H 7093 4466 50  0000 C CNN
F 1 "LED" H 7093 4375 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 7100 4250 50  0001 C CNN
F 3 "~" H 7100 4250 50  0001 C CNN
	1    7100 4250
	1    0    0    -1  
$EndComp
$Comp
L LED:HDSP-4830_2 BAR1
U 1 1 5EAB7D6F
P 5550 3850
F 0 "BAR1" H 5550 3083 50  0000 C CNN
F 1 "HDSP-4830_2" H 5550 3174 50  0000 C CNN
F 2 "Display:HDSP-4830" H 5550 3050 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 3550 4050 50  0001 C CNN
	1    5550 3850
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5EAC5E40
P 6800 2600
F 0 "SW1" H 6800 2885 50  0000 C CNN
F 1 "SW_Push" H 6800 2794 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H8mm" H 6800 2800 50  0001 C CNN
F 3 "~" H 6800 2800 50  0001 C CNN
	1    6800 2600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5EAC67AB
P 7350 2600
F 0 "SW2" H 7350 2885 50  0000 C CNN
F 1 "SW_Push" H 7350 2794 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H8mm" H 7350 2800 50  0001 C CNN
F 3 "~" H 7350 2800 50  0001 C CNN
	1    7350 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3350 5350 3450
Wire Wire Line
	5350 3450 5350 3550
Connection ~ 5350 3450
Wire Wire Line
	5350 3550 5350 3650
Connection ~ 5350 3550
Wire Wire Line
	5350 3650 5350 3750
Connection ~ 5350 3650
Wire Wire Line
	5350 3750 5350 3850
Connection ~ 5350 3750
Wire Wire Line
	5350 3850 5350 3950
Connection ~ 5350 3850
Wire Wire Line
	5350 3950 5350 4050
Connection ~ 5350 3950
Wire Wire Line
	5350 4050 5350 4150
Connection ~ 5350 4050
Wire Wire Line
	5350 4150 5350 4250
Connection ~ 5350 4150
$EndSCHEMATC