# EECE4632 FPGA Frequency Hopping Project
This project will compare the performance of a frequency hopping receiver when implemented using hardware vs software.

# Final Pipeline

## Input Files
All input files required to run the pipeline are included in the GitHub repository:
- `encrypted_packets.bin` — the packetized, encrypted signal produced by the transmitter pipeline
- `input.wav` — the original audio file used as the transmitter input

No additional downloads are needed. Clone the repo and all inputs will be present.

## How to Run

### Software Pipeline (PS)
1. Open `final_project_pipeline.ipynb` in JupyterLab
2. Run all cells in order. The notebook will:
   - Load the `.wav` file and generate a pseudo-random carrier signal
   - Modulate and packetize the signal into `encrypted_packets.bin`
   - Demodulate the packets and reconstruct the audio
   - Plot the original, encrypted, and decrypted waveforms and compute MSE

### Hardware Pipeline (PL)
Transfer the following files to the same directory on your Pynq board:
- `demodulate_axistream.bit`
- `demodulate_axistream.hwh`
- 'demodulate_axistream.tcl'
- `encrypted_packets.bin`

## AI Usage
We used **ChatGPT** during this project for the following tasks:
- Debugging Vitis HLS code, particularly around AXI stream interface pragmas and data type handling
- Troubleshooting the Jupyter notebook when the DMA was not writing to hardware correctly
- Helping structure and proofread sections of this report

Where AI suggestions were used in code, we reviewed and tested them on hardware ourselves — several suggestions required correction before they worked in our specific Pynq environment. The overall design decisions (single DMA architecture, bit-packing scheme, MSE validation approach) were made by us independently.

**Camille** wrote the Vitis code for the axi_stream, and the rest of the work was completed by **Liam and Camille** during lab time.

# Update Two: Hardware Implementation
This document discusses the changes made in update two of the project, for our hardware implementation.

## Vitis Implementation:
We used axi_stream to implement the demodulation function of our FHSS pipeline. The C++ file takes in an input signal, from the encrypted_packets.bin file; a carrier, which has the values by which to mulitiply the input signal; and the output signal, which is where the final, demodulated signal comes from. We wrote a testbench to ensure the code worked properly, then exported it to Vivado.

## Vivado Implementation:
We added the IP for our demodulate block to the axi_stream.tcl example from the earlier lab. The main modification we had to make was adding a second dma block, since our demodulate block has 2 inputs. We used the outputs of both dma blocks, for our input signal and carrier, as well as the input of one block, for the output signal. We generated our bitstream and used this code for our Jupyter notebook.

## Jupyter Notebook:
We tried to create a Jupyter notebook where we could get any sort of output from our demodulate block. However, we ran into a problem where our hardware wasn't being written to at all, which we were still trying to debug at the end of class on Wednesday, 3/25. We plan to diagnose this problem, but we suspect it is an issue with introducing the second dma block, and something possibly with our registers. It may be worthwhile to create a new Vivado design from scratch using the previous axi_stream example in case we corrupted memory addresses in the design.

## Files:
1. demodulate.cpp - The .cpp file written in Vitis, which we created an IP for in Vivado
2. encrypted_packets.bin - The input for the FHSS demodulate block to decrypt
3. demodulate_axistream.bit - The bitstream for our vivado implementation
4. demodulate_axistream.tcl - The .tcl diagram for our Vivado design
5. demodulate_axistream.hwh - The .hwh file for our Vivado design
6. demodulate_axistream.ipynb - The Jupyter notebook where we run our code using the implemented hardware
7. demodulate_axistream_bd.pdf - The pdf of our Vivado block diagram design

## Next Steps: 
A potential fix may be in our Jupyter notebook. We wrote **ol.demodulate_0.write(0x00, 0x01)**. After re-examining the tutorial, we should have written **ol.demodulate_0.write(0x00, 0x81)**, as AUTO_RESTART is not enabled otherwise, which would explain why our program stalled.
