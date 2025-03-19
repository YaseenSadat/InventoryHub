"""
inventory_cache.py

This module interfaces with a shared C library (`libinventory.so`) using `ctypes` 
to manage an in-memory cache for inventory data.

Features:
- Loads the shared library dynamically based on the OS.
- Provides wrapper functions for interacting with a B-tree and hash map.
- Supports inserting, retrieving, and debugging inventory items in memory.
- Includes cleanup functions to free allocated resources.

Dependencies:
- The shared C library (`libinventory.so` or `inventory.dll` on Windows).
- `ctypes` for calling C functions from Python.
"""

import os
import ctypes

# --- Load Shared Library ---

# Determine the correct library filename based on the OS.
# For Linux/macOS, it is likely "libinventory.so"
# For Windows, it might be "inventory.dll"
lib_filename = "libinventory.so"  # Modify this if needed

# Construct the full path to the shared library.
lib_path = os.path.join(os.path.dirname(__file__), lib_filename)

# Load the shared library using ctypes.
inventory_lib = ctypes.CDLL(lib_path)

# --- Define Function Prototypes ---

# B-tree functions
inventory_lib.init_btree.restype = None
inventory_lib.insert_into_btree.argtypes = [ctypes.c_int]
inventory_lib.insert_into_btree.restype = None
inventory_lib.traverse_btree.restype = None

# Hash map functions
inventory_lib.init_hashmap.restype = None
inventory_lib.insert_into_hashmap.argtypes = [ctypes.c_int, ctypes.c_char_p]
inventory_lib.insert_into_hashmap.restype = None
inventory_lib.get_from_hashmap.argtypes = [ctypes.c_int]
inventory_lib.get_from_hashmap.restype = ctypes.c_char_p

# Memory cleanup function
inventory_lib.free_structures.restype = None

# --- Python Wrappers for Convenience ---

def initialize_structures():
    """Initialize both the B-tree and hash map."""
    inventory_lib.init_btree()
    inventory_lib.init_hashmap()

def cache_insert_item(item_id, item_data):
    """Insert an item into the in-memory cache.
    
    Parameters:
      item_id (int): Unique identifier for the item.
      item_data (str): Data associated with the item.
      
    Example:
      cache_insert_item(101, "Widget A - High Quality")
    """
    # Convert the Python string to bytes (UTF-8) before passing to C.
    inventory_lib.insert_into_hashmap(item_id, item_data.encode('utf-8'))
    
    # Optionally, also insert into the B-tree.
    # Here we simply use item_id as the key (or you could hash the name).
    inventory_lib.insert_into_btree(item_id)

def cache_get_item(item_id):
    """Retrieve an item’s data from the in-memory cache.
    
    Parameters:
      item_id (int): Unique identifier for the item.
      
    Returns:
      str or None: The associated data if found, else None.
    
    Example:
      data = cache_get_item(101)
      print(data)  # Outputs: "Widget A - High Quality"
    """
    result = inventory_lib.get_from_hashmap(item_id)
    if result:
        return result.decode('utf-8')  # Convert C string (bytes) back to Python string
    return None

def debug_traverse_btree():
    """Traverse the global B-tree and print its keys (for debugging)."""
    inventory_lib.traverse_btree()

def free_cache():
    """Clean up the in-memory structures."""
    inventory_lib.free_structures()
