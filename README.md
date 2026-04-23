# 🍉 Fruit Ninja Game (Qt - C++)

## 📌 Project Overview

This project is a **desktop-based Fruit Ninja-style game** developed using **Qt and C++**.

Players click on falling fruits to gain points while avoiding bombs. The game includes animations, sound effects, and a scoring system.

---

## 💻 Screenshots 

<table>
  <tr>
    <td align="center">
      <img src="screenshots\gamearea.png" alt="gamearea" width="1000"/><br/>
      <em>Main gameplay screen with active fruit slicing mechanics.</em>
    </td>
    <td align="center">
      <img src="screenshots\scoretable.png" alt="scoretable" width="1000"/><br/>
      <em>Scoreboard displaying real-time game statistics.</em>
    </td>
  </tr>
</table>


## 🎮 Gameplay Features

* 🍉 Multiple fruit types (watermelon, pineapple, strawberry)
* 🖱️ Click-based slicing mechanic
* 💣 Bomb system (game ends when clicked)
* ⏱️ Countdown timer
* 📊 Score calculation system
* 🏆 Maximum score tracking
* 🔊 Sound effects for slicing and explosions

---

## 🚀 Technologies Used

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://en.cppreference.com/w/)

[![Qt](https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)](https://www.qt.io/)

[![Qt Animation](https://img.shields.io/badge/QPropertyAnimation-009688?style=for-the-badge)](https://doc.qt.io/qt-6/qpropertyanimation.html)

[![Qt Timer](https://img.shields.io/badge/QTimer-FF9800?style=for-the-badge)](https://doc.qt.io/qt-6/qtimer.html)

[![Qt Resource](https://img.shields.io/badge/Qt_Resource-qrc-blue?style=for-the-badge)](https://doc.qt.io/qt-6/resources.html)

---

## ⚙️ Features & Implementation Details

- Event-driven programming (signals & slots)
- Custom clickable QLabel component for fruit interaction
- Real-time game loop using QTimer
- Smooth object movement with QPropertyAnimation
- Resource management using `.qrc` (images & sounds)
- File handling for score tracking (skorlar.txt)

---

## 🧠 Core Mechanics

* Fruits spawn randomly on screen
* Each fruit falls with animation
* Clicking a fruit:

  * Stops animation
  * Increases score
* Missing fruits:

  * Increases missed counter
* Bomb click:

  * Ends game immediately

---

## 📁 Project Structure

```bash
├── main.cpp
├── dialog.*
├── click.h
├── resources.qrc
├── images/
├── sounds/
├── screenshots/
├── konumlar.txt
├── skorlar.txt
```

---

## ▶️ How to Run

1. Open project in **Qt Creator**
2. Build the project
3. Run the game

---

## 🧠 Learning Outcomes

* GUI game development with Qt
* Event handling and signals/slots
* Animation systems
* Object-oriented design
* Real-time interaction handling

---

## 📞 Support & Contact

-   📧 Email: ozdogrumerve57@gmail.com
-   🐛 Issues: Feel free to report bugs or suggest features on [GitHub Issues](https://github.com/ozdogrumerve/Spotly/issues)
-   👤 Author: [Merve Özdoğru](https://github.com/ozdogrumerve)

---

<div align="center">

**⭐ Star this repo if you find it helpful!**

Made with ❤️ by [Merve Özdoğru](https://github.com/ozdogrumerve)

</div>

