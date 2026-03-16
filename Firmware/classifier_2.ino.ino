#include <Wire.h>             
#include <Adafruit_AMG88xx.h> 
#include <math.h>             
#include "perimeter_model.h"  

// --- Global Variables and Setup ---
Adafruit_AMG88xx amg;           
float pixels[64];               
double normalized_input[64];    
double model_output[3];         

// Updated to 3 classes based on your latest successful training
// Index 0: animal, Index 1: empty, Index 2: human
const char* class_names[3] = {"animal", "empty", "human"};

void setup() {
  Serial.begin(115200);
  Serial.println("Thermal Classifier Booting (3-Class Polynomial SVM)...");
  
  if (!amg.begin()) {
    Serial.println("ERROR: Thermal sensor not detected. Check configuration!");
    while (1); 
  }
  Serial.println("Sensor OK. Monitoring Perimeter...");
  delay(100);
}

void loop() {
  // 1. Read the 8x8 pixel data
  amg.readPixels(pixels);

  // 2. Data Preprocessing: Z-score Normalization
  // Must match exactly how we normalized in Python
  double sum = 0.0;
  for (int i = 0; i < 64; i++) sum += pixels[i];
  double mean = sum / 64.0;

  double sq_sum = 0.0;
  for (int i = 0; i < 64; i++) sq_sum += pow(pixels[i] - mean, 2);
  double std_dev = sqrt(sq_sum / 64.0); 
  double epsilon = 1e-8; 

  for (int i = 0; i < 64; i++) {
    normalized_input[i] = (pixels[i] - mean) / (std_dev + epsilon);
  }

  // 3. Model Inference
  // Calls the C++ function in your generated perimeter_model.h
  score(normalized_input, model_output);

  // 4. Find the winning class (Highest score)
  int predicted_class = 0;
  double max_score = model_output[0];
  for (int i = 1; i < 3; i++) {
    if (model_output[i] > max_score) {
      max_score = model_output[i];
      predicted_class = i;
    }
  }

  // 5. Output Results
  Serial.print("Target: ");
  Serial.print(class_names[predicted_class]);
  Serial.print(" | Confidence Score: ");
  Serial.println(max_score);

  // Using a 500ms delay for faster real-time response
  delay(500);
}