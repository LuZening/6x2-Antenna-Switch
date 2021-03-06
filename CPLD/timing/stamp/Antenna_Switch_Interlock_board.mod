/*
 Copyright (C) 2018  Intel Corporation. All rights reserved.
 Your use of Intel Corporation's design tools, logic functions 
 and other software and tools, and its AMPP partner logic 
 functions, and any output files from any of the foregoing 
 (including device programming or simulation files), and any 
 associated documentation or information are expressly subject 
 to the terms and conditions of the Intel Program License 
 Subscription Agreement, the Intel Quartus Prime License Agreement,
 the Intel FPGA IP License Agreement, or other applicable license
 agreement, including, without limitation, that your use is for
 the sole purpose of programming logic devices manufactured by
 Intel and sold by Intel or its authorized distributors.  Please
 refer to the applicable agreement for further details.
*/
MODEL
/*MODEL HEADER*/
/*
 This file contains Slow Corner delays for the design using part EPM240T100C3
 with speed grade 3, core voltage Auto, and temperature 2147483647 Celsius

*/
MODEL_VERSION "1.0";
DESIGN "Antenna_Switch_Interlock";
DATE "11/21/2019 01:44:33";
PROGRAM "Quartus Prime";



INPUT REMOTE;
INPUT SW1[5];
INPUT SW1[0];
INPUT SW1[1];
INPUT SW1[2];
INPUT SW1[3];
INPUT SW1[4];
INPUT BCD1[0];
INPUT BCD1[2];
INPUT BCD1[1];
INPUT SW2[5];
INPUT SW2[0];
INPUT SW2[1];
INPUT SW2[2];
INPUT SW2[3];
INPUT SW2[4];
INPUT BCD2[0];
INPUT BCD2[2];
INPUT BCD2[1];
INPUT LOAD;
OUTPUT BCDM1[0];
OUTPUT BCDM1[1];
OUTPUT BCDM1[2];
OUTPUT BCDM2[0];
OUTPUT BCDM2[1];
OUTPUT BCDM2[2];
OUTPUT SEG1[0];
OUTPUT SEG1[1];
OUTPUT SEG1[2];
OUTPUT SEG1[3];
OUTPUT SEG1[4];
OUTPUT SEG1[5];
OUTPUT SEG1[6];
OUTPUT SEG1[7];
OUTPUT SEG1_en;
OUTPUT SEG2[0];
OUTPUT SEG2[1];
OUTPUT SEG2[2];
OUTPUT SEG2[3];
OUTPUT SEG2[4];
OUTPUT SEG2[5];
OUTPUT SEG2[6];
OUTPUT SEG2[7];
OUTPUT SEG2_en;
OUTPUT SEL1[0];
OUTPUT SEL1[1];
OUTPUT SEL1[2];
OUTPUT SEL1[3];
OUTPUT SEL1[4];
OUTPUT SEL1[5];
OUTPUT SEL2[0];
OUTPUT SEL2[1];
OUTPUT SEL2[2];
OUTPUT SEL2[3];
OUTPUT SEL2[4];
OUTPUT SEL2[5];

/*Arc definitions start here*/
pos_BCD1[0]__LOAD__setup:		SETUP (POSEDGE) BCD1[0] LOAD ;
pos_BCD1[1]__LOAD__setup:		SETUP (POSEDGE) BCD1[1] LOAD ;
pos_BCD1[2]__LOAD__setup:		SETUP (POSEDGE) BCD1[2] LOAD ;
pos_BCD2[0]__LOAD__setup:		SETUP (POSEDGE) BCD2[0] LOAD ;
pos_BCD2[1]__LOAD__setup:		SETUP (POSEDGE) BCD2[1] LOAD ;
pos_BCD2[2]__LOAD__setup:		SETUP (POSEDGE) BCD2[2] LOAD ;
pos_REMOTE__LOAD__setup:		SETUP (POSEDGE) REMOTE LOAD ;
pos_SW1[0]__LOAD__setup:		SETUP (POSEDGE) SW1[0] LOAD ;
pos_SW1[1]__LOAD__setup:		SETUP (POSEDGE) SW1[1] LOAD ;
pos_SW1[2]__LOAD__setup:		SETUP (POSEDGE) SW1[2] LOAD ;
pos_SW1[3]__LOAD__setup:		SETUP (POSEDGE) SW1[3] LOAD ;
pos_SW1[4]__LOAD__setup:		SETUP (POSEDGE) SW1[4] LOAD ;
pos_SW1[5]__LOAD__setup:		SETUP (POSEDGE) SW1[5] LOAD ;
pos_SW2[0]__LOAD__setup:		SETUP (POSEDGE) SW2[0] LOAD ;
pos_SW2[1]__LOAD__setup:		SETUP (POSEDGE) SW2[1] LOAD ;
pos_SW2[2]__LOAD__setup:		SETUP (POSEDGE) SW2[2] LOAD ;
pos_SW2[3]__LOAD__setup:		SETUP (POSEDGE) SW2[3] LOAD ;
pos_SW2[4]__LOAD__setup:		SETUP (POSEDGE) SW2[4] LOAD ;
pos_SW2[5]__LOAD__setup:		SETUP (POSEDGE) SW2[5] LOAD ;
pos_BCD1[0]__LOAD__hold:		HOLD (POSEDGE) BCD1[0] LOAD ;
pos_BCD1[1]__LOAD__hold:		HOLD (POSEDGE) BCD1[1] LOAD ;
pos_BCD1[2]__LOAD__hold:		HOLD (POSEDGE) BCD1[2] LOAD ;
pos_BCD2[0]__LOAD__hold:		HOLD (POSEDGE) BCD2[0] LOAD ;
pos_BCD2[1]__LOAD__hold:		HOLD (POSEDGE) BCD2[1] LOAD ;
pos_BCD2[2]__LOAD__hold:		HOLD (POSEDGE) BCD2[2] LOAD ;
pos_REMOTE__LOAD__hold:		HOLD (POSEDGE) REMOTE LOAD ;
pos_SW1[0]__LOAD__hold:		HOLD (POSEDGE) SW1[0] LOAD ;
pos_SW1[1]__LOAD__hold:		HOLD (POSEDGE) SW1[1] LOAD ;
pos_SW1[2]__LOAD__hold:		HOLD (POSEDGE) SW1[2] LOAD ;
pos_SW1[3]__LOAD__hold:		HOLD (POSEDGE) SW1[3] LOAD ;
pos_SW1[4]__LOAD__hold:		HOLD (POSEDGE) SW1[4] LOAD ;
pos_SW1[5]__LOAD__hold:		HOLD (POSEDGE) SW1[5] LOAD ;
pos_SW2[0]__LOAD__hold:		HOLD (POSEDGE) SW2[0] LOAD ;
pos_SW2[1]__LOAD__hold:		HOLD (POSEDGE) SW2[1] LOAD ;
pos_SW2[2]__LOAD__hold:		HOLD (POSEDGE) SW2[2] LOAD ;
pos_SW2[3]__LOAD__hold:		HOLD (POSEDGE) SW2[3] LOAD ;
pos_SW2[4]__LOAD__hold:		HOLD (POSEDGE) SW2[4] LOAD ;
pos_SW2[5]__LOAD__hold:		HOLD (POSEDGE) SW2[5] LOAD ;
pos_LOAD__BCDM1[0]__delay:		DELAY (POSEDGE) LOAD BCDM1[0] ;
pos_LOAD__BCDM1[1]__delay:		DELAY (POSEDGE) LOAD BCDM1[1] ;
pos_LOAD__BCDM1[2]__delay:		DELAY (POSEDGE) LOAD BCDM1[2] ;
pos_LOAD__BCDM2[0]__delay:		DELAY (POSEDGE) LOAD BCDM2[0] ;
pos_LOAD__BCDM2[1]__delay:		DELAY (POSEDGE) LOAD BCDM2[1] ;
pos_LOAD__BCDM2[2]__delay:		DELAY (POSEDGE) LOAD BCDM2[2] ;
pos_LOAD__SEG1[0]__delay:		DELAY (POSEDGE) LOAD SEG1[0] ;
pos_LOAD__SEG1[1]__delay:		DELAY (POSEDGE) LOAD SEG1[1] ;
pos_LOAD__SEG1[2]__delay:		DELAY (POSEDGE) LOAD SEG1[2] ;
pos_LOAD__SEG1[3]__delay:		DELAY (POSEDGE) LOAD SEG1[3] ;
pos_LOAD__SEG1[4]__delay:		DELAY (POSEDGE) LOAD SEG1[4] ;
pos_LOAD__SEG1[5]__delay:		DELAY (POSEDGE) LOAD SEG1[5] ;
pos_LOAD__SEG1[6]__delay:		DELAY (POSEDGE) LOAD SEG1[6] ;
pos_LOAD__SEG1[7]__delay:		DELAY (POSEDGE) LOAD SEG1[7] ;
pos_LOAD__SEG1_en__delay:		DELAY (POSEDGE) LOAD SEG1_en ;
pos_LOAD__SEG2[0]__delay:		DELAY (POSEDGE) LOAD SEG2[0] ;
pos_LOAD__SEG2[1]__delay:		DELAY (POSEDGE) LOAD SEG2[1] ;
pos_LOAD__SEG2[2]__delay:		DELAY (POSEDGE) LOAD SEG2[2] ;
pos_LOAD__SEG2[3]__delay:		DELAY (POSEDGE) LOAD SEG2[3] ;
pos_LOAD__SEG2[4]__delay:		DELAY (POSEDGE) LOAD SEG2[4] ;
pos_LOAD__SEG2[5]__delay:		DELAY (POSEDGE) LOAD SEG2[5] ;
pos_LOAD__SEG2[6]__delay:		DELAY (POSEDGE) LOAD SEG2[6] ;
pos_LOAD__SEG2[7]__delay:		DELAY (POSEDGE) LOAD SEG2[7] ;
pos_LOAD__SEG2_en__delay:		DELAY (POSEDGE) LOAD SEG2_en ;
pos_LOAD__SEL1[0]__delay:		DELAY (POSEDGE) LOAD SEL1[0] ;
pos_LOAD__SEL1[1]__delay:		DELAY (POSEDGE) LOAD SEL1[1] ;
pos_LOAD__SEL1[2]__delay:		DELAY (POSEDGE) LOAD SEL1[2] ;
pos_LOAD__SEL1[3]__delay:		DELAY (POSEDGE) LOAD SEL1[3] ;
pos_LOAD__SEL1[4]__delay:		DELAY (POSEDGE) LOAD SEL1[4] ;
pos_LOAD__SEL1[5]__delay:		DELAY (POSEDGE) LOAD SEL1[5] ;
pos_LOAD__SEL2[0]__delay:		DELAY (POSEDGE) LOAD SEL2[0] ;
pos_LOAD__SEL2[1]__delay:		DELAY (POSEDGE) LOAD SEL2[1] ;
pos_LOAD__SEL2[2]__delay:		DELAY (POSEDGE) LOAD SEL2[2] ;
pos_LOAD__SEL2[3]__delay:		DELAY (POSEDGE) LOAD SEL2[3] ;
pos_LOAD__SEL2[4]__delay:		DELAY (POSEDGE) LOAD SEL2[4] ;
pos_LOAD__SEL2[5]__delay:		DELAY (POSEDGE) LOAD SEL2[5] ;
___15.499__delay:		DELAY  15.499 ;
___15.847__delay:		DELAY  15.847 ;
___14.928__delay:		DELAY  14.928 ;
___16.177__delay:		DELAY  16.177 ;
___16.232__delay:		DELAY  16.232 ;
___14.432__delay:		DELAY  14.432 ;
___23.011__delay:		DELAY  23.011 ;
___23.175__delay:		DELAY  23.175 ;
___22.994__delay:		DELAY  22.994 ;
___23.027__delay:		DELAY  23.027 ;
___23.096__delay:		DELAY  23.096 ;
___23.152__delay:		DELAY  23.152 ;
___23.574__delay:		DELAY  23.574 ;
___21.252__delay:		DELAY  21.252 ;
___21.368__delay:		DELAY  21.368 ;
___23.500__delay:		DELAY  23.500 ;
___23.419__delay:		DELAY  23.419 ;
___23.346__delay:		DELAY  23.346 ;
___22.675__delay:		DELAY  22.675 ;
___22.678__delay:		DELAY  22.678 ;
___23.085__delay:		DELAY  23.085 ;
___22.675__delay:		DELAY  22.675 ;
___20.746__delay:		DELAY  20.746 ;
___21.725__delay:		DELAY  21.725 ;
___12.733__delay:		DELAY  12.733 ;
___12.669__delay:		DELAY  12.669 ;
___13.003__delay:		DELAY  13.003 ;
___13.183__delay:		DELAY  13.183 ;
___12.996__delay:		DELAY  12.996 ;
___12.626__delay:		DELAY  12.626 ;
___13.865__delay:		DELAY  13.865 ;
___13.600__delay:		DELAY  13.600 ;
___14.174__delay:		DELAY  14.174 ;
___12.713__delay:		DELAY  12.713 ;
___13.455__delay:		DELAY  13.455 ;
___13.142__delay:		DELAY  13.142 ;
___13.259__delay:		DELAY  13.259 ;
___13.607__delay:		DELAY  13.607 ;
___12.688__delay:		DELAY  12.688 ;
___13.937__delay:		DELAY  13.937 ;
___13.992__delay:		DELAY  13.992 ;
___12.192__delay:		DELAY  12.192 ;
___20.771__delay:		DELAY  20.771 ;
___20.935__delay:		DELAY  20.935 ;
___20.754__delay:		DELAY  20.754 ;
___20.787__delay:		DELAY  20.787 ;
___20.856__delay:		DELAY  20.856 ;
___20.912__delay:		DELAY  20.912 ;
___21.334__delay:		DELAY  21.334 ;
___19.012__delay:		DELAY  19.012 ;
___19.128__delay:		DELAY  19.128 ;
___21.260__delay:		DELAY  21.260 ;
___21.179__delay:		DELAY  21.179 ;
___21.106__delay:		DELAY  21.106 ;
___20.435__delay:		DELAY  20.435 ;
___20.438__delay:		DELAY  20.438 ;
___20.845__delay:		DELAY  20.845 ;
___20.435__delay:		DELAY  20.435 ;
___18.506__delay:		DELAY  18.506 ;
___19.485__delay:		DELAY  19.485 ;
___10.493__delay:		DELAY  10.493 ;
___10.429__delay:		DELAY  10.429 ;
___10.763__delay:		DELAY  10.763 ;
___10.943__delay:		DELAY  10.943 ;
___10.756__delay:		DELAY  10.756 ;
___10.386__delay:		DELAY  10.386 ;
___11.625__delay:		DELAY  11.625 ;
___11.360__delay:		DELAY  11.360 ;
___11.934__delay:		DELAY  11.934 ;
___10.473__delay:		DELAY  10.473 ;
___11.215__delay:		DELAY  11.215 ;
___10.902__delay:		DELAY  10.902 ;
___15.205__delay:		DELAY  15.205 ;
___15.553__delay:		DELAY  15.553 ;
___14.634__delay:		DELAY  14.634 ;
___15.883__delay:		DELAY  15.883 ;
___15.938__delay:		DELAY  15.938 ;
___14.138__delay:		DELAY  14.138 ;
___22.717__delay:		DELAY  22.717 ;
___22.881__delay:		DELAY  22.881 ;
___22.700__delay:		DELAY  22.700 ;
___22.733__delay:		DELAY  22.733 ;
___22.802__delay:		DELAY  22.802 ;
___22.858__delay:		DELAY  22.858 ;
___23.280__delay:		DELAY  23.280 ;
___20.958__delay:		DELAY  20.958 ;
___21.074__delay:		DELAY  21.074 ;
___23.206__delay:		DELAY  23.206 ;
___23.125__delay:		DELAY  23.125 ;
___23.052__delay:		DELAY  23.052 ;
___22.381__delay:		DELAY  22.381 ;
___22.384__delay:		DELAY  22.384 ;
___22.791__delay:		DELAY  22.791 ;
___22.381__delay:		DELAY  22.381 ;
___20.452__delay:		DELAY  20.452 ;
___21.431__delay:		DELAY  21.431 ;
___12.439__delay:		DELAY  12.439 ;
___12.375__delay:		DELAY  12.375 ;
___12.709__delay:		DELAY  12.709 ;
___12.889__delay:		DELAY  12.889 ;
___12.702__delay:		DELAY  12.702 ;
___12.332__delay:		DELAY  12.332 ;
___13.571__delay:		DELAY  13.571 ;
___13.306__delay:		DELAY  13.306 ;
___13.880__delay:		DELAY  13.880 ;
___12.419__delay:		DELAY  12.419 ;
___13.161__delay:		DELAY  13.161 ;
___12.848__delay:		DELAY  12.848 ;
___15.903__delay:		DELAY  15.903 ;
___16.251__delay:		DELAY  16.251 ;
___15.332__delay:		DELAY  15.332 ;
___16.581__delay:		DELAY  16.581 ;
___16.636__delay:		DELAY  16.636 ;
___14.836__delay:		DELAY  14.836 ;
___23.415__delay:		DELAY  23.415 ;
___23.579__delay:		DELAY  23.579 ;
___23.398__delay:		DELAY  23.398 ;
___23.431__delay:		DELAY  23.431 ;
___23.500__delay:		DELAY  23.500 ;
___23.556__delay:		DELAY  23.556 ;
___23.978__delay:		DELAY  23.978 ;
___21.656__delay:		DELAY  21.656 ;
___21.772__delay:		DELAY  21.772 ;
___23.904__delay:		DELAY  23.904 ;
___23.823__delay:		DELAY  23.823 ;
___23.750__delay:		DELAY  23.750 ;
___23.079__delay:		DELAY  23.079 ;
___23.082__delay:		DELAY  23.082 ;
___23.489__delay:		DELAY  23.489 ;
___23.079__delay:		DELAY  23.079 ;
___21.150__delay:		DELAY  21.150 ;
___22.129__delay:		DELAY  22.129 ;
___13.137__delay:		DELAY  13.137 ;
___13.073__delay:		DELAY  13.073 ;
___13.407__delay:		DELAY  13.407 ;
___13.587__delay:		DELAY  13.587 ;
___13.400__delay:		DELAY  13.400 ;
___13.030__delay:		DELAY  13.030 ;
___14.269__delay:		DELAY  14.269 ;
___14.004__delay:		DELAY  14.004 ;
___14.578__delay:		DELAY  14.578 ;
___13.117__delay:		DELAY  13.117 ;
___13.859__delay:		DELAY  13.859 ;
___13.546__delay:		DELAY  13.546 ;
___14.544__delay:		DELAY  14.544 ;
___14.892__delay:		DELAY  14.892 ;
___13.973__delay:		DELAY  13.973 ;
___15.222__delay:		DELAY  15.222 ;
___15.277__delay:		DELAY  15.277 ;
___13.477__delay:		DELAY  13.477 ;
___22.056__delay:		DELAY  22.056 ;
___22.220__delay:		DELAY  22.220 ;
___22.039__delay:		DELAY  22.039 ;
___22.072__delay:		DELAY  22.072 ;
___22.141__delay:		DELAY  22.141 ;
___22.197__delay:		DELAY  22.197 ;
___22.619__delay:		DELAY  22.619 ;
___20.297__delay:		DELAY  20.297 ;
___20.413__delay:		DELAY  20.413 ;
___22.545__delay:		DELAY  22.545 ;
___22.464__delay:		DELAY  22.464 ;
___22.391__delay:		DELAY  22.391 ;
___21.720__delay:		DELAY  21.720 ;
___21.723__delay:		DELAY  21.723 ;
___22.130__delay:		DELAY  22.130 ;
___21.720__delay:		DELAY  21.720 ;
___19.791__delay:		DELAY  19.791 ;
___20.770__delay:		DELAY  20.770 ;
___11.778__delay:		DELAY  11.778 ;
___11.714__delay:		DELAY  11.714 ;
___12.048__delay:		DELAY  12.048 ;
___12.228__delay:		DELAY  12.228 ;
___12.041__delay:		DELAY  12.041 ;
___11.671__delay:		DELAY  11.671 ;
___12.910__delay:		DELAY  12.910 ;
___12.645__delay:		DELAY  12.645 ;
___13.219__delay:		DELAY  13.219 ;
___11.758__delay:		DELAY  11.758 ;
___12.500__delay:		DELAY  12.500 ;
___12.187__delay:		DELAY  12.187 ;
___16.061__delay:		DELAY  16.061 ;
___16.409__delay:		DELAY  16.409 ;
___15.490__delay:		DELAY  15.490 ;
___16.739__delay:		DELAY  16.739 ;
___16.794__delay:		DELAY  16.794 ;
___14.994__delay:		DELAY  14.994 ;
___23.573__delay:		DELAY  23.573 ;
___23.737__delay:		DELAY  23.737 ;
___23.556__delay:		DELAY  23.556 ;
___23.589__delay:		DELAY  23.589 ;
___23.658__delay:		DELAY  23.658 ;
___23.714__delay:		DELAY  23.714 ;
___24.136__delay:		DELAY  24.136 ;
___21.814__delay:		DELAY  21.814 ;
___21.930__delay:		DELAY  21.930 ;
___24.062__delay:		DELAY  24.062 ;
___23.981__delay:		DELAY  23.981 ;
___23.908__delay:		DELAY  23.908 ;
___23.237__delay:		DELAY  23.237 ;
___23.240__delay:		DELAY  23.240 ;
___23.647__delay:		DELAY  23.647 ;
___23.237__delay:		DELAY  23.237 ;
___21.308__delay:		DELAY  21.308 ;
___22.287__delay:		DELAY  22.287 ;
___13.295__delay:		DELAY  13.295 ;
___13.231__delay:		DELAY  13.231 ;
___13.565__delay:		DELAY  13.565 ;
___13.745__delay:		DELAY  13.745 ;
___13.558__delay:		DELAY  13.558 ;
___13.188__delay:		DELAY  13.188 ;
___14.427__delay:		DELAY  14.427 ;
___14.162__delay:		DELAY  14.162 ;
___14.736__delay:		DELAY  14.736 ;
___13.275__delay:		DELAY  13.275 ;
___14.017__delay:		DELAY  14.017 ;
___13.704__delay:		DELAY  13.704 ;
___17.951__delay:		DELAY  17.951 ;
___18.299__delay:		DELAY  18.299 ;
___17.380__delay:		DELAY  17.380 ;
___18.629__delay:		DELAY  18.629 ;
___18.684__delay:		DELAY  18.684 ;
___16.884__delay:		DELAY  16.884 ;
___25.463__delay:		DELAY  25.463 ;
___25.627__delay:		DELAY  25.627 ;
___25.446__delay:		DELAY  25.446 ;
___25.479__delay:		DELAY  25.479 ;
___25.548__delay:		DELAY  25.548 ;
___25.604__delay:		DELAY  25.604 ;
___26.026__delay:		DELAY  26.026 ;
___23.704__delay:		DELAY  23.704 ;
___23.820__delay:		DELAY  23.820 ;
___25.952__delay:		DELAY  25.952 ;
___25.871__delay:		DELAY  25.871 ;
___25.798__delay:		DELAY  25.798 ;
___25.127__delay:		DELAY  25.127 ;
___25.130__delay:		DELAY  25.130 ;
___25.537__delay:		DELAY  25.537 ;
___25.127__delay:		DELAY  25.127 ;
___23.198__delay:		DELAY  23.198 ;
___24.177__delay:		DELAY  24.177 ;
___15.185__delay:		DELAY  15.185 ;
___15.121__delay:		DELAY  15.121 ;
___15.455__delay:		DELAY  15.455 ;
___15.635__delay:		DELAY  15.635 ;
___15.448__delay:		DELAY  15.448 ;
___15.078__delay:		DELAY  15.078 ;
___16.317__delay:		DELAY  16.317 ;
___16.052__delay:		DELAY  16.052 ;
___16.626__delay:		DELAY  16.626 ;
___15.165__delay:		DELAY  15.165 ;
___15.907__delay:		DELAY  15.907 ;
___15.594__delay:		DELAY  15.594 ;
___16.610__delay:		DELAY  16.610 ;
___16.958__delay:		DELAY  16.958 ;
___16.039__delay:		DELAY  16.039 ;
___17.288__delay:		DELAY  17.288 ;
___17.343__delay:		DELAY  17.343 ;
___15.543__delay:		DELAY  15.543 ;
___24.122__delay:		DELAY  24.122 ;
___24.286__delay:		DELAY  24.286 ;
___24.105__delay:		DELAY  24.105 ;
___24.138__delay:		DELAY  24.138 ;
___24.207__delay:		DELAY  24.207 ;
___24.263__delay:		DELAY  24.263 ;
___24.685__delay:		DELAY  24.685 ;
___22.363__delay:		DELAY  22.363 ;
___22.479__delay:		DELAY  22.479 ;
___24.611__delay:		DELAY  24.611 ;
___24.530__delay:		DELAY  24.530 ;
___24.457__delay:		DELAY  24.457 ;
___23.786__delay:		DELAY  23.786 ;
___23.789__delay:		DELAY  23.789 ;
___24.196__delay:		DELAY  24.196 ;
___23.786__delay:		DELAY  23.786 ;
___21.857__delay:		DELAY  21.857 ;
___22.836__delay:		DELAY  22.836 ;
___13.844__delay:		DELAY  13.844 ;
___13.780__delay:		DELAY  13.780 ;
___14.114__delay:		DELAY  14.114 ;
___14.294__delay:		DELAY  14.294 ;
___14.107__delay:		DELAY  14.107 ;
___13.737__delay:		DELAY  13.737 ;
___14.976__delay:		DELAY  14.976 ;
___14.711__delay:		DELAY  14.711 ;
___15.285__delay:		DELAY  15.285 ;
___13.824__delay:		DELAY  13.824 ;
___14.566__delay:		DELAY  14.566 ;
___14.253__delay:		DELAY  14.253 ;
___16.701__delay:		DELAY  16.701 ;
___17.049__delay:		DELAY  17.049 ;
___16.130__delay:		DELAY  16.130 ;
___17.379__delay:		DELAY  17.379 ;
___17.434__delay:		DELAY  17.434 ;
___15.634__delay:		DELAY  15.634 ;
___24.213__delay:		DELAY  24.213 ;
___24.377__delay:		DELAY  24.377 ;
___24.196__delay:		DELAY  24.196 ;
___24.229__delay:		DELAY  24.229 ;
___24.298__delay:		DELAY  24.298 ;
___24.354__delay:		DELAY  24.354 ;
___24.776__delay:		DELAY  24.776 ;
___22.454__delay:		DELAY  22.454 ;
___22.570__delay:		DELAY  22.570 ;
___24.702__delay:		DELAY  24.702 ;
___24.621__delay:		DELAY  24.621 ;
___24.548__delay:		DELAY  24.548 ;
___23.877__delay:		DELAY  23.877 ;
___23.880__delay:		DELAY  23.880 ;
___24.287__delay:		DELAY  24.287 ;
___23.877__delay:		DELAY  23.877 ;
___21.948__delay:		DELAY  21.948 ;
___22.927__delay:		DELAY  22.927 ;
___13.935__delay:		DELAY  13.935 ;
___13.871__delay:		DELAY  13.871 ;
___14.205__delay:		DELAY  14.205 ;
___14.385__delay:		DELAY  14.385 ;
___14.198__delay:		DELAY  14.198 ;
___13.828__delay:		DELAY  13.828 ;
___15.067__delay:		DELAY  15.067 ;
___14.802__delay:		DELAY  14.802 ;
___15.376__delay:		DELAY  15.376 ;
___13.915__delay:		DELAY  13.915 ;
___14.657__delay:		DELAY  14.657 ;
___14.344__delay:		DELAY  14.344 ;
___16.593__delay:		DELAY  16.593 ;
___16.941__delay:		DELAY  16.941 ;
___16.022__delay:		DELAY  16.022 ;
___17.271__delay:		DELAY  17.271 ;
___17.326__delay:		DELAY  17.326 ;
___15.526__delay:		DELAY  15.526 ;
___24.105__delay:		DELAY  24.105 ;
___24.269__delay:		DELAY  24.269 ;
___24.088__delay:		DELAY  24.088 ;
___24.121__delay:		DELAY  24.121 ;
___24.190__delay:		DELAY  24.190 ;
___24.246__delay:		DELAY  24.246 ;
___24.668__delay:		DELAY  24.668 ;
___22.346__delay:		DELAY  22.346 ;
___22.462__delay:		DELAY  22.462 ;
___24.594__delay:		DELAY  24.594 ;
___24.513__delay:		DELAY  24.513 ;
___24.440__delay:		DELAY  24.440 ;
___23.769__delay:		DELAY  23.769 ;
___23.772__delay:		DELAY  23.772 ;
___24.179__delay:		DELAY  24.179 ;
___23.769__delay:		DELAY  23.769 ;
___21.840__delay:		DELAY  21.840 ;
___22.819__delay:		DELAY  22.819 ;
___13.827__delay:		DELAY  13.827 ;
___13.763__delay:		DELAY  13.763 ;
___14.097__delay:		DELAY  14.097 ;
___14.277__delay:		DELAY  14.277 ;
___14.090__delay:		DELAY  14.090 ;
___13.720__delay:		DELAY  13.720 ;
___14.959__delay:		DELAY  14.959 ;
___14.694__delay:		DELAY  14.694 ;
___15.268__delay:		DELAY  15.268 ;
___13.807__delay:		DELAY  13.807 ;
___14.549__delay:		DELAY  14.549 ;
___14.236__delay:		DELAY  14.236 ;
___16.679__delay:		DELAY  16.679 ;
___17.027__delay:		DELAY  17.027 ;
___16.108__delay:		DELAY  16.108 ;
___17.357__delay:		DELAY  17.357 ;
___17.412__delay:		DELAY  17.412 ;
___15.612__delay:		DELAY  15.612 ;
___24.191__delay:		DELAY  24.191 ;
___24.355__delay:		DELAY  24.355 ;
___24.174__delay:		DELAY  24.174 ;
___24.207__delay:		DELAY  24.207 ;
___24.276__delay:		DELAY  24.276 ;
___24.332__delay:		DELAY  24.332 ;
___24.754__delay:		DELAY  24.754 ;
___22.432__delay:		DELAY  22.432 ;
___22.548__delay:		DELAY  22.548 ;
___24.680__delay:		DELAY  24.680 ;
___24.599__delay:		DELAY  24.599 ;
___24.526__delay:		DELAY  24.526 ;
___23.855__delay:		DELAY  23.855 ;
___23.858__delay:		DELAY  23.858 ;
___24.265__delay:		DELAY  24.265 ;
___23.855__delay:		DELAY  23.855 ;
___21.926__delay:		DELAY  21.926 ;
___22.905__delay:		DELAY  22.905 ;
___13.913__delay:		DELAY  13.913 ;
___13.849__delay:		DELAY  13.849 ;
___14.183__delay:		DELAY  14.183 ;
___14.363__delay:		DELAY  14.363 ;
___14.176__delay:		DELAY  14.176 ;
___13.806__delay:		DELAY  13.806 ;
___15.045__delay:		DELAY  15.045 ;
___14.780__delay:		DELAY  14.780 ;
___15.354__delay:		DELAY  15.354 ;
___13.893__delay:		DELAY  13.893 ;
___14.635__delay:		DELAY  14.635 ;
___14.322__delay:		DELAY  14.322 ;
___16.604__delay:		DELAY  16.604 ;
___16.952__delay:		DELAY  16.952 ;
___16.033__delay:		DELAY  16.033 ;
___17.282__delay:		DELAY  17.282 ;
___17.337__delay:		DELAY  17.337 ;
___15.537__delay:		DELAY  15.537 ;
___24.116__delay:		DELAY  24.116 ;
___24.280__delay:		DELAY  24.280 ;
___24.099__delay:		DELAY  24.099 ;
___24.132__delay:		DELAY  24.132 ;
___24.201__delay:		DELAY  24.201 ;
___24.257__delay:		DELAY  24.257 ;
___24.679__delay:		DELAY  24.679 ;
___22.357__delay:		DELAY  22.357 ;
___22.473__delay:		DELAY  22.473 ;
___24.605__delay:		DELAY  24.605 ;
___24.524__delay:		DELAY  24.524 ;
___24.451__delay:		DELAY  24.451 ;
___23.780__delay:		DELAY  23.780 ;
___23.783__delay:		DELAY  23.783 ;
___24.190__delay:		DELAY  24.190 ;
___23.780__delay:		DELAY  23.780 ;
___21.851__delay:		DELAY  21.851 ;
___22.830__delay:		DELAY  22.830 ;
___13.838__delay:		DELAY  13.838 ;
___13.774__delay:		DELAY  13.774 ;
___14.108__delay:		DELAY  14.108 ;
___14.288__delay:		DELAY  14.288 ;
___14.101__delay:		DELAY  14.101 ;
___13.731__delay:		DELAY  13.731 ;
___14.970__delay:		DELAY  14.970 ;
___14.705__delay:		DELAY  14.705 ;
___15.279__delay:		DELAY  15.279 ;
___13.818__delay:		DELAY  13.818 ;
___14.560__delay:		DELAY  14.560 ;
___14.247__delay:		DELAY  14.247 ;
___16.375__delay:		DELAY  16.375 ;
___16.723__delay:		DELAY  16.723 ;
___15.804__delay:		DELAY  15.804 ;
___17.053__delay:		DELAY  17.053 ;
___17.108__delay:		DELAY  17.108 ;
___15.308__delay:		DELAY  15.308 ;
___23.887__delay:		DELAY  23.887 ;
___24.051__delay:		DELAY  24.051 ;
___23.870__delay:		DELAY  23.870 ;
___23.903__delay:		DELAY  23.903 ;
___23.972__delay:		DELAY  23.972 ;
___24.028__delay:		DELAY  24.028 ;
___24.450__delay:		DELAY  24.450 ;
___22.128__delay:		DELAY  22.128 ;
___22.244__delay:		DELAY  22.244 ;
___24.376__delay:		DELAY  24.376 ;
___24.295__delay:		DELAY  24.295 ;
___24.222__delay:		DELAY  24.222 ;
___23.551__delay:		DELAY  23.551 ;
___23.554__delay:		DELAY  23.554 ;
___23.961__delay:		DELAY  23.961 ;
___23.551__delay:		DELAY  23.551 ;
___21.622__delay:		DELAY  21.622 ;
___22.601__delay:		DELAY  22.601 ;
___13.609__delay:		DELAY  13.609 ;
___13.545__delay:		DELAY  13.545 ;
___13.879__delay:		DELAY  13.879 ;
___14.059__delay:		DELAY  14.059 ;
___13.872__delay:		DELAY  13.872 ;
___13.502__delay:		DELAY  13.502 ;
___14.741__delay:		DELAY  14.741 ;
___14.476__delay:		DELAY  14.476 ;
___15.050__delay:		DELAY  15.050 ;
___13.589__delay:		DELAY  13.589 ;
___14.331__delay:		DELAY  14.331 ;
___14.018__delay:		DELAY  14.018 ;
___17.300__delay:		DELAY  17.300 ;
___17.648__delay:		DELAY  17.648 ;
___16.729__delay:		DELAY  16.729 ;
___17.978__delay:		DELAY  17.978 ;
___18.033__delay:		DELAY  18.033 ;
___16.233__delay:		DELAY  16.233 ;
___24.812__delay:		DELAY  24.812 ;
___24.976__delay:		DELAY  24.976 ;
___24.795__delay:		DELAY  24.795 ;
___24.828__delay:		DELAY  24.828 ;
___24.897__delay:		DELAY  24.897 ;
___24.953__delay:		DELAY  24.953 ;
___25.375__delay:		DELAY  25.375 ;
___23.053__delay:		DELAY  23.053 ;
___23.169__delay:		DELAY  23.169 ;
___25.301__delay:		DELAY  25.301 ;
___25.220__delay:		DELAY  25.220 ;
___25.147__delay:		DELAY  25.147 ;
___24.476__delay:		DELAY  24.476 ;
___24.479__delay:		DELAY  24.479 ;
___24.886__delay:		DELAY  24.886 ;
___24.476__delay:		DELAY  24.476 ;
___22.547__delay:		DELAY  22.547 ;
___23.526__delay:		DELAY  23.526 ;
___14.534__delay:		DELAY  14.534 ;
___14.470__delay:		DELAY  14.470 ;
___14.804__delay:		DELAY  14.804 ;
___14.984__delay:		DELAY  14.984 ;
___14.797__delay:		DELAY  14.797 ;
___14.427__delay:		DELAY  14.427 ;
___15.666__delay:		DELAY  15.666 ;
___15.401__delay:		DELAY  15.401 ;
___15.975__delay:		DELAY  15.975 ;
___14.514__delay:		DELAY  14.514 ;
___15.256__delay:		DELAY  15.256 ;
___14.943__delay:		DELAY  14.943 ;
___17.731__delay:		DELAY  17.731 ;
___18.079__delay:		DELAY  18.079 ;
___17.160__delay:		DELAY  17.160 ;
___18.409__delay:		DELAY  18.409 ;
___18.464__delay:		DELAY  18.464 ;
___16.664__delay:		DELAY  16.664 ;
___25.243__delay:		DELAY  25.243 ;
___25.407__delay:		DELAY  25.407 ;
___25.226__delay:		DELAY  25.226 ;
___25.259__delay:		DELAY  25.259 ;
___25.328__delay:		DELAY  25.328 ;
___25.384__delay:		DELAY  25.384 ;
___25.806__delay:		DELAY  25.806 ;
___23.484__delay:		DELAY  23.484 ;
___23.600__delay:		DELAY  23.600 ;
___25.732__delay:		DELAY  25.732 ;
___25.651__delay:		DELAY  25.651 ;
___25.578__delay:		DELAY  25.578 ;
___24.907__delay:		DELAY  24.907 ;
___24.910__delay:		DELAY  24.910 ;
___25.317__delay:		DELAY  25.317 ;
___24.907__delay:		DELAY  24.907 ;
___22.978__delay:		DELAY  22.978 ;
___23.957__delay:		DELAY  23.957 ;
___14.965__delay:		DELAY  14.965 ;
___14.901__delay:		DELAY  14.901 ;
___15.235__delay:		DELAY  15.235 ;
___15.415__delay:		DELAY  15.415 ;
___15.228__delay:		DELAY  15.228 ;
___14.858__delay:		DELAY  14.858 ;
___16.097__delay:		DELAY  16.097 ;
___15.832__delay:		DELAY  15.832 ;
___16.406__delay:		DELAY  16.406 ;
___14.945__delay:		DELAY  14.945 ;
___15.687__delay:		DELAY  15.687 ;
___15.374__delay:		DELAY  15.374 ;
___17.072__delay:		DELAY  17.072 ;
___17.420__delay:		DELAY  17.420 ;
___16.501__delay:		DELAY  16.501 ;
___17.750__delay:		DELAY  17.750 ;
___17.805__delay:		DELAY  17.805 ;
___16.005__delay:		DELAY  16.005 ;
___24.584__delay:		DELAY  24.584 ;
___24.748__delay:		DELAY  24.748 ;
___24.567__delay:		DELAY  24.567 ;
___24.600__delay:		DELAY  24.600 ;
___24.669__delay:		DELAY  24.669 ;
___24.725__delay:		DELAY  24.725 ;
___25.147__delay:		DELAY  25.147 ;
___22.825__delay:		DELAY  22.825 ;
___22.941__delay:		DELAY  22.941 ;
___25.073__delay:		DELAY  25.073 ;
___24.992__delay:		DELAY  24.992 ;
___24.919__delay:		DELAY  24.919 ;
___24.248__delay:		DELAY  24.248 ;
___24.251__delay:		DELAY  24.251 ;
___24.658__delay:		DELAY  24.658 ;
___24.248__delay:		DELAY  24.248 ;
___22.319__delay:		DELAY  22.319 ;
___23.298__delay:		DELAY  23.298 ;
___14.306__delay:		DELAY  14.306 ;
___14.242__delay:		DELAY  14.242 ;
___14.576__delay:		DELAY  14.576 ;
___14.756__delay:		DELAY  14.756 ;
___14.569__delay:		DELAY  14.569 ;
___14.199__delay:		DELAY  14.199 ;
___15.438__delay:		DELAY  15.438 ;
___15.173__delay:		DELAY  15.173 ;
___15.747__delay:		DELAY  15.747 ;
___14.286__delay:		DELAY  14.286 ;
___15.028__delay:		DELAY  15.028 ;
___14.715__delay:		DELAY  14.715 ;
___18.045__delay:		DELAY  18.045 ;
___18.393__delay:		DELAY  18.393 ;
___17.474__delay:		DELAY  17.474 ;
___18.723__delay:		DELAY  18.723 ;
___18.778__delay:		DELAY  18.778 ;
___16.978__delay:		DELAY  16.978 ;
___25.557__delay:		DELAY  25.557 ;
___25.721__delay:		DELAY  25.721 ;
___25.540__delay:		DELAY  25.540 ;
___25.573__delay:		DELAY  25.573 ;
___25.642__delay:		DELAY  25.642 ;
___25.698__delay:		DELAY  25.698 ;
___26.120__delay:		DELAY  26.120 ;
___23.798__delay:		DELAY  23.798 ;
___23.914__delay:		DELAY  23.914 ;
___26.046__delay:		DELAY  26.046 ;
___25.965__delay:		DELAY  25.965 ;
___25.892__delay:		DELAY  25.892 ;
___25.221__delay:		DELAY  25.221 ;
___25.224__delay:		DELAY  25.224 ;
___25.631__delay:		DELAY  25.631 ;
___25.221__delay:		DELAY  25.221 ;
___23.292__delay:		DELAY  23.292 ;
___24.271__delay:		DELAY  24.271 ;
___15.279__delay:		DELAY  15.279 ;
___15.215__delay:		DELAY  15.215 ;
___15.549__delay:		DELAY  15.549 ;
___15.729__delay:		DELAY  15.729 ;
___15.542__delay:		DELAY  15.542 ;
___15.172__delay:		DELAY  15.172 ;
___16.411__delay:		DELAY  16.411 ;
___16.146__delay:		DELAY  16.146 ;
___16.720__delay:		DELAY  16.720 ;
___15.259__delay:		DELAY  15.259 ;
___16.001__delay:		DELAY  16.001 ;
___15.688__delay:		DELAY  15.688 ;
___17.463__delay:		DELAY  17.463 ;
___17.811__delay:		DELAY  17.811 ;
___16.892__delay:		DELAY  16.892 ;
___18.141__delay:		DELAY  18.141 ;
___18.196__delay:		DELAY  18.196 ;
___16.396__delay:		DELAY  16.396 ;
___24.975__delay:		DELAY  24.975 ;
___25.139__delay:		DELAY  25.139 ;
___24.958__delay:		DELAY  24.958 ;
___24.991__delay:		DELAY  24.991 ;
___25.060__delay:		DELAY  25.060 ;
___25.116__delay:		DELAY  25.116 ;
___25.538__delay:		DELAY  25.538 ;
___23.216__delay:		DELAY  23.216 ;
___23.332__delay:		DELAY  23.332 ;
___25.464__delay:		DELAY  25.464 ;
___25.383__delay:		DELAY  25.383 ;
___25.310__delay:		DELAY  25.310 ;
___24.639__delay:		DELAY  24.639 ;
___24.642__delay:		DELAY  24.642 ;
___25.049__delay:		DELAY  25.049 ;
___24.639__delay:		DELAY  24.639 ;
___22.710__delay:		DELAY  22.710 ;
___23.689__delay:		DELAY  23.689 ;
___14.697__delay:		DELAY  14.697 ;
___14.633__delay:		DELAY  14.633 ;
___14.967__delay:		DELAY  14.967 ;
___15.147__delay:		DELAY  15.147 ;
___14.960__delay:		DELAY  14.960 ;
___14.590__delay:		DELAY  14.590 ;
___15.829__delay:		DELAY  15.829 ;
___15.564__delay:		DELAY  15.564 ;
___16.138__delay:		DELAY  16.138 ;
___14.677__delay:		DELAY  14.677 ;
___15.419__delay:		DELAY  15.419 ;
___15.106__delay:		DELAY  15.106 ;
___17.263__delay:		DELAY  17.263 ;
___17.611__delay:		DELAY  17.611 ;
___16.692__delay:		DELAY  16.692 ;
___17.941__delay:		DELAY  17.941 ;
___17.996__delay:		DELAY  17.996 ;
___16.196__delay:		DELAY  16.196 ;
___24.775__delay:		DELAY  24.775 ;
___24.939__delay:		DELAY  24.939 ;
___24.758__delay:		DELAY  24.758 ;
___24.791__delay:		DELAY  24.791 ;
___24.860__delay:		DELAY  24.860 ;
___24.916__delay:		DELAY  24.916 ;
___25.338__delay:		DELAY  25.338 ;
___23.016__delay:		DELAY  23.016 ;
___23.132__delay:		DELAY  23.132 ;
___25.264__delay:		DELAY  25.264 ;
___25.183__delay:		DELAY  25.183 ;
___25.110__delay:		DELAY  25.110 ;
___24.439__delay:		DELAY  24.439 ;
___24.442__delay:		DELAY  24.442 ;
___24.849__delay:		DELAY  24.849 ;
___24.439__delay:		DELAY  24.439 ;
___22.510__delay:		DELAY  22.510 ;
___23.489__delay:		DELAY  23.489 ;
___14.497__delay:		DELAY  14.497 ;
___14.433__delay:		DELAY  14.433 ;
___14.767__delay:		DELAY  14.767 ;
___14.947__delay:		DELAY  14.947 ;
___14.760__delay:		DELAY  14.760 ;
___14.390__delay:		DELAY  14.390 ;
___15.629__delay:		DELAY  15.629 ;
___15.364__delay:		DELAY  15.364 ;
___15.938__delay:		DELAY  15.938 ;
___14.477__delay:		DELAY  14.477 ;
___15.219__delay:		DELAY  15.219 ;
___14.906__delay:		DELAY  14.906 ;

ENDMODEL
