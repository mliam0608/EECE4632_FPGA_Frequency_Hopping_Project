# Update Two
This document discusses the changes made in update two of the project, for our hardware implementation.

## Vitis Implementation:
We used axi_stream to implement the demodulation function of our FHSS pipeline. The C++ file takes in an input signal, from the encrypted_packets.bin file; a carrier, which has the values by which to mulitiply the input signal; and the output signal, which is where the final, demodulated signal comes from. We wrote a testbench to ensure the code worked properly, then exported it to Vivado.

## Vivado Implementation:
We added the IP for our demodulate block to the axi_stream.tcl example from the earlier lab. The main modification we had to make was adding a second dma block, since our demodulate block has 2 inputs. We used the outputs of both dma blocks, for our input signal and carrier, as well as the input of one block, for the output signal. We generated our bitstream and used this code for our Jupyter notebook.

## Jupyter Notebook:
We tried to create a Jupyter notebook where we could get any sort of output from our demodulate block. However, we ran into a problem where ... , which we were still trying to debug at the end of class on Wednesday, 3/25. We plan to diagnose this problem, but we suspect it is an issue with introducing the second dma block. It may be worthwhile to create a new Vivado design from scratch using the previous axi_stream example in case we corrupted memory addresses in the design.

## Files:
1. demodulate.cpp - The .cpp file written in Vitis, which we created an IP for in Vivado
2. encrypted_packets.bin - The input for the FHSS demodulate block to decrypt
3. demodulate_axistream.bit - The bitstream for our vivado implementation
4. demodulate_axistream.tcl - The .tcl diagram for our Vivado design
5. demodulate_axistream.hwh - The .hwh file for our Vivado design
6. demodulate_axistream.ipynb - The Jupyter notebook where we run our code using the implemented hardware
7. demodulate_axistream_bd.pdf - The pdf of our Vivado block diagram design

## Next Steps: 
We want to try to regenerate our Vivado design to see if we can fix the problem with ... . A major obstacle we ran into was the amount of time it takes to generate a bitstream, which limited our progress.