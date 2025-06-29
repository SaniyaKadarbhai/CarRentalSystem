# CarRentalSystem-C

## An Efficient Command-Line Car Rental Management System in C

This project implements a console-based Car Rental System developed in C. It provides functionalities for both car owners (or administrators) and customers to manage car listings, view availability, search for vehicles, book rentals, and track rental history. The system leverages fundamental data structures and algorithms, including the Boyer-Moore string matching algorithm for efficient searches and `qsort` for dynamic data sorting.

## ✨ Features

### Owner/Administrator Functions:

* **Car Management:** Add, modify, and view details of cars in the fleet.

* **Availability Tracking:** Easily see which cars are available and which are currently booked.

* **Rental History:** View the rental history for specific cars, including customer details, rental duration, and total cost.

* **Secure Access:** Owner functionalities are protected by a predefined password.

### Customer Functions:

* **Browse Cars:** Display a list of all available cars with their details (model, registration, price per hour, seats, AC status, category).

* **Search Filters:** Efficiently find cars based on category (e.g., Luxury, Economy, SUV), number of seats, and AC availability using the Boyer-Moore algorithm.

* **Sort Listings:** Sort cars by price, model name, or availability to quickly find suitable options.

* **Book Cars:** Rent an available car for a specified number of hours, with automatic calculation of total cost and recording of booking time and customer.

* **Authentication:** Basic customer login for accessing rental services.

## 🚀 Technologies Used

* **C Language:** The core implementation.

* **Standard C Libraries:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`.

* **Data Structures:** Arrays for storing car records.

* **Algorithms:**

    * **Boyer-Moore Algorithm:** Used for efficient string matching in search functionalities (e.g., matching car categories).

    * **`qsort`:** Standard C library function for sorting car listings based on various criteria (price, model, availability).



## 📖 Usage

Upon running the program, you will be prompted to choose between an **Owner (O)** or **Customer (C)** role.

* **Owner Authentication:**

    * `Username`: (Any)

    * `Password`: `owner@123` (This is the hardcoded password for owner access)

* **Customer Authentication:**

    * `Username`: (Any, for now)

    * `Password`: (Any, for now)

Follow the on-screen menu prompts to navigate through the system's functionalities.

