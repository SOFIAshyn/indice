# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\User\Documents\PSoC Creator\Workspace06\Design01.cydsn\Design01.cyprj
<<<<<<< HEAD
# Date: Fri, 10 Jan 2020 07:55:41 GMT
#set_units -time ns
create_clock -name {Clock_1(FFB)} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/ff_div_7}] [get_pins {ClockBlock/ff_div_8}]]
=======
# Date: Tue, 24 Dec 2019 14:17:44 GMT
#set_units -time ns
create_clock -name {Clock_1(FFB)} -period 166.66666666666666 -waveform {0 83.3333333333333} [list [get_pins {ClockBlock/ff_div_7}] [get_pins {ClockBlock/ff_div_8}]]
>>>>>>> 2a53f4dd5b08b7201acfd3c35e4b74087265cf45
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
<<<<<<< HEAD
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 5 9} [list]
=======
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 9 17} [list]
>>>>>>> 2a53f4dd5b08b7201acfd3c35e4b74087265cf45


# Component constraints for C:\Users\User\Documents\PSoC Creator\Workspace06\Design01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\User\Documents\PSoC Creator\Workspace06\Design01.cydsn\Design01.cyprj
<<<<<<< HEAD
# Date: Fri, 10 Jan 2020 07:55:38 GMT
=======
# Date: Tue, 24 Dec 2019 14:17:42 GMT
>>>>>>> 2a53f4dd5b08b7201acfd3c35e4b74087265cf45
