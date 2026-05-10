# EasyCharge - EV Charging Management System

A comprehensive management system for a nationwide electric vehicle (EV) charging network, developed as part of the "Advanced Topics in C" course (Spring 2025) at Ruppin Academic Center.

## Project Overview
EasyCharge is a modular C-based system designed to simulate and manage real-time EV charging operations. The system handles station databases, charging port availability, customer tracking, and automated billing.

## Architecture & Data Structures
The system implements several complex Abstract Data Types (ADTs) to ensure efficient data management:

* **Stations Database**: Managed using a **Binary Search Tree (BST)**, indexed by a unique Station ID.
* **Cars Database**: Managed using a **Binary Search Tree (BST)**, indexed by car license plates.
* **Charging Ports**: Each station maintains a **Single Linked List** of its ports (Fast, Mid, or Slow).
* **Waiting Queues**: Stations manage a **Queue** for vehicles waiting for an available port.

## Key Features
* **Dynamic Data Persistence**: Automatically loads system state from `.txt` files on startup and saves all updates upon exit.
* **Location Services**: Finds the nearest charging station based on user coordinates.
* **Automated Charging Logic**: Assigns cars to appropriate port types or manages their position in the waiting queue.
* **Billing System**: Calculates fees in real-time at a rate of **1.2 NIS per minute**.
* **Analytics**: Generates station utilization reports, tracks hardware failures, and identifies top-spending customers.

## Technical Implementation
* **Language**: C.
* **Memory Management**: Strict adherence to dynamic memory allocation and deallocation to prevent leaks.
* **Modularity**: Multi-file project structure separating ADT logic from system operations.
* **Standard Libraries**: Utilizes `time.h` for precise charging duration tracking.

## How to Run
1.  Ensure all data files (`Stations.txt`, `Ports.txt`, `Cars.txt`, `LineOfCars.txt`) are in the project directory.
2.  Build the project using **CMake**.
3.  Execute the compiled program to access the interactive user menu.

---
**Author**: Asaf Hayon