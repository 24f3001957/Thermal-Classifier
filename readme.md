# 💡 Thermal Signature Classification (Embedded Machine Learning)

This project is a **collaborative effort** to develop a rugged, low-power system for real-time perimeter monitoring. It implements a lightweight, high-performance machine learning model directly on an embedded system to classify thermal signatures captured by an 8x8 sensor array. [cite_start]The goal is to provide a rapid, flexible, and scalable security solution for dynamic and remote environments[cite: 16].

***

## 🤝 Project Team & Roles

The success of this project was dependent on the combined effort in both hardware system design and the machine learning software implementation.

| Name | Role |
| :--- | :--- |
| **V.Sai Nihal** | Team Member |
| **V.S.K.Sumiran Apparaju** | Team Member |
| **V.Shanmukha Rakesh**  | Team Member |
| **V.Preethi** | Team Member |
| **U.Anamika** | Team Member |
| **V.Madhu Krishna Vinay** | Team Member |

#### Hardware & System Design Contribution
This section acknowledges the critical contribution of the team members responsible for the physical deployment and system integration. [cite_start]Their work involved selecting the sensors (e.g., Thermal Sensor, Microwave radar sensor), setting up the communication system (LoRa Module), designing the power solution (e.g., Li-ion battery with TP4056 charging and solar panel), and ensuring the final device is rugged and compact[cite: 58, 59, 158, 165, 172, 188].

#### Software & ML Implementation (My Contribution)
My role focused exclusively on developing the entire intelligence layer—the machine learning model and the embedded C++ firmware—to transform raw data into actionable classification alerts.

***

## 💻 My Contribution: ML Pipeline & Embedded Firmware

My role involved designing and implementing the complete **software stack**, spanning from initial data processing and model training to the final deployment of the classification algorithm onto the microcontroller.

#### 1. Machine Learning Model Development (See `ML_Development/`)
My work ensured the creation of a robust model that can accurately process sensor data in a resource-constrained environment.

* **Model Training & Selection:** Developed, trained, and tuned a multiclass classification model (documented in `Ml_project.ipynb`) capable of distinguishing between up to three thermal categories.
* **Model Conversion:** Converted the trained model into highly optimized, dependency-free **standard C code** (`perimeter_model.h`). This step enables rapid, low-power inference directly on a microcontroller.

#### 2. Embedded Firmware Implementation (See `Firmware/`)
I developed the C++ firmware (`thermal_classifier.ino`) that executes the entire classification workflow in real-time.

* **Real-time Preprocessing:** Implemented the **Z-score Normalization** algorithm to standardize the 64-pixel sensor readings before they are fed into the model.
* **Classification Logic:** Developed the logic to manage the model's core `score()` function, including implementing custom **class merging logic** to map the three detailed model outputs into the final, simple, user-facing categories: **`human`** or **`nonhuman`**.

***
