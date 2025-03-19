# Dockerfile
#
# This Dockerfile creates a containerized environment for running a Flask application 
# that integrates with a shared C library for efficient data structures. 
# It sets up a Python 3.11 environment, compiles C code into a shared library, 
# installs required dependencies, and runs the Flask application using Gunicorn.

# Use an official Python base image (Python 3.11-slim is a lightweight Linux-based image)
FROM python:3.11-slim

# Update package lists and install necessary build dependencies:
# - `gcc`        : Required for compiling C source files into a shared library.
# - `make`       : Needed for compiling certain dependencies.
# - `libsqlite3-dev` : Ensures SQLite functionality is available for database interactions.
RUN apt-get update && \
    apt-get install -y gcc make libsqlite3-dev && \
    rm -rf /var/lib/apt/lists/*  # Clean up package lists to reduce image size.

# Set the working directory inside the container.
# All subsequent commands will be executed relative to this directory.
WORKDIR /app

# Copy the entire project into the container.
# This includes Python files, C source files, dependencies, and any other necessary assets.
COPY . .

# Compile the C source files into a shared library.
# - `gcc -shared -fPIC` : Creates a position-independent shared library.
# - `-o flask_app/libinventory.so` : Specifies the output file (shared library).
# - `inventory_structures.c btree.c hashmap.c` : Input C source files to be compiled.
RUN gcc -shared -fPIC -o flask_app/libinventory.so inventory_structures.c btree.c hashmap.c

# (Optional) Verify that the shared library was built correctly:
# Uncomment the following line to confirm the correct file type.
# RUN file flask_app/libinventory.so

# Install Python dependencies:
# - `pip install --upgrade pip` : Ensures the latest version of `pip` is used.
# - `pip install -r requirements.txt` : Installs all required Python dependencies listed in `requirements.txt`.
RUN pip install --upgrade pip && pip install -r requirements.txt

# Expose the port that your Flask application will listen on.
# Default Gunicorn port is `8000`, so this ensures the container listens for incoming requests.
EXPOSE 8000

# Define the command to start the application.
# - `gunicorn` : A production-grade WSGI server for running Python web applications.
# - `flask_app.app:app` : Specifies the Flask application module and instance to run.
# - `--bind 0.0.0.0:8000` : Binds the application to all network interfaces on port 8000.
CMD ["gunicorn", "flask_app.app:app", "--bind", "0.0.0.0:8000"]
