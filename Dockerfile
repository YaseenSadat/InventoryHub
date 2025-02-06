# Use an official Python base image (Python 3.11-slim is a lightweight Linux-based image)
FROM python:3.11-slim

# Install build dependencies (GCC, make, and libsqlite3-dev) required to compile your C code.
RUN apt-get update && \
    apt-get install -y gcc make libsqlite3-dev && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container.
WORKDIR /app

# Copy the entire project into the container.
COPY . .

# Build the shared library:
# The C source files are located at the root, so reference them directly.
# This will output libinventory.so into the flask_app/ folder.
RUN gcc -shared -fPIC -o flask_app/libinventory.so inventory_structures.c btree.c hashmap.c

# (Optional) Verify that the shared library was built correctly:
# RUN file flask_app/libinventory.so

# Install Python dependencies.
RUN pip install --upgrade pip && pip install -r requirements.txt

# Expose the port that your app will run on (default for Gunicorn is 8000).
EXPOSE 8000

# Set the command to run your application using Gunicorn.
CMD ["gunicorn", "flask_app.app:app", "--bind", "0.0.0.0:8000"]
