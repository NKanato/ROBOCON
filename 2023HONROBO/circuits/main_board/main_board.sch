EESchema Schematic File Version 4
EELAYER 30 0
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
L nucleo_f446re:Nucleo_f446RE U?
U 1 1 645E14EF
P 2900 1100
F 0 "U?" H 2900 1215 50  0000 C CNN
F 1 "Nucleo_f446RE" H 2900 1124 50  0000 C CNN
F 2 "" H 2900 1100 50  0001 C CNN
F 3 "" H 2900 1100 50  0001 C CNN
	1    2900 1100
	1    0    0    -1  
$EndComp
Wire Notes Line
	5400 500  5400 7800
Wire Notes Line
	5400 4700 500  4700
$Comp
L Xbee:xbeepros1 U?
U 1 1 645E9FE6
P 950 5800
F 0 "U?" H 1400 6015 50  0000 C CNN
F 1 "xbeepros1" H 1400 5924 50  0000 C CNN
F 2 "" H 950 5800 50  0001 C CNN
F 3 "" H 950 5800 50  0001 C CNN
	1    950  5800
	1    0    0    -1  
$EndComp
$Comp
L ESP32-30Pin:ESP32_3-Pin U?
U 1 1 645F8765
P 2950 5350
F 0 "U?" H 2950 5465 50  0000 C CNN
F 1 "ESP32_3-Pin" H 2950 5374 50  0000 C CNN
F 2 "" H 2950 5350 50  0001 C CNN
F 3 "" H 2950 5350 50  0001 C CNN
	1    2950 5350
	1    0    0    -1  
$EndComp
Text GLabel 1500 2000 0    50   Input ~ 0
PS3_TX
Text GLabel 2300 2000 2    50   Input ~ 0
PS3_RX
Text GLabel 950  6150 0    50   Input ~ 0
PS3_TX
Text GLabel 950  6050 0    50   Input ~ 0
PS3_RX
Text GLabel 950  5950 0    50   Input ~ 0
3vOut
Text GLabel 2300 2700 2    50   Input ~ 0
3vOut
Text GLabel 2300 2800 2    50   Input ~ 0
5vOut
Text GLabel 3500 3000 0    50   Input ~ 0
ESP32_TX
Text GLabel 3500 3600 0    50   Input ~ 0
ESP32_RX
Text GLabel 3500 6650 2    50   Input ~ 0
ESP32_TX
Text GLabel 3500 6550 2    50   Input ~ 0
ESP32_RX
$Comp
L power:GND #PWR0101
U 1 1 645FCDC4
P 2500 2900
F 0 "#PWR0101" H 2500 2650 50  0001 C CNN
F 1 "GND" H 2505 2727 50  0000 C CNN
F 2 "" H 2500 2900 50  0001 C CNN
F 3 "" H 2500 2900 50  0001 C CNN
	1    2500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2900 2500 2900
Wire Wire Line
	950  5850 600  5850
$Comp
L power:GND #PWR0102
U 1 1 645FDD11
P 600 5850
F 0 "#PWR0102" H 600 5600 50  0001 C CNN
F 1 "GND" H 605 5677 50  0000 C CNN
F 2 "" H 600 5850 50  0001 C CNN
F 3 "" H 600 5850 50  0001 C CNN
	1    600  5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 645FE03B
P 2200 7050
F 0 "#PWR0103" H 2200 6800 50  0001 C CNN
F 1 "GND" H 2205 6877 50  0000 C CNN
F 2 "" H 2200 7050 50  0001 C CNN
F 3 "" H 2200 7050 50  0001 C CNN
	1    2200 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 7050 2200 7050
$Comp
L Interface_CAN_LIN:MCP2561-E-MF U?
U 1 1 645FF36A
P 4650 6000
F 0 "U?" H 4650 6581 50  0000 C CNN
F 1 "MCP2561-E-MF" H 4650 6490 50  0000 C CNN
F 2 "Package_DFN_QFN:DFN-8-1EP_3x3mm_P0.65mm_EP1.55x2.4mm" H 4650 5500 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 4650 6000 50  0001 C CNN
	1    4650 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 646023D6
P 4000 6200
F 0 "#PWR0104" H 4000 5950 50  0001 C CNN
F 1 "GND" H 4005 6027 50  0000 C CNN
F 2 "" H 4000 6200 50  0001 C CNN
F 3 "" H 4000 6200 50  0001 C CNN
	1    4000 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6200 4000 6200
$Comp
L power:GND #PWR0105
U 1 1 64602EBB
P 4650 6400
F 0 "#PWR0105" H 4650 6150 50  0001 C CNN
F 1 "GND" H 4655 6227 50  0000 C CNN
F 2 "" H 4650 6400 50  0001 C CNN
F 3 "" H 4650 6400 50  0001 C CNN
	1    4650 6400
	1    0    0    -1  
$EndComp
Text Notes 4550 800  2    207  ~ 41
Main Microcontroller
Text Notes 9200 800  2    207  ~ 41
connectors
Text Notes 4650 5000 2    207  ~ 41
Communication Devices
Text GLabel 4300 2500 2    50   Input ~ 0
CAN1_TD
Text GLabel 4300 2600 2    50   Input ~ 0
CAN1_RD
Text GLabel 4150 5800 0    50   Input ~ 0
CAN1_TD
Text GLabel 4150 5900 0    50   Input ~ 0
CAN1_RD
Text GLabel 4650 5600 2    50   Input ~ 0
5vOut
Text GLabel 5150 5900 2    50   Input ~ 0
CAN_H
Text GLabel 5150 6100 2    50   Input ~ 0
CAN_L
$Comp
L Device:LED D?
U 1 1 64676B21
P 6700 4950
F 0 "D?" H 6693 5167 50  0000 C CNN
F 1 "LED" H 6693 5076 50  0000 C CNN
F 2 "" H 6700 4950 50  0001 C CNN
F 3 "~" H 6700 4950 50  0001 C CNN
	1    6700 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 64678B2F
P 7000 4950
F 0 "R?" V 7207 4950 50  0000 C CNN
F 1 "R" V 7116 4950 50  0000 C CNN
F 2 "" V 6930 4950 50  0001 C CNN
F 3 "~" H 7000 4950 50  0001 C CNN
	1    7000 4950
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_DP3T SW?
U 1 1 6467A021
P 10750 3550
F 0 "SW?" H 10750 3833 50  0000 C CNN
F 1 "SW_DP3T" H 10750 3742 50  0000 C CNN
F 2 "" H 10125 3725 50  0001 C CNN
F 3 "~" H 10125 3725 50  0001 C CNN
	1    10750 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 646877FB
P 10300 2800
F 0 "J?" H 10408 3081 50  0000 C CNN
F 1 "Conn_01x03_Male" H 10408 2990 50  0000 C CNN
F 2 "" H 10300 2800 50  0001 C CNN
F 3 "~" H 10300 2800 50  0001 C CNN
	1    10300 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 6468B3CB
P 10700 2900
F 0 "#PWR0106" H 10700 2650 50  0001 C CNN
F 1 "GND" H 10705 2727 50  0000 C CNN
F 2 "" H 10700 2900 50  0001 C CNN
F 3 "" H 10700 2900 50  0001 C CNN
	1    10700 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 2900 10700 2900
$Comp
L Connector:Conn_01x02_Male J?
U 1 1 6468BFBC
P 6550 1550
F 0 "J?" H 6658 1731 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6658 1640 50  0000 C CNN
F 2 "" H 6550 1550 50  0001 C CNN
F 3 "~" H 6550 1550 50  0001 C CNN
	1    6550 1550
	1    0    0    -1  
$EndComp
Text GLabel 6750 1550 2    50   Input ~ 0
CAN_H
Text GLabel 6750 1650 2    50   Input ~ 0
CAN_L
$Comp
L power:GND #PWR?
U 1 1 6468E92E
P 6400 4950
F 0 "#PWR?" H 6400 4700 50  0001 C CNN
F 1 "GND" H 6405 4777 50  0000 C CNN
F 2 "" H 6400 4950 50  0001 C CNN
F 3 "" H 6400 4950 50  0001 C CNN
	1    6400 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 4950 6400 4950
$Comp
L Device:LED D?
U 1 1 64691D3D
P 6700 5350
F 0 "D?" H 6693 5567 50  0000 C CNN
F 1 "LED" H 6693 5476 50  0000 C CNN
F 2 "" H 6700 5350 50  0001 C CNN
F 3 "~" H 6700 5350 50  0001 C CNN
	1    6700 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 64691D43
P 7000 5350
F 0 "R?" V 7207 5350 50  0000 C CNN
F 1 "R" V 7116 5350 50  0000 C CNN
F 2 "" V 6930 5350 50  0001 C CNN
F 3 "~" H 7000 5350 50  0001 C CNN
	1    7000 5350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64691D49
P 6400 5350
F 0 "#PWR?" H 6400 5100 50  0001 C CNN
F 1 "GND" H 6405 5177 50  0000 C CNN
F 2 "" H 6400 5350 50  0001 C CNN
F 3 "" H 6400 5350 50  0001 C CNN
	1    6400 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 5350 6400 5350
$Comp
L Device:LED D?
U 1 1 64692372
P 6700 5800
F 0 "D?" H 6693 6017 50  0000 C CNN
F 1 "LED" H 6693 5926 50  0000 C CNN
F 2 "" H 6700 5800 50  0001 C CNN
F 3 "~" H 6700 5800 50  0001 C CNN
	1    6700 5800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 64692378
P 7000 5800
F 0 "R?" V 7207 5800 50  0000 C CNN
F 1 "R" V 7116 5800 50  0000 C CNN
F 2 "" V 6930 5800 50  0001 C CNN
F 3 "~" H 7000 5800 50  0001 C CNN
	1    7000 5800
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6469237E
P 6400 5800
F 0 "#PWR?" H 6400 5550 50  0001 C CNN
F 1 "GND" H 6405 5627 50  0000 C CNN
F 2 "" H 6400 5800 50  0001 C CNN
F 3 "" H 6400 5800 50  0001 C CNN
	1    6400 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 5800 6400 5800
$Comp
L Device:LED D?
U 1 1 64698A76
P 8000 4900
F 0 "D?" H 7993 5117 50  0000 C CNN
F 1 "LED" H 7993 5026 50  0000 C CNN
F 2 "" H 8000 4900 50  0001 C CNN
F 3 "~" H 8000 4900 50  0001 C CNN
	1    8000 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 64698A7C
P 8300 4900
F 0 "R?" V 8507 4900 50  0000 C CNN
F 1 "R" V 8416 4900 50  0000 C CNN
F 2 "" V 8230 4900 50  0001 C CNN
F 3 "~" H 8300 4900 50  0001 C CNN
	1    8300 4900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64698A82
P 7700 4900
F 0 "#PWR?" H 7700 4650 50  0001 C CNN
F 1 "GND" H 7705 4727 50  0000 C CNN
F 2 "" H 7700 4900 50  0001 C CNN
F 3 "" H 7700 4900 50  0001 C CNN
	1    7700 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 4900 7700 4900
$Comp
L Device:LED D?
U 1 1 64698A89
P 8000 5300
F 0 "D?" H 7993 5517 50  0000 C CNN
F 1 "LED" H 7993 5426 50  0000 C CNN
F 2 "" H 8000 5300 50  0001 C CNN
F 3 "~" H 8000 5300 50  0001 C CNN
	1    8000 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 64698A8F
P 8300 5300
F 0 "R?" V 8507 5300 50  0000 C CNN
F 1 "R" V 8416 5300 50  0000 C CNN
F 2 "" V 8230 5300 50  0001 C CNN
F 3 "~" H 8300 5300 50  0001 C CNN
	1    8300 5300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64698A95
P 7700 5300
F 0 "#PWR?" H 7700 5050 50  0001 C CNN
F 1 "GND" H 7705 5127 50  0000 C CNN
F 2 "" H 7700 5300 50  0001 C CNN
F 3 "" H 7700 5300 50  0001 C CNN
	1    7700 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 5300 7700 5300
Text GLabel 2300 3300 2    50   Input ~ 0
LED1
Text GLabel 2300 3400 2    50   Input ~ 0
LED2
Text GLabel 2300 3500 2    50   Input ~ 0
LED3
Text GLabel 2300 3600 2    50   Input ~ 0
LED4
Text GLabel 2300 3700 2    50   Input ~ 0
LED5
Text GLabel 7150 4950 2    50   Input ~ 0
LED1
Text GLabel 7150 5350 2    50   Input ~ 0
LED2
Text GLabel 7150 5800 2    50   Input ~ 0
LED3
Text GLabel 8450 4900 2    50   Input ~ 0
LED4
Text GLabel 8450 5300 2    50   Input ~ 0
LED5
$EndSCHEMATC
