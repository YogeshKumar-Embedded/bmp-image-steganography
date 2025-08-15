# BMP Image Steganography (C Project)

Hide and extract secret data inside BMP images using Least Significant Bit (LSB) steganography in C.


## 📌 Overview
A C-based project that hides and extracts secret data inside **24-bit BMP images** using **Least Significant Bit (LSB) steganography**.  
This simulates real-world data hiding techniques used in **digital forensics**, **watermarking**, and **secure communication**.

---

## 🎯 Features
- **Encode mode**: Embed text or file data into a BMP image without visibly changing it.
- **Decode mode**: Retrieve hidden data from a stego image.
- Works with **any 24-bit BMP image**.
- Validates hidden data using a **magic string**.
- Encodes:
  - File extension
  - File size
  - File content

---

## 🛠️ Tech Stack
- **Language**: C
- **Concepts**: File I/O, bitwise operations, data encoding/decoding
- **Platform**: Linux
- **Tools Used**: GCC compiler

---
