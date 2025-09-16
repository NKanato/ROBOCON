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
U 1 1 64686CC3
P 2850 1400
F 0 "U?" H 2850 1515 50  0000 C CNN
F 1 "Nucleo_f446RE" H 2850 1424 50  0000 C CNN
F 2 "" H 2850 1400 50  0001 C CNN
F 3 "" H 2850 1400 50  0001 C CNN
	1    2850 1400
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2561-E-MF U?
U 1 1 64689738
P 1500 6750
F 0 "U?" H 1500 7331 50  0000 C CNN
F 1 "MCP2561-E-MF" H 1500 7240 50  0000 C CNN
F 2 "Package_DFN_QFN:DFN-8-1EP_3x3mm_P0.65mm_EP1.55x2.4mm" H 1500 6250 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 1500 6750 50  0001 C CNN
	1    1500 6750
	1    0    0    -1  
$EndComp
Wire Notes Line
	5350 500  5350 7850
Wire Notes Line
	500  5000 5350 5000
Text Notes 1100 5450 0    207  ~ 41
Communication Devices
Text Notes 900  900  0    207  ~ 41
Andercarriage Controller
Text Notes 7500 900  0    207  ~ 41
Connectors
Text GLabel 4250 2300 2    50   Input ~ 0
Omni3_P
Text GLabel 4250 2400 2    50   Input ~ 0
Omni1_P
Text GLabel 4250 2800 2    50   Input ~ 0
CAN_TD
Text GLabel 4250 2900 2    50   Input ~ 0
CAN_RD
Text GLabel 4250 3300 2    50   Input ~ 0
Updown2_D
Text GLabel 4250 3400 2    50   Input ~ 0
Updown1_D
Text GLabel 4250 3900 2    50   Input ~ 0
Caterpillar_D
Text GLabel 3450 2300 0    50   Input ~ 0
Omni2_P
Text GLabel 3450 2400 0    50   Input ~ 0
Omni2_D
Text GLabel 3450 2500 0    50   Input ~ 0
Omni3_D
Text GLabel 4250 2500 2    50   Input ~ 0
Omni1_D
Text GLabel 3450 3000 0    50   Input ~ 0
Omni4_D
Text GLabel 3450 3200 0    50   Input ~ 0
Omni4_P
Text GLabel 3450 3300 0    50   Input ~ 0
Updown2_P
Text GLabel 3450 3400 0    50   Input ~ 0
Updown1_P
Text GLabel 3450 3900 0    50   Input ~ 0
Caterpillar_P
$Comp
L power:GND #PWR?
U 1 1 6468D9A6
P 2400 3200
F 0 "#PWR?" H 2400 2950 50  0001 C CNN
F 1 "GND" H 2405 3027 50  0000 C CNN
F 2 "" H 2400 3200 50  0001 C CNN
F 3 "" H 2400 3200 50  0001 C CNN
	1    2400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3200 2400 3200
Text GLabel 2250 3000 2    50   Input ~ 0
3vOut
Text GLabel 2250 3100 2    50   Input ~ 0
5vOut
Text GLabel 1500 6350 2    50   Input ~ 0
5vOut
Text GLabel 1000 6550 0    50   Input ~ 0
CAN_TD
Text GLabel 1000 6650 0    50   Input ~ 0
CAN_RD
Text GLabel 2000 6650 2    50   Input ~ 0
CAN_H
Text GLabel 2000 6850 2    50   Input ~ 0
CAN_L
$Comp
L power:GND #PWR?
U 1 1 6468E7D4
P 1500 7150
F 0 "#PWR?" H 1500 6900 50  0001 C CNN
F 1 "GND" H 1505 6977 50  0000 C CNN
F 2 "" H 1500 7150 50  0001 C CNN
F 3 "" H 1500 7150 50  0001 C CNN
	1    1500 7150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6468EC38
P 800 6950
F 0 "#PWR?" H 800 6700 50  0001 C CNN
F 1 "GND" H 805 6777 50  0000 C CNN
F 2 "" H 800 6950 50  0001 C CNN
F 3 "" H 800 6950 50  0001 C CNN
	1    800  6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 6950 800  6950
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 6469107C
P 6500 1600
F 0 "J?" H 6608 1881 50  0000 C CNN
F 1 "Conn_01x03_Male" H 6608 1790 50  0000 C CNN
F 2 "" H 6500 1600 50  0001 C CNN
F 3 "~" H 6500 1600 50  0001 C CNN
	1    6500 1600
	1    0    0    -1  
$EndComp
Text GLabel 6700 1500 2    50   Input ~ 0
Omni4_D
Text GLabel 6700 1600 2    50   Input ~ 0
Omni4_P
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 646923BF
P 6500 2350
F 0 "J?" H 6608 2631 50  0000 C CNN
F 1 "Conn_01x03_Male" H 6608 2540 50  0000 C CNN
F 2 "" H 6500 2350 50  0001 C CNN
F 3 "~" H 6500 2350 50  0001 C CNN
	1    6500 2350
	1    0    0    -1  
$EndComp
Text GLabel 6700 2250 2    50   Input ~ 0
Omni3_D
Text GLabel 6700 2350 2    50   Input ~ 0
Omni3_P
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 64692EA3
P 6500 3100
F 0 "J?" H 6608 3381 50  0000 C CNN
F 1 "Conn_01x03_Male" H 6608 3290 50  0000 C CNN
F 2 "" H 6500 3100 50  0001 C CNN
F 3 "~" H 6500 3100 50  0001 C CNN
	1    6500 3100
	1    0    0    -1  
$EndComp
Text GLabel 6700 3000 2    50   Input ~ 0
Omni2_D
Text GLabel 6700 3100 2    50   Input ~ 0
Omni2_P
Text GLabel 6700 3700 2    50   Input ~ 0
Omni1_D
Text GLabel 6700 3800 2    50   Input ~ 0
Omni1_P
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 6469352B
P 6500 3800
F 0 "J?" H 6608 4081 50  0000 C CNN
F 1 "Conn_01x03_Male" H 6608 3990 50  0000 C CNN
F 2 "" H 6500 3800 50  0001 C CNN
F 3 "~" H 6500 3800 50  0001 C CNN
	1    6500 3800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 64695174
P 5750 3800
F 0 "J?" H 5858 4081 50  0000 C CNN
F 1 "Conn_01x03_Male" H 5858 3990 50  0000 C CNN
F 2 "" H 5750 3800 50  0001 C CNN
F 3 "~" H 5750 3800 50  0001 C CNN
	1    5750 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 646953E4
P 6850 1700
F 0 "#PWR?" H 6850 1450 50  0001 C CNN
F 1 "GND" H 6855 1527 50  0000 C CNN
F 2 "" H 6850 1700 50  0001 C CNN
F 3 "" H 6850 1700 50  0001 C CNN
	1    6850 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1700 6850 1700
$Comp
L power:GND #PWR?
U 1 1 64696850
P 6850 2450
F 0 "#PWR?" H 6850 2200 50  0001 C CNN
F 1 "GND" H 6855 2277 50  0000 C CNN
F 2 "" H 6850 2450 50  0001 C CNN
F 3 "" H 6850 2450 50  0001 C CNN
	1    6850 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 2450 6850 2450
$Comp
L power:GND #PWR?
U 1 1 64696BCD
P 6850 3200
F 0 "#PWR?" H 6850 2950 50  0001 C CNN
F 1 "GND" H 6855 3027 50  0000 C CNN
F 2 "" H 6850 3200 50  0001 C CNN
F 3 "" H 6850 3200 50  0001 C CNN
	1    6850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3200 6850 3200
$Comp
L power:GND #PWR?
U 1 1 64696DA0
P 6850 3900
F 0 "#PWR?" H 6850 3650 50  0001 C CNN
F 1 "GND" H 6855 3727 50  0000 C CNN
F 2 "" H 6850 3900 50  0001 C CNN
F 3 "" H 6850 3900 50  0001 C CNN
	1    6850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3900 6850 3900
$Comp
L power:GND #PWR?
U 1 1 64699E6E
P 6100 3900
F 0 "#PWR?" H 6100 3650 50  0001 C CNN
F 1 "GND" H 6105 3727 50  0000 C CNN
F 2 "" H 6100 3900 50  0001 C CNN
F 3 "" H 6100 3900 50  0001 C CNN
	1    6100 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3900 6100 3900
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 6469B2C1
P 7500 2350
F 0 "J?" H 7608 2631 50  0000 C CNN
F 1 "Conn_01x03_Male" H 7608 2540 50  0000 C CNN
F 2 "" H 7500 2350 50  0001 C CNN
F 3 "~" H 7500 2350 50  0001 C CNN
	1    7500 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6469B2C7
P 7850 2450
F 0 "#PWR?" H 7850 2200 50  0001 C CNN
F 1 "GND" H 7855 2277 50  0000 C CNN
F 2 "" H 7850 2450 50  0001 C CNN
F 3 "" H 7850 2450 50  0001 C CNN
	1    7850 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 2450 7850 2450
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 6469C4AE
P 7500 1600
F 0 "J?" H 7608 1881 50  0000 C CNN
F 1 "Conn_01x03_Male" H 7608 1790 50  0000 C CNN
F 2 "" H 7500 1600 50  0001 C CNN
F 3 "~" H 7500 1600 50  0001 C CNN
	1    7500 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6469C4B4
P 7850 1700
F 0 "#PWR?" H 7850 1450 50  0001 C CNN
F 1 "GND" H 7855 1527 50  0000 C CNN
F 2 "" H 7850 1700 50  0001 C CNN
F 3 "" H 7850 1700 50  0001 C CNN
	1    7850 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 1700 7850 1700
Text GLabel 7700 2250 2    50   Input ~ 0
Updown1_D
Text GLabel 7700 2350 2    50   Input ~ 0
Updown1_P
Text GLabel 7700 1500 2    50   Input ~ 0
Updown2_D
Text GLabel 7700 1600 2    50   Input ~ 0
Updown2_P
$Comp
L Connector:Conn_01x03_Male J?
U 1 1 6469E03C
P 7500 3100
F 0 "J?" H 7608 3381 50  0000 C CNN
F 1 "Conn_01x03_Male" H 7608 3290 50  0000 C CNN
F 2 "" H 7500 3100 50  0001 C CNN
F 3 "~" H 7500 3100 50  0001 C CNN
	1    7500 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6469E042
P 7850 3200
F 0 "#PWR?" H 7850 2950 50  0001 C CNN
F 1 "GND" H 7855 3027 50  0000 C CNN
F 2 "" H 7850 3200 50  0001 C CNN
F 3 "" H 7850 3200 50  0001 C CNN
	1    7850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3200 7850 3200
Text GLabel 7700 3000 2    50   Input ~ 0
Caterpillar_D
Text GLabel 7700 3100 2    50   Input ~ 0
Caterpillar_P
$Comp
L Connector:Conn_01x02_Male J?
U 1 1 6469E8D3
P 8600 1500
F 0 "J?" H 8708 1681 50  0000 C CNN
F 1 "Conn_01x02_Male" H 8708 1590 50  0000 C CNN
F 2 "" H 8600 1500 50  0001 C CNN
F 3 "~" H 8600 1500 50  0001 C CNN
	1    8600 1500
	1    0    0    -1  
$EndComp
Text GLabel 8800 1500 2    50   Input ~ 0
CAN_H
Text GLabel 8800 1600 2    50   Input ~ 0
CAN_L
Text GLabel 3450 3100 0    50   Input ~ 0
Sonic1_echo
Text GLabel 3450 3500 0    50   Input ~ 0
Sonic1_trigger
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 6469ED42
P 9600 1550
F 0 "J?" H 9708 1831 50  0000 C CNN
F 1 "Conn_01x04_Male" H 9708 1740 50  0000 C CNN
F 2 "" H 9600 1550 50  0001 C CNN
F 3 "~" H 9600 1550 50  0001 C CNN
	1    9600 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 646A3DCC
P 10200 1650
F 0 "#PWR?" H 10200 1400 50  0001 C CNN
F 1 "GND" H 10205 1477 50  0000 C CNN
F 2 "" H 10200 1650 50  0001 C CNN
F 3 "" H 10200 1650 50  0001 C CNN
	1    10200 1650
	1    0    0    -1  
$EndComp
Text GLabel 9800 1750 2    50   Input ~ 0
5vOut
Wire Wire Line
	9800 1650 10200 1650
Text GLabel 9800 1450 2    50   Input ~ 0
Sonic1_trigger
Text GLabel 9800 1550 2    50   Input ~ 0
Sonic1_echo
$EndSCHEMATC
