from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///project.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""

    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Displays a welcome screen
@app.route("/")
@login_required
def index():
    """Homepage"""

    user_id = session["user_id"]
    username = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]["username"]
    return render_template("welcome.html", username=username)


# Displays the calculator and coffee stopwatch
@app.route("/make", methods=["GET", "POST"])
@login_required
def make():
    """Show Calculator and Stopwatch"""

    if request.method == "GET":
        return render_template("make_coffee.html")

    if request.method == "POST":

        # Get coffee information
        ratio = int(request.form.get("ratio"))
        water = int(request.form.get("water"))

        # Calculates and returns how much coffee to use
        coffee = round((water / (1000 / ratio)), 1)
        return render_template("make_coffee_copy.html", coffee=coffee, ratio=ratio, water=water)


# Displays all the coffee notes from the database
@app.route("/notes", methods=["GET", "POST"])
@login_required
def notes():
    """Show Notes"""

    # Query history page information from database
    notes = db.execute(
        "SELECT name, roaster, origin, roast, flavors, notes, rating FROM coffee WHERE user_id = ? ORDER BY rating DESC", session["user_id"])
    return render_template("notes.html", notes=notes)


# Adds information to the notes database
@app.route("/create_note", methods=["GET", "POST"])
@login_required
def create():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("create_note.html")

    if request.method == "POST":

        # Get stock information
        name = request.form.get("name")
        roaster = request.form.get("roaster")
        origin = request.form.get("origin")
        roast = request.form.get("roast")
        rating = request.form.get("rating")
        flavors = request.form.get("flavors")
        notes = request.form.get("notes")

        # Get user_id
        user_id = session["user_id"]

        # Record note in database
        db.execute("INSERT INTO coffee(user_id, name, roaster, origin, roast, rating, flavors, notes) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
                   user_id, name, roaster, origin, roast, rating, flavors, notes)
        return redirect("/notes")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return "Must provide username", 400

        # Ensure password was submitted
        elif not request.form.get("password"):
            return "Must provide password", 400

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return "Invalid username and/or password", 400

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Cick the register button
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return "Must provide username", 400

        # Ensure password was submitted
        elif not request.form.get("password"):
            return "Must provide password", 400

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return "Passwords must match", 400

        # Ensure username does not already exist
        try:
            request.form.get("username") == db.execute("SELECT username FROM users WHERE username = ?",
                                                       request.form.get("username"))[0]["username"]
        except IndexError:
            # Add username and password to users
            pass_hash = generate_password_hash(request.form.get("password"))
            username = request.form.get("username")
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, pass_hash)
            return redirect("/")

        return "username already exists"


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

