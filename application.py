import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
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
    user_stocks = db.execute("SELECT USER_ID, SYMBOL, NAME, SUM(SHARES) AS SHARES, SUM(VALUE) AS VALUE FROM current_stocks WHERE user_id = :user GROUP BY symbol",
    user = session["user_id"])

    for stock in user_stocks:
        price = lookup(stock["symbol"])["price"]
        stock.update({ "price": price })

        total = price * stock["SHARES"]
        stock.update({ "total": total })

    return render_template("index.html", user_stocks=user_stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Ensure stock name was submitted
        if not symbol:
            return apology("must provide a stock symbol")

        # Ensure valid stock name
        elif not lookup(symbol):
            return apology("Could not find stock")

        # Ensure a positive integer entered for number of shares
        elif shares < 1:
            return apology("Please provide a valid no. of shares")


        # Calculate value of transaction, users current balance, and new total balance
        value = (lookup(symbol))["price"] * shares

        current_balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash']

        total = current_balance - value

        # If enough money for transaction, carry out transaction
        if int(value) < current_balance:



            # Update current_stocks
            ## If users doesn't currently have stocks from this company, insert new row
            stock = db.execute("SELECT shares, value FROM current_stocks WHERE user_id = :user AND symbol = :symbol",
                          user=session["user_id"], symbol=symbol)

            if not stock:
                db.execute("INSERT INTO current_stocks (user_id, symbol, name, shares, value, total) VALUES (:user, :symbol, :name, :shares, :value, :total)",
                        user = session["user_id"],
                        symbol = symbol,
                        name = (lookup(symbol))["name"],
                        shares = shares,
                        value = value,
                        total = total)

            ## If already has a row for this stock, update row in the stock table
            else:
                shares += stock[0]['shares']
                value += stock[0]['value']

            db.execute("UPDATE current_stocks SET shares = :shares, value = :value, total = :total WHERE user_id = :user AND symbol = :symbol",
                user=session["user_id"],
                symbol=symbol,
                shares=shares,
                value=value,
                total=total)

            # Update cash in users
            db.execute("UPDATE users SET cash = :cash WHERE id = :user",
                        cash = current_balance - value, user = session["user_id"])

            # Update transaction history
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user, :symbol, :shares, :price)",
                        user = session["user_id"],
                        symbol = symbol,
                        shares = shares,
                        price = value)

            # Redirect user to homepage
            return redirect("/")

        else:
            return apology("You do not have enough money")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT symbol, shares, price, time FROM transactions WHERE user_id = :user",
                   user = session["user_id"])

    return render_template("history.html", transactions=transactions )

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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        price = lookup(request.form.get("symbol"))

        if not price:
            return apology("Could not find stock")

        return render_template("quoted.html", price=price)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any existing logged in user
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        #return apology("TODO")
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password and confirm password matches
        elif request.form.get("password") != request.form.get("password2"):
            return apology("passwords do not match", 403)

        # Query database to ensure username does not already exist
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) != 0:
            return apology("username already exists", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                    username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Ensure stock name was selected
        if not symbol:
            return apology("must select a stock symbol")

        # Ensure they own shares of this stock to sell
        elif not db.execute("SELECT symbol FROM current_stocks WHERE symbol = :symbol AND user_id = :user", symbol = symbol, user = session["user_id"]):
            return apology("You do not own any of this stock")


        shares = int(request.form.get("shares"))

        # Ensure a positive integer entered for number of shares
        if shares < 1:
            return apology("Please provide a valid no. of shares")

        # Ensure they own enough shares of this stock to sell
        elif db.execute("SELECT shares FROM current_stocks WHERE symbol = :symbol AND user_id = :user", symbol = symbol, user = session["user_id"])[0]['shares'] < shares:
            return apology("You do not own enough of this stock")

        # Calculate value of transaction, users current balance, and new balance
        value = (lookup(symbol))["price"] * shares

        current_balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash']

        new_balance = current_balance + value

        # Update current_stocks table
        ## calculate new number of shares
        new_shares = db.execute("SELECT shares FROM current_stocks WHERE symbol = :symbol AND user_id = :user",
                     user = session["user_id"], symbol = symbol)[0]['shares'] + shares

        db.execute("UPDATE current_stocks SET shares = :shares WHERE user_id = :user AND symbol = :symbol",
                          symbol=symbol, user=session["user_id"], shares=new_shares)

        # Update cash in users
        db.execute("UPDATE users SET cash = :cash WHERE id = :user",
                        cash = new_balance, user = session["user_id"])

        # Update transaction history
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user, :symbol, :shares, :price)",
                        user = session["user_id"],
                        symbol = symbol,
                        shares = shares,
                        price = value)

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
