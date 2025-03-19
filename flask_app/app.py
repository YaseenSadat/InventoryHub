"""
app.py

Flask-based Inventory Management System.

Features:
- User authentication (signup, login, logout).
- Inventory management (add, update, delete items).
- User role handling (admin, regular users).
- SQLite database for persistent storage.
- In-memory cache integration for performance optimization.

Modules:
- Flask: Web framework.
- SQLite3: Database storage.
- Werkzeug.security: Password hashing and verification.
- functools: Used for authentication decorators.
"""

from flask import Flask, render_template, request, redirect, url_for, flash, session
import sqlite3
from werkzeug.security import generate_password_hash, check_password_hash
from functools import wraps

app = Flask(__name__)
app.secret_key = 'your_secret_key'  # Replace with a secure key in production

# Path to your SQLite DB file (make sure the path is correct relative to this app)
DATABASE = '../inmemory.db'


def init_db():
    """Initialize the database by creating required tables."""
    conn = get_db_connection()
    conn.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role TEXT DEFAULT 'user'
        );
    ''')
    conn.execute('''
        CREATE TABLE IF NOT EXISTS items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT,
            quantity INTEGER NOT NULL DEFAULT 0,
            price REAL NOT NULL DEFAULT 0.0,
            supplier TEXT,
            user_id INTEGER,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
    ''')
    conn.commit()
    conn.close()


def get_db_connection():
    """Establish a connection to the SQLite database."""
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row  # Enables accessing columns by name
    return conn


def login_required(view):
    """Decorator to restrict access to logged-in users."""
    @wraps(view)
    def wrapped_view(**kwargs):
        if 'user_id' not in session:
            flash('Please log in to access this page.')
            return redirect(url_for('login'))
        return view(**kwargs)
    return wrapped_view


@app.route('/signup', methods=('GET', 'POST'))
def signup():
    """Handles user registration."""
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        confirm = request.form['confirm']

        if not username or not password or not confirm:
            flash('All fields are required!')
        elif password != confirm:
            flash('Passwords do not match!')
        else:
            hashed_password = generate_password_hash(password)
            try:
                conn = get_db_connection()
                conn.execute('INSERT INTO users (username, password) VALUES (?, ?)', 
                             (username, hashed_password))
                conn.commit()
                conn.close()
                flash('Account created successfully! Please log in.')
                return redirect(url_for('login'))
            except sqlite3.IntegrityError:
                flash('Username already exists!')
    return render_template('signup.html')


@app.route('/login', methods=('GET', 'POST'))
def login():
    """Handles user login."""
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        conn = get_db_connection()
        user = conn.execute('SELECT * FROM users WHERE username = ?', (username,)).fetchone()
        conn.close()

        if user is None:
            flash('Incorrect username!')
        elif not check_password_hash(user['password'], password):
            flash('Incorrect password!')
        else:
            session.clear()
            session['user_id'] = user['id']
            session['username'] = user['username']
            flash('Logged in successfully!')
            return redirect(url_for('index'))
    return render_template('login.html')


@app.route('/logout')
def logout():
    """Logs the user out and clears the session."""
    session.clear()
    flash('Logged out successfully!')
    return redirect(url_for('index'))


@app.route('/')
def index():
    """Redirects the homepage to the inventory page."""
    return redirect(url_for('inventory'))


@app.route('/inventory')
def inventory():
    """Displays all inventory items."""
    conn = get_db_connection()
    items = conn.execute('''
        SELECT i.*, u.username
        FROM items i
        LEFT JOIN users u ON i.user_id = u.id
        ORDER BY i.name
    ''').fetchall()
    conn.close()
    return render_template('inventory.html', items=items)


@app.route('/inventory/add', methods=('GET', 'POST'))
@login_required
def add_item():
    """Allows logged-in users to add an inventory item."""
    if request.method == 'POST':
        name = request.form['name']
        description = request.form['description']
        quantity = request.form['quantity']
        price = request.form['price']
        supplier = request.form['supplier']

        if not name or not quantity or not price:
            flash('Name, Quantity, and Price are required!')
        else:
            conn = get_db_connection()
            cur = conn.execute('''
                INSERT INTO items (name, description, quantity, price, supplier, user_id)
                VALUES (?, ?, ?, ?, ?, ?)
            ''', (name, description, quantity, price, supplier, session['user_id']))
            item_id = cur.lastrowid  # Get the generated item ID
            conn.commit()
            conn.close()
            flash('Item added successfully!')
            return redirect(url_for('inventory'))
    return render_template('add_item.html')


@app.route('/inventory/update/<int:item_id>', methods=('GET', 'POST'))
@login_required
def update_item(item_id):
    """Allows users to update an inventory item they own."""
    conn = get_db_connection()
    item = conn.execute('SELECT * FROM items WHERE id = ?', (item_id,)).fetchone()
    conn.close()

    if item is None:
        flash(f'Item with ID {item_id} not found.')
        return redirect(url_for('inventory'))

    if (int(item['user_id']) != int(session.get('user_id'))) and (session.get('role') != 'admin'):
        flash("You don't have permission to edit this item.")
        return redirect(url_for('inventory'))

    if request.method == 'POST':
        name = request.form['name']
        description = request.form['description']
        quantity = request.form['quantity']
        price = request.form['price']
        supplier = request.form['supplier']

        if not name or not quantity or not price:
            flash('Name, Quantity, and Price are required!')
        else:
            conn = get_db_connection()
            conn.execute('''
                UPDATE items
                SET name = ?, description = ?, quantity = ?, price = ?, supplier = ?
                WHERE id = ?
            ''', (name, description, quantity, price, supplier, item_id))
            conn.commit()
            conn.close()
            flash('Item updated successfully!')
            return redirect(url_for('inventory'))
    return render_template('update_item.html', item=item)


@app.route('/inventory/delete/<int:item_id>', methods=('POST',))
@login_required
def delete_item(item_id):
    """Allows users to delete an inventory item they own."""
    conn = get_db_connection()
    item = conn.execute('SELECT * FROM items WHERE id = ?', (item_id,)).fetchone()

    if item is None:
        flash(f'Item with ID {item_id} not found.')
        return redirect(url_for('inventory'))

    if (int(item['user_id']) != int(session.get('user_id'))) and (session.get('role') != 'admin'):
        flash("You don't have permission to delete this item.")
        return redirect(url_for('inventory'))

    conn.execute('DELETE FROM items WHERE id = ?', (item_id,))
    conn.commit()
    conn.close()
    flash('Item deleted successfully!')
    return redirect(url_for('inventory'))


cache_initialized = False

@app.before_request
def init_cache_once():
    """Initializes the cache and database before handling requests."""
    global cache_initialized
    if not cache_initialized:
        init_db()
        cache_initialized = True

    
if __name__ == '__main__':
    app.run(debug=True)
