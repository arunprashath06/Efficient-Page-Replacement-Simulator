# 🧠 Efficient Page Replacement Algorithm Simulator

A complete C++ simulator for **FIFO**, **LRU**, and **Optimal** page replacement algorithms with a simple console UI, detailed frame visualization, and performance comparison.

---

## 📌 Project Overview

This project demonstrates how an Operating System handles page faults using different **page replacement algorithms**.  
The simulator accepts:

- A reference string (list of page numbers)  
- Number of frames  

Then it performs step-by-step simulation of:

- **FIFO** (First-In First-Out)  
- **LRU** (Least Recently Used)  
- **Optimal Page Replacement**

The program displays:

- Frame evolution (time × frame table)  
- Page fault indicators  
- Total page faults  
- Hit ratio  
- Algorithm comparison  

---

## 🎯 Features

- ✔ Interactive menu-driven console UI  
- ✔ FIFO, LRU, and Optimal algorithms  
- ✔ Visual 2D frame evolution table  
- ✔ Page fault tracking per request  
- ✔ Hit ratio + performance statistics  
- ✔ Compare all algorithms side-by-side  
- ✔ Modular and clean C++ codebase  
- ✔ Uses only standard C++ headers (no `<bits/stdc++.h>`)

---

## 🛠 Technologies Used

### **Programming Language**
- C++

### **Standard Libraries**
- `<iostream>` — input/output  
- `<vector>` — dynamic arrays  
- `<queue>` — FIFO operations  
- `<unordered_set>` — fast lookup  
- `<unordered_map>` — used for LRU timestamps  
- `<sstream>` — parses reference string  
- `<limits>` — input buffer handling  
- `<iomanip>` — formatting ratios  
- `<string>` — string processing  

---

## 📥 Input Format

### **Reference String (Example)**
```
7 0 1 2 0 3 0 4 2 3 0 3
```

### **Number of Frames**
```
3
```

---

## 📋 Menu Options
1. FIFO  
2. LRU  
3. Optimal  
4. Compare All  
5. Exit  

---

## 📊 Sample Output

```
Reference string:
7 0 1 2 0 3 ...

Frame evolution:
Time\Frame   0   1   2
Frame 0      7   7   2
Frame 1      -   0   0
Frame 2      -   -   1
Fault        F   F   F

Total Requests: 12
Page Faults: 9
Hit Ratio: 25.00%
```

---

## 🚀 How to Compile & Run

### **Compile**
```bash
g++ src/pagereplacement.cpp -o simulator
```

### **Run**
```bash
./simulator
```

---

## 📂 Project Structure

```
Efficient-Page-Replacement-Simulator/
│
├── src/
│   └── pagereplacement.cpp
│
├── tests/
│   └── run_tests.sh
│
├── examples/
│   └── sample.txt
│
├── .github/workflows/
│   └── ci.yml
│
├── README.md
└── LICENSE
```

---

## 🔮 Future Enhancements

- Add Second Chance / Clock algorithm  
- Build a GUI (Qt or web front-end)  
- Auto-generate random reference strings  
- Export results to CSV/PDF  
- Add animations or visual charts  

---
