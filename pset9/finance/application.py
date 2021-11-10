import os

import re
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get user data and sharedata
    userdata = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])
    sharedata = db.execute("SELECT symbol, SUM(shares) FROM purchase WHERE id = ? GROUP BY symbol", session["user_id"])
    sharetotal = 0

    for row in sharedata:
        stockinfo = lookup(row["symbol"])
        sharetotal += row["SUM(shares)"] * stockinfo["price"]
        row["price"] = usd(stockinfo["price"])
        row["value"] = usd(stockinfo["price"]*row["SUM(shares)"])

    userdata[0]["total"] = usd(sharetotal+userdata[0]["cash"])
    userdata[0]["cash"] = usd(userdata[0]["cash"])

    return render_template("index.html", userdata=userdata[0], sharedata=sharedata)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # check symbol name
        if not request.form.get("symbol"):
            return apology("give symbol name")

        if not request.form.get("shares"):
            return apology("give shares number")

        # check for valid symbol name
        data = lookup(request.form.get("symbol"))
        if data == None:
            return apology("invalid symbol name")
        print(data)

        # check for positive number of shares
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("give whole number")
        if shares < 1:
            return apology("give a postive number")

        # total money required to buy given number of shares
        total = data["price"] * shares
        # get available cash
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        # check if user has enough money
        if rows[0]["cash"] < total:
            return apology("not enough funds.")

        time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # reduce cash and add shares
        db.execute("INSERT INTO purchase (id, symbol, shares, time) VALUES (?, ?, ?, ?)",
                   session["user_id"], request.form.get("symbol"), shares, time)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", rows[0]["cash"]-total, session["user_id"])

        return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute("SELECT symbol, shares, time FROM purchase WHERE id=?", session["user_id"])
    return render_template("history.html", sharedata=data)


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
    if request.method == "POST":
        # check for symbol name
        if not request.form.get("symbol") or request.form.get("symbol") == None:
            return apology("give stock name")

        # get symbol info
        data = lookup(request.form.get("symbol"))
        if data == None:
            return apology("invalid symbol")

        # return name and price
        return render_template("quoted.html", name=data["name"], price=usd(data["price"]))
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # if someone is trying to register an account
    if request.method == "POST":
        # check for username and password
        if not request.form.get("username"):
            return apology("provide username", 400)
        if not request.form.get("password"):
            return apology("provide password", 400)
        if not request.form.get("confirmation"):
            return apology("confirm your password", 400)

        # check if username exists
        row = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(row) == 1:
            return apology("username already exists")

        # make password have letters, numbers and symbols
        if not has_numbers(request.form.get("password")) or not specialCharacters(request.form.get("password")):
            return apology("password should have numbers and special characters")

        # checking if the passwords match
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # generate password hash
        hash = generate_password_hash(request.form.get("password"))
        # query databse to add their account
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hash)

        return redirect("/")

    return render_template("register.html")


def specialCharacters(password):
    regex = re.compile('[@_!#$%^&*()<>?/\|}{~:]')
    if(regex.search(password) != None):
        return True
    return False


def has_numbers(inputString):
    return any(char.isdigit() for char in inputString)


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # check for proper values
        if request.form.get("symbol") == "none":
            return apology("choose a symbol")
        # check if user has said shares
        shares = db.execute("SELECT symbol, SUM(shares) FROM purchase WHERE id = ? AND symbol = ?",
                            session["user_id"], request.form.get("symbol"))
        if len(shares) == 0:
            return apology("you have no shares")

        # check for valid number of shares
        if not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("enter proper amount of shares")

        # check if user has enough shares
        sell_share = int(request.form.get("shares"))
        if sell_share > shares[0]["SUM(shares)"]:
            return apology("you dont have enough shares")

        # get latest price
        data = lookup(shares[0]["symbol"])
        time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # update purchase table with sold shares
        db.execute("INSERT INTO purchase (id, symbol, shares, time) VALUES (?, ?, ?, ?)",
                   session["user_id"], shares[0]["symbol"], -sell_share, time)
        # add cash to users table
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", data["price"]*sell_share, session["user_id"])

        return redirect("/")

    symbols = db.execute("SELECT symbol FROM purchase WHERE id = ? GROUP BY symbol", session["user_id"])
    return render_template("sell.html", symbol=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
