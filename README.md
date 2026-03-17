---

# 🧠 Semester Project – C Programming

[🇵🇱 Polski](README.pl.md) | 🇬🇧 English

Project description in English goes here.

# Project Title: [Title, e.g., "Interactive Checkers in C"]

---

## 🧩 Project Topic

The project should align with the course scope. Example categories include:

- 🎮 games (e.g., Snake, 2048, Connect Four)
- 🗂 system applications (file manager, resource monitor)
- 📊 data processing applications
- ⚙️ embedded / sensor-based systems

👉 The final topic must be approved by the instructor.

---

## 📌 General Information

- **Course:** Low-Level Programming in C  
- **Instructor:** Dr. Eng. Serhii Baraban  
- **Academic Year:** 2025/2026  
- **Format:** Individual or pair project  

---

## Project Description:
An interactive, low-level programming project implemented entirely in C, featuring modular architecture, dynamic memory management, robust error handling, and a graphical user interface (GUI). This project complies with the specific academic requirements for a grade of 5.0, including structured and clear coding practices, effective use of pointers, structures, dynamic data structures, and file-based operations.

## Project Timeline:
- **Start Date:** March 17, 2026  
- **End Date:** June 15, 2026

---

## 🏗️ Functional Requirements

The project should:

- provide user interaction (keyboard or GUI)
- validate input data
- handle errors robustly
- support saving and loading data from files

---

## 💾 File Operations

The program must implement:

- saving application/game state
- logging or configuration files
- reading data from files

---

## 🧠 Project Architecture

The code should be modular and organized into components:

- `main.c` – entry point
- `logic.c` – core logic
- `io.c` – file operations
- `ui.c` – user interface

Required elements:

- structures (`struct`)
- pointers
- dynamic memory allocation
- well-structured, readable functions

---

## Milestones:

### 🚩 Milestone 1: Initial Project Setup & Modular Structure
**Due Date:** April 07, 2026
- [ ] Define detailed project specification and functional requirements (#Issue 1)
- [ ] Set up repository structure and initial modular framework in C (#Issue 2)
- [ ] Configure Makefile and compilation settings (#Issue 3)
- [ ] Design and implement initial data structures (structs, pointers) (#Issue 4)

### 🚩 Milestone 2: Core Game Logic & File Handling
**Due Date:** May 05, 2026
- [ ] Implement the main game logic (move validation, game rules enforcement) (#Issue 5)
- [ ] Integrate dynamic memory management (linked lists, dynamic arrays, trees) for game state handling (#Issue 6)
- [ ] Develop file operations for game states, logs, and player statistics (saving/loading mechanisms) (#Issue 7)
- [ ] Perform rigorous debugging, memory-leak checking (valgrind), and preliminary testing (#Issue 8)

### 🚩 Milestone 3: GUI Development & Enhanced User Interaction
**Due Date:** June 11, 2026
- [ ] Implement graphical user interface using libraries like Allegro, SFML, or SDL (#Issue 9)
- [ ] Introduce cursor-based interaction and graphical constraints on valid moves (#Issue 10)
- [ ] Comprehensive error handling (input validation, memory safety, exception handling) (#Issue 11)
- [ ] Integration, functional, and usability testing of GUI and core gameplay mechanics (#Issue 12)

### 🚩 Final Submission
**Due Date:** June 15, 2026
- [ ] Refactor, finalize codebase for readability, maintainability, and efficiency (#Issue 13)
- [ ] Finalize documentation, commenting, and README updates (#Issue 14)
- [ ] Project submission via ekursy platform (#Issue 15)

---

## Repository Structure:
```
your_project/
├── assets/
│   └── images/
├── docs/
│   ├── project_specification.md
│   └── user_manual.md
├── include/
│   ├── game_logic.h
│   ├── gui.h
│   └── file_operations.h
├── src/
│   ├── game_logic.c
│   ├── gui.c
│   ├── file_operations.c
│   └── main.c
├── logs/
├── saves/
├── tests/
│   ├── test_logic.c
│   └── test_gui.c
├── Makefile
├── .gitignore
└── README.md
```

---

## GitHub Issues Template:
Clearly defined issues for efficient project tracking:

1. Specification and Requirements Documentation
2. Repository and Modular Architecture Setup
3. Makefile and Compilation Configuration
4. Initial Dynamic Structures (e.g., structs, pointers)
5. Implementation of Game Logic and Rule Enforcement
6. Dynamic Data Structures for Game State Management
7. File Operations (Saving/Loading, Logging)
8. Debugging and Memory Management Validation (valgrind)
9. GUI Implementation using Allegro/SFML/SDL
10. Cursor and Gameplay Interaction Improvements
11. Robust Error Handling and Input Validation
12. GUI and Core Integration Tests
13. Final Code Refactoring and Optimization
14. Final Documentation and README Preparation
15. Submission 

---

## Technical and Academic Compliance:
This project explicitly aligns with requirements for obtaining a 5.0 grade:

- **Logical modularity** for ease of maintenance and scalability.
- **Advanced use of pointers and dynamic structures** to manage memory explicitly and efficiently.
- **Robust file-based functionality** to store game state and logs.
- **Comprehensive GUI** ensuring intuitive, interactive gameplay.
- **Detailed error handling** to produce resilient, secure software.

---

## Development and Contribution Guidelines:
- Clear and descriptive variable/function naming for readability.
- Modular implementation: each C file represents a specific module.
- Regular commits and pull requests, referencing specific issues (e.g., `#3 Modular architecture implemented`).
- Thorough documentation and inline comments.

---

## 🧪 Testing

The project should include:

- test scenarios
- manual testing
- handling of edge/error cases

---

## 📚 Documentation

Located in the `docs/` directory:

- `project_description.md`
- `architecture.md`
- `user_manual.md`

---

## 🐛 Project Management

Required:

- use of GitHub Issues
- task decomposition
- meaningful commit messages

---

