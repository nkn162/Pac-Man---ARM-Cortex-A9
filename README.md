# Pac-Man: Real-Time Game Engine on ARM Cortex-A9 (DE1-SoC)

## Project Overview

This repository contains the final source code for a fully playable, real-time Pac-Man implementation developed using Bare-Metal Embedded-C on the **ARM Cortex-A9** High Performance System (HPS) of the **Terasic DE1-SoC** development kit.

The project demonstrates expertise in low-level hardware control, constrained system design, gameplay coding, and the implementation of complex algorithms (like A* search) in a resource-limited environment.

---

## My Core Contributions

*This was a group project, completed as part of the Embedded Microprocessor System Design Module. My primary focus and intellectual contribution involved developing the **Player Control Subsystem** and managing the **System Integration**.*

| Feature Developed | Technical Skills Demonstrated | Relevant Files |
| :--- | :--- | :--- |
| **Real-Time Pac-Man Control Logic** | **Bare-Metal C**, Input Debouncing, **Finite State Machines** for movement, Continuous Motion Control. | `PacMan_Control.c/h` |
| **Precision Centering Algorithm** | **Fixed-Point Arithmetic**, Collision Detection (Read-ahead logic), Path Alignment at intersections. | `PacMan_Control.c/h`, `maze.c/h` |
| **Initial System Integration** | **Module Interfacing** (Pac-Man → Maze → UI), managing game state transitions, **LCD Rendering Pipeline** implementation. | `main.c`, UI files |
| **UI Asset Integration** | **Memory/Asset Management** for pre-rendered images/frames (e.g., menu, high-score, startup video). | `video_frames.c/h`, `menu_image.c/h` |

---

## Key Technologies & Implementation Stack

| Category | Details |
| :--- | :--- |
| **Target Hardware** | **Terasic DE1-SoC** (ARM Cortex-A9 HPS) |
| **Software Approach** | **Bare-Metal Embedded C** (No OS), custom low-level register access. |
| **Development IDE** | **ARM Development Studio (DS) 2022** (Build, Debug), ARM cross-toolchain. |
| **Display & Input** | **LT24 TFT LCD** (Rendering), DE1-SoC **Keys/Buttons** (User Input). |
| **Algorithms** | ****A* Search Pathfinding**** (for Ghost AI), Custom **Collision Detection** matrix. |

---

## Project Features & Technical Deep Dive

### 1. Game Mechanics and Logic
* **Collision System:** Efficient map-based collision detection and **path centering logic** ensures smooth player movement through maze intersections, resolving path ambiguity.
* **Ghost AI:** Four distinct ghost behaviours (e.g., direct pursuit, ambush) are implemented, leveraging the **A* search algorithm** for dynamic pathfinding across the maze graph.

### 2. Architecture & Assets
* The system utilizes a modular file structure, separating concerns into `maze` (layout, tile map), `PacMan_Control` (input, movement), `Ghost` (AI, rendering), and various UI asset files.
* **Asset Management Note:** Static UI screens (menu, help) and the animated startup video are stored as **Embedded C arrays** (e.g., `video_frames.c`), simplifying direct memory access for LCD drawing but optimizing for constrained memory.

---

## Group Roles and Responsibilities (for Context)

*The project's successful integration was a collaborative effort:*

* **Neeraj Nambudiri:** Pac-Man control, centering, movement logic, and integration.
* **Yahia Hassanin:** Ghost rendering, A* pathfinding implementation, and integration.
* **Avinash Nair:** Comprehensive maze design, maze rendering, and core collision detection utilities.
* **Aravindh Anil:** UI design, **Blender-rendered introductory video** implementation, and project documentation.
* **Harsh Sharma:** UI design, high score logic, image rendering for menu screens, image asset packaging, and trials for sound integration.

---

## Repository Structure

The key source files are organized as follows:

* `main.c`: Program entry point, game state machine, and initialization.
* `PacMan_Control.c/h`: Player movement, centering, and input processing.
* `Ghost.c/h`: Ghost AI, pathfinding, and movement logic.
* `maze.c/h`: Maze layout representation and low-level collision utilities.
* `video_frames.c/h`: Large C array containing startup video frame data.
* `Maze.pdf`: Visual representation of the final maze design.

## Build and Run (High Level)

1.  **Import:** Import the project folder into **ARM DS IDE** as an existing C/C++ project.
2.  **Configure:** Ensure the ARM cross-compiler toolchain is correctly configured for the **ARM Cortex-A9 HPS** target.
3.  **Deploy:** Deploy the generated binary to the DE1-SoC hardware (e.g., via JTAG, SD card, or HPS bootloader) for execution.

---

## Contact

* **Author:** Neeraj Nambudiri (`nkn162`)
* For detailed inquiries regarding my specific code contributions or the movement/centering algorithms, please contact me via the email associated with this GitHub account.
