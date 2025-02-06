# InventoryHub

# InventoryHub

**InventoryHub** is a custom inventory and resource management system designed to provide small businesses and hobbyists with a robust, efficient solution for tracking and managing stock. The platform features a modern Flask-based web interface, an SQLite database for persistent storage, and integrates a custom C shared library (loaded via Python’s ctypes) that implements a hash map and B‑tree. These low‑level data structures enable fast in‑memory caching and efficient, sorted search indexing, making InventoryHub uniquely powerful and responsive.

Check out the project: [InventoryHub](https://lumenera.onrender.com).

Check out the code on GitHub: [InventoryHub](https://github.com/YaseenSadat/InventoryHub).

---

## Table of Contents

1. [Creators](#creators)
2. [Summary](#summary)
3. [Features](#features)
4. [Technology](#technology)

---
## Creators

The project was founded and developed by:

- [YaseenSadat](https://github.com/YaseenSadat)

---

## Summary

🌟 **What InventoryHub Does**  
InventoryHub is an end-to-end inventory management system that empowers small businesses and hobbyists to efficiently track and manage their stock. It provides a comprehensive dashboard where users can add, update, and delete inventory items, view detailed item information, and manage records securely. By integrating a custom C shared library that implements a hash map and B‑tree for in-memory caching and search indexing, InventoryHub delivers fast data retrieval and efficient performance alongside modern web technologies.

🌟 **Why I Built InventoryHub**  
Inspired by the need for a cost-effective, customizable solution for inventory management, InventoryHub was created to bridge the gap between traditional inventory systems and modern, responsive web applications. The project showcases my ability to combine high-level web development with low-level systems programming, demonstrating both robust application design and optimized performance through custom data structures.

🌟 **Why InventoryHub is Unique**  
Unlike conventional inventory systems, InventoryHub uniquely integrates a custom C-based hash map and B‑tree via Python’s ctypes, ensuring rapid, in-memory access to critical data and efficient search capabilities. This hybrid approach provides a scalable and responsive solution, making InventoryHub an ideal platform for dynamic inventory management where speed and reliability are essential.



---

## Features

### ✨ User Authentication & Authorization  

- **Secure sign-up, login, and logout functionality.**
- **Role-based access control ensuring only authorized users (or admins) can add, update, or delete inventory items.** 

### Inventory Management  

- **A comprehensive dashboard that displays all inventory items along with details such as name, description, quantity, price, supplier, and the username of the person who added the item**
- **Full CRUD (Create, Read, Update, Delete) operations for inventory items with permission checks.**

### Custom In-Memory Data Structures Integration 

- **Hash Map: Implements fast, constant-time in-memory caching of inventory records.**
- **B‑Tree: Provides efficient, sorted search indexing and supports quick range queries.**

### Modern, Responsive UI  

- **A Flask-based web interface styled with Bootstrap, ensuring a seamless and intuitive user experience across devices.**

---

## Technology

### Languages  
- **Python**: The primary language for building the Flask web application, handling backend logic, and server-side functionality. 
- **C**: Used to implement custom low-level data structures (hash map and B‑tree) for efficient in‑memory caching and sorted search indexing, integrated via Python’s ctypes.

### Frameworks  
- **Flask**: A lightweight web framework that powers the backend and dynamic page rendering of the application.
- **Bootstrap**: Provides a modern, responsive, and aesthetically pleasing frontend UI.

### Libraries  
- **ctypes**: A Python library that enables the integration and usage of the custom C shared library within the Flask application.
- **Werkzeug**: Offers utilities for secure password hashing and session management in the Flask framework.

### Tools  
- **SQLite**: A lightweight, file-based database used for persistent storage of user and inventory data.- **Stripe**: Processes payments securely, including service fee handling. Serves as the core database system, managing data for users and inventory items with simplicity and efficiency.
- **Git**: Version control system used to track code changes and facilitate collaboration.
- **Docker**: Ensures consistent environments and smooth deployments via containerization.  
---
