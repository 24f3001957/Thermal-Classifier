#include <Wire.h>             // Standard library for I2C communication (required for many embedded sensors)
#include <Adafruit_AMG88xx.h> // Library for the 8x8 thermal array sensor
#include <math.h>             // For mathematical functions like sqrt() and pow()
#include "perimeter_model.h"  // Header file containing the pre-trained ML model function 'score()'

// --- Global Variables and Setup ---
Adafruit_AMG88xx amg;           // Instance of the thermal sensor object
float pixels[64];               // Array to store 64 raw 8x8 pixel temperature readings
double normalized_input[64];    // Array to store 64 normalized pixel values (input for the ML model)
double model_output[3];         // Array to store the 3 class probabilities/scores from model inference

// Define the final human-readable output classes
const char* class_names[2] = {"nonhuman", "human"};

void setup() {
  // Initialize serial communication for logging and debugging
  Serial.begin(115200);
  Serial.println("Thermal Classifier Booting...");
  
  // Initialize the thermal sensor
  if (!amg.begin()) {
    Serial.println("ERROR: Thermal sensor not detected. Check configuration!");
    while (1); // Halt execution on failure
  }
  Serial.println("Sensor OK. Starting prediction loop...");
  delay(100);
}

void loop() {
  // Read the 8x8 pixel data (64 values) from the sensor
  amg.readPixels(pixels);

  // --- Data Preprocessing: Z-score Normalization ---
  // Calculates the Z-score for each pixel value, which is required by the trained model.
  double sum = 0.0;
  for (int i = 0; i < 64; i++) sum += pixels[i];
  double mean = sum / 64.0; // Calculate the mean

  double sq_sum = 0.0;
  for (int i = 0; i < 64; i++) sq_sum += pow(pixels[i] - mean, 2);
  double std_dev = sqrt(sq_sum / 64.0); // Calculate the standard deviation
  double epsilon = 1e-8; // Prevents division by zero

  // Apply Z-score formula: (x - mean) / std_dev
  for (int i = 0; i < 64; i++) {
    normalized_input[i] = (pixels[i] - mean) / (std_dev + epsilon);
  }

  // --- Model Inference ---
  // Execute the pre-trained machine learning model on the normalized data.
  // The model outputs scores for 3 distinct classes (0, 1, 2).
  score(normalized_input, model_output);

  // Find the class index (0, 1, or 2) with the highest probability/score
  int predicted_class = 0;
  double max_score = model_output[0];
  for (int i = 1; i < 3; i++) {
    if (model_output[i] > max_score) {
      max_score = model_output[i];
      predicted_class = i;
    }
  }

  // --- Classification Post-processing: Class Merging ---
  // Map the 3 raw model outputs into 2 final, desired categories (human/nonhuman).
  int final_class;
  if (predicted_class == 2) {
      final_class = 1;      // e.g., Model Class 2 (Human) maps to final 'human'
  } else {
      final_class = 0;      // e.g., Model Class 0 or 1 (Ambient/Object) maps to final 'nonhuman'
  }

  // Log the final result and the model's confidence score
  Serial.print("Prediction: ");
  Serial.print(class_names[final_class]);
  Serial.print(" | Score: ");
  Serial.println(max_score);

  // Delay before the next reading cycle
  delay(1000);
}