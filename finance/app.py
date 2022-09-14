import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # Query data to see which stocks the user has bought
    user_stock_info = db.execute(
        "SELECT symbol, name, SUM(shares) as total_shares, price FROM purchase WHERE user_id = ? GROUP BY symbol", user_id)

    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    total = user_cash

    for stock in user_stock_info:
        total += stock["price"] * stock["total_shares"]

    return render_template("index.html", user_stock_info=user_stock_info, user_cash=usd(user_cash), total=usd(total), usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        stock_info = lookup(request.form.get("symbol"))
        if not stock_info:
            return apology("That stock symbol does not exist")

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Number of shares must be a positive integer")

        if shares <= 0:
            return apology("Number of shares must be greater than zero")

        # Get stock information
        stock_price = stock_info["price"]
        stock_symbol = stock_info["symbol"]
        stock_name = stock_info["name"]

        # Get user_id
        user_id = session["user_id"]

        # Query how much cash the user has
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Check if user has enough money
        remaining_cash = user_cash - shares * stock_price

        # If user does not have enough money
        if remaining_cash < 0:
            return apology("You do not have enough money to make this purchase")

        # If user has enough cash, execute purchase
        # Update user cash
        # Record purchase in database
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, user_id)
            db.execute("INSERT INTO purchase(user_id, type, symbol, name, shares, price) VALUES(?, ?, ?, ?, ?, ?)",
                       user_id, "BUY", stock_symbol, stock_name, shares, stock_price)
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query history page information from database
    transactions = db.execute(
        "SELECT type, symbol, price, shares, timestamp FROM purchase WHERE user_id = ? ORDER BY timestamp DESC", session["user_id"])
    return render_template("history.html", transactions=transactions, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Render page
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        stock_info = lookup(request.form.get("symbol"))
        if not stock_info:
            return apology("That stock symbol does not exist")
        return render_template("quoted.html", name=stock_info["name"], price=usd(stock_info["price"]), symbol=stock_info["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Cick the register button
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)

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

        return apology("username already exists")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    if request.method == "GET":
        stock_symbol = db.execute("SELECT symbol FROM purchase WHERE user_id = ? GROUP BY symbol", user_id)

        return render_template("sell.html", stock_symbol=stock_symbol)

    if request.method == "POST":

        selected_stock_symbol = request.form.get("symbol")

        quantity = db.execute("SELECT SUM(shares) AS sum FROM purchase WHERE user_id = ? AND symbol = ?",
                              user_id, selected_stock_symbol)
        selected_stock_quantity = int(quantity[0]["sum"])
        sell_quantity = int(request.form.get("shares"))
        selected_stock_price = lookup(selected_stock_symbol)["price"]
        selected_stock_name = lookup(selected_stock_symbol)["name"]

        # Ensure user selects a viable quantity
        if sell_quantity <= 0:
            return apology("Number of shares must be a positive number")

        if sell_quantity > selected_stock_quantity:
            return apology("You do not have that many shares!")

        # Query user's current cash
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Update user's current cash after selling the stock
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (current_cash + (sell_quantity * selected_stock_price)), user_id)

        # Update the purchase table after the sell
        db.execute("INSERT INTO purchase(user_id, type, symbol, name, shares, price) VALUES(?, ?, ?, ?, ?, ?)",
                   user_id, "SELL", selected_stock_symbol, selected_stock_name, -sell_quantity, selected_stock_price)

        return redirect("/")
