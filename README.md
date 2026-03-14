# EECE4632_FPGA_Frequency_Hopping_Project
This project will compare the performance of a frequency hopping receiver when implemented using hardware vs software.

## Update One: Software Implementation:
This notebook is the **software-side signal processing prototype** for our EECE 4632 project on **Frequency Hopping Spread Spectrum (FHSS)**.

The purpose of this notebook is to show that we can:

1. Load or generate an audio signal
2. Modulate the signal using a carrier
3. Demodulate the signal using the same carrier
4. Packetize the .wav data in UDP format and write it to a .bin file
5. Recover the original audio in software
6. Measure correctness and timing
7. Extend the idea to a basic FHSS-style hopping carrier
8. Implement a FHSS-style hopping carrier using and LSFR algorithm to add randomness

**Camille** focused on the **audio modulation/demodulation** portion of this update.
**Liam** focused on the **UDP packetization and pseudo-random FHSS implementation** for this portion of the update.


---

## What This Notebook Does

### 1. Loads an audio signal
The notebook either:
- reads an input `.wav` file, or
- generates a synthetic test signal if no file is available

This gives us a known audio source to process.

---

### 2. Creates a carrier signal
The notebook generates a carrier waveform using a cosine function.

It supports:
- a **single-frequency carrier** for the simplest proof of concept
- a **basic hopping carrier** where the frequency changes every fixed number of samples

---

### 3. Modulates the audio
The original audio is “encrypted” by multiplying it by the carrier signal.

For the basic version:

\[
\text{modulated} = \text{audio} \times \text{carrier}
\]

This shifts the signal into a higher-frequency form.

---

### 4. Demodulates the audio
The receiver multiplies the modulated signal by the **same carrier** again.

\[
\text{mixed\_back} = \text{modulated} \times \text{carrier}
\]

Then the notebook applies a **low-pass filter** to remove the high-frequency terms and recover the original baseband audio.

---

### 5. Saves output files
The notebook saves:
- an encrypted/modulated `.wav` file
- a decrypted/demodulated `.wav` file

This allows us to listen to the intermediate and final results.

---

### 6. Visualizes the results
The notebook plots:
- the original waveform
- the encrypted waveform
- the decrypted waveform

This helps verify that the output matches the input.

---

### 7. Evaluates correctness
The notebook calculates several metrics, including:
- **MSE** (mean squared error)
- **MAE** (mean absolute error)
- **SNR** (signal-to-noise ratio)
- **Approximate BER** (bit error rate approximation using sample sign)

These give us a software baseline for later comparison with FPGA hardware.

---

### 8. Measures software timing
The notebook measures:
- total execution time
- throughput in samples per second

These timing results are important because the long-term goal of the project is to compare **software (PS)** versus **hardware (PL)** performance.

---

### 9. Packetizes UDP data and writes it to a .bin file for future demodulation
The notebook writes modulated data to UDP packets:
- this format allows us to use the same input data for PS and PL
- this format allows us to effectively separate the transmitter-side and receiver-side 

We can use this UDP data to track packet loss on the same input to compare **software (PS)** versus **hardware (PL)**. 

---

### 10. Implements LSFR hop sequence generator
The notebook uses an extensible pseudo-random hop-sequence to create a more complex hop pattern:

Using this hop sequence allows us to simulate more realistic hop patterns, better demonstrating the abilities and limitations of **software (PS)** versus **hardware (PL)**. 

---

## What This Notebook Demonstrates

This notebook demonstrates the **core signal-processing idea** behind the project:

- the transmitter and receiver can share a known carrier pattern
- the audio can be modulated and later recovered
- the process works in software before moving to FPGA hardware

This makes the notebook a valid **first software milestone** for the project.

---

## Current Limitation

The notebook does not actually send packets over a socket, instead writing simulated UDP packets to a .bin file to represent the sending function, then the data is decrypted and read to represent the receiving function. Both the hardware and software implementations will use the same encrypted .bin file, and then both will decrypt and display the results to represent the receiver side.

Right now, the final FHSS demodulated signal is slighty distorted. We can investigate this issue further to determine if this distortion represents a problem in our code, or if it is to be expected from our filtering function.

---
