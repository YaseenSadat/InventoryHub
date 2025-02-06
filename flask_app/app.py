from flask import Flask, render_template, request, redirect, url_for, flash, session
import sqlite3
from werkzeug.security import generate_password_hash, check_password_hash
from functools import wraps

app = Flask(__name__)
app.secret_key = 'your_secret_key'  # Replace with a secure key in production

# Path to your SQLite DB file (make sure the path is correct relative to this app)
DATABASE = '../inmemory.db'


def init_db():
    conn = get_db_connection()
    # Create the users table.
    conn.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role TEXT DEFAULT 'user'
        );
    ''')
    # Create the items table for inventory.
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
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row  # Allows accessing columns by name
    return conn

def login_required(view):
    @wraps(view)
    def wrapped_view(**kwargs):
        if 'user_id' not in session:
            flash('Please log in to access this page.')
            return redirect(url_for('login'))
        return view(**kwargs)
    return wrapped_view

@app.route('/signup', methods=('GET', 'POST'))
def signup():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        confirm  = request.form['confirm']

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
    session.clear()
    flash('Logged out successfully!')
    return redirect(url_for('index'))


@app.route('/')
def index():
    # Repurpose the index route to show the inventory.
    return redirect(url_for('inventory'))



# @app.route('/add', methods=('GET', 'POST'))
# @login_required
# def add():
#     if request.method == 'POST':
#         # Only logged-in users can reach here, so we use session['user_id']
#         record_id = request.form['id']
#         data = request.form['data']

#         if not record_id or not data:
#             flash('Both ID and Data are required!')
#         else:
#             try:
#                 conn = get_db_connection()
#                 # Insert the record along with the user_id from session
#                 conn.execute('INSERT INTO records (id, data, user_id) VALUES (?, ?, ?)',
#              (record_id, data, session['user_id']))
#                 conn.commit()
#                 conn.close()
#                 flash('Record added successfully!')
#                 return redirect(url_for('index'))
#             except sqlite3.IntegrityError:
#                 flash('Error: A record with that ID may already exist.')
#     return render_template('add.html')


# @app.route('/delete/<int:record_id>', methods=('POST',))
# @login_required
# def delete(record_id):
#     conn = get_db_connection()
#     # Again, include r.user_id in the selection.
#     record = conn.execute('''
#         SELECT r.id, r.data, r.user_id, u.username
#         FROM records r
#         LEFT JOIN users u ON r.user_id = u.id
#         WHERE r.id = ?
#     ''', (record_id,)).fetchone()
    
#     if record is None:
#         flash(f'Record with ID {record_id} not found.')
#         return redirect(url_for('index'))
    
#     print("DEBUG: record['user_id'] =", record['user_id'], "type =", type(record['user_id']))
#     print("DEBUG: session['user_id'] =", session.get('user_id'), "type =", type(session.get('user_id')))
    
#     try:
#         record_user_id = int(record['user_id'])
#         current_user_id = int(session.get('user_id'))
#     except (TypeError, ValueError) as e:
#         flash('Invalid user ID detected. ' + str(e))
#         return redirect(url_for('index'))
    
#     if record_user_id != current_user_id:
#         flash('You do not have permission to delete this record.')
#         return redirect(url_for('index'))
    
#     conn.execute('DELETE FROM records WHERE id = ?', (record_id,))
#     conn.commit()
#     conn.close()
#     flash(f'Record {record_id} deleted successfully!')
#     return redirect(url_for('index'))



# @app.route('/update/<int:record_id>', methods=('GET', 'POST'))
# @login_required
# def update(record_id):
#     conn = get_db_connection()
#     # Include r.user_id in the selection.
#     record = conn.execute('''
#         SELECT r.id, r.data, r.user_id, u.username
#         FROM records r
#         LEFT JOIN users u ON r.user_id = u.id
#         WHERE r.id = ?
#     ''', (record_id,)).fetchone()
#     conn.close()

#     if record is None:
#         flash(f'Record with ID {record_id} not found.')
#         return redirect(url_for('index'))

#     # Debug prints to check the values
#     print("DEBUG: record['user_id'] =", record['user_id'], "type =", type(record['user_id']))
#     print("DEBUG: session['user_id'] =", session.get('user_id'), "type =", type(session.get('user_id')))

#     try:
#         record_user_id = int(record['user_id'])
#         current_user_id = int(session.get('user_id'))
#     except (TypeError, ValueError) as e:
#         flash('Invalid user ID detected. ' + str(e))
#         return redirect(url_for('index'))

#     if record_user_id != current_user_id:
#         flash('You do not have permission to edit this record.')
#         return redirect(url_for('index'))

#     if request.method == 'POST':
#         data = request.form['data']
#         if not data:
#             flash('Data is required!')
#         else:
#             conn = get_db_connection()
#             conn.execute('UPDATE records SET data = ? WHERE id = ?', (data, record_id))
#             conn.commit()
#             conn.close()
#             flash(f'Record {record_id} updated successfully!')
#             return redirect(url_for('index'))

#     return render_template('update.html', record=record)

@app.route('/inventory')
def inventory():
    conn = get_db_connection()
    # Join items with users to get the username of the person who added each item.
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
    if request.method == 'POST':
        name = request.form['name']
        description = request.form['description']
        quantity = request.form['quantity']
        price = request.form['price']
        supplier = request.form['supplier']

        if not name or not quantity or not price:
            flash('Name, Quantity, and Price are required!')
        else:
            try:
                conn = get_db_connection()
                cur = conn.execute('''
                    INSERT INTO items (name, description, quantity, price, supplier, user_id)
                    VALUES (?, ?, ?, ?, ?, ?)
                ''', (name, description, quantity, price, supplier, session['user_id']))
                item_id = cur.lastrowid  # Get the generated item ID.
                conn.commit()
                conn.close()
                flash('Item added successfully!')

                # Update the in-memory cache using the shared library.
                from inventory_cache import cache_insert_item
                # Create a data string to cache (for example, "name: description").
                item_data = f"{name}: {description}"
                cache_insert_item(item_id, item_data)

                return redirect(url_for('inventory'))
            except Exception as e:
                flash('Error adding item: ' + str(e))
    return render_template('add_item.html')


@app.route('/inventory/update/<int:item_id>', methods=('GET', 'POST'))
@login_required
def update_item(item_id):
    conn = get_db_connection()
    item = conn.execute('SELECT * FROM items WHERE id = ?', (item_id,)).fetchone()
    conn.close()

    if item is None:
        flash(f'Item with ID {item_id} not found.')
        return redirect(url_for('inventory'))

    # Allow update if the user is the owner or an admin.
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
    global cache_initialized
    if not cache_initialized:
        from .inventory_cache import initialize_structures
        init_db()
        initialize_structures()
        cache_initialized = True

    
if __name__ == '__main__':
    app.run(debug=True)
