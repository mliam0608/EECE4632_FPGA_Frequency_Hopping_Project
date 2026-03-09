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

# Compatibility with Liam’s Work

## What Liam’s code currently does

Liam’s code currently implements:
- a shared pseudo-random hop sequence
- server/client communication over sockets
- port hopping using the shared sequence
- repeated transfer of a WAV file

This supports the **communication and packet-transfer side** of the project.

---

## How my notebook and Liam’s code are currently related

The two parts are compatible at the **project level**, because they both rely on:
- a shared sequence between transmitter and receiver
- transferring or protecting audio
- building toward a full FHSS-style system

However, they are **not yet fully code-compatible**, because they operate at different layers.

### My notebook works at the signal-processing level:
- audio samples
- carrier multiplication
- modulation/demodulation
- waveform recovery

### Liam’s code works at the communication level:
- file bytes
- TCP socket transfer
- pseudo-random port hopping
- repeated file sending

So at the moment, the two implementations are connected conceptually, but not yet merged into one pipeline.

---

# What Needs to Change to Become Fully Compatible

To make the two implementations fully compatible, we need a shared interface between them.

## 1. Liam should send the modulated/encrypted file, not just the original WAV
Right now, Liam’s code sends the original WAV file repeatedly.

To connect with this notebook, the process should become:

1. This notebook loads `input.wav`
2. This notebook creates `encrypted.wav`
3. Liam’s code transmits `encrypted.wav`
4. The receiver stores the received encrypted file
5. This notebook demodulates the received file into `decrypted.wav`

This would make the two parts compatible at the file level.

---

## 2. The transfer should be packetized instead of sending the whole file every hop
Currently, Liam’s code sends the entire WAV file once per hop.

To better match the proposal, the file should be split into chunks or packets.

Each hop should send:
- one chunk of the modulated signal
- or a small group of chunks

Then the receiver should:
- store packet order
- reassemble packets
- reconstruct the full encrypted signal before demodulation

This would make the implementation much closer to an actual hopping communication system.

---

## 3. Packet metadata should be added
Each transmitted packet should eventually include information such as:
- packet index
- total number of packets
- payload size
- timestamp or hop index

This is necessary so that the receiver can correctly rebuild the file and track loss or corruption.

---

## 4. Packet loss / corruption handling should be added
The proposal mentions evaluating:
- packet loss rate
- successful demodulation
- throughput
- correctness under hopping conditions

To support this, the communication layer should eventually:
- detect missing packets
- optionally simulate dropped packets
- measure how much data was successfully received
- report whether the final recovered audio is complete

---

## 5. The hop sequence should be unified
My notebook currently supports:
- fixed carrier frequency
- a simple predefined hopping frequency list
- an optional LFSR starter example

Liam’s code currently uses:
- a Python PRNG with a shared seed to generate hop ports

To better match the project proposal, both sides should eventually use a **single shared pseudo-random scheme**, ideally based on an **LFSR-generated sequence**.

That sequence could then drive:
- carrier frequency hopping in the signal-processing notebook
- packet or communication hopping in the transfer implementation

This would make the transmitter and receiver synchronization story much cleaner.

---

## 6. The final receiver pipeline should be connected end-to-end
The fully integrated pipeline should look like this:

1. Load original audio
2. Modulate/encrypt audio
3. Split encrypted signal into packets
4. Transmit packets using a shared hopping sequence
5. Reassemble received encrypted signal
6. Demodulate/decrypt signal
7. Save recovered output
8. Compare original vs recovered signal

That is the final form needed for full compatibility.

---

# Recommended Next Integration Step

The simplest next step is:

## Short-term integration plan
- Keep my notebook as the modulation/demodulation stage
- Have my notebook generate `encrypted.wav`
- Change Liam’s sender so it transmits `encrypted.wav`
- Save the received file as something like `received_encrypted.wav`
- Add a notebook cell that loads `received_encrypted.wav` and demodulates it

This would give us a clean combined demo quickly.

---

# Summary

This notebook currently provides the **DSP/software prototype** for the project:
- audio input/output
- modulation
- demodulation
- recovery
- correctness metrics
- timing metrics

Liam’s code currently provides the **communication prototype**:
- shared-sequence hopping
- sender/receiver transfer
- groundwork for packetized communication

The two components support the same overall FHSS project, but to become fully compatible they still need:
- a shared file/packet interface
- packetization
- reconstruction of the received encrypted signal
- a unified pseudo-random hopping sequence
- end-to-end integration from modulation through transmission to recovery
