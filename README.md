# EECE4632_FPGA_Frequency_Hopping_Project
This project will compare the performance of a frequency hopping receiver when implemented using hardware vs software.

## Update One: Software Implementation:
This notebook is the **software-side signal processing prototype** for our EECE 4632 project on **Frequency Hopping Spread Spectrum (FHSS)**.

The purpose of this notebook is to show that we can:

1. Load or generate an audio signal
2. Modulate the signal using a carrier
3. Demodulate the signal using the same carrier
4. Recover the original audio in software
5. Measure correctness and timing
6. Extend the idea to a basic FHSS-style hopping carrier

This notebook focuses on the **audio modulation/demodulation portion** of the project, which is the part of the project primarily assigned to **Camille** in the proposal.

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

## What This Notebook Demonstrates

This notebook demonstrates the **core signal-processing idea** behind the project:

- the transmitter and receiver can share a known carrier pattern
- the audio can be modulated and later recovered
- the process works in software before moving to FPGA hardware

This makes the notebook a valid **first software milestone** for the project.

---

## Current Limitation

Although this notebook matches the **signal-processing side** of the project proposal, it is **not yet fully integrated** with Liam’s communication-layer implementation.

Right now, the notebook works entirely in Python on local arrays and files. It does not yet send packets over sockets or reconstruct a received signal from network transfers.

---
