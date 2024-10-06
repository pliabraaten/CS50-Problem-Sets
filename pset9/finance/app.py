import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from datetime import datetime
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    # Pull data for the user's index table
    indexTable = db.execute(
            "SELECT stock, SUM(shares) as shares FROM portfolio WHERE user_id = ? GROUP BY stock", session["user_id"])
    # "SELECT stock, shares FROM portfolio WHERE user_id = ?", session["user_id"])

    print("user_id:", session["user_id"])

    # Loop through data in the table to lookup price and calculate total value based on share amount
    prices = {}
    values = {}
    total_stocks = 0
    for stock in indexTable:
        symbol = stock["stock"].upper()
        share = stock["shares"]
        lookup_result = (lookup(symbol))
        if lookup_result is not None:
            prices[symbol] = round(lookup_result["price"], 2)
            values[symbol] = round(prices[symbol] * share, 2)
            total_stocks += values[symbol]

    # PASS IN TOTAL USER CASH BALANCE AMOUNT IN USER TABLE
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    print("cash:", cash[0]["cash"])
    cash_balance = float(cash[0]["cash"])

    # PASS IN TOTAL VALUE OF STOCK VALUE AND CASH BALANCE
    grand_total = cash_balance + total_stocks

    print("Formatted cash_balance:", cash_balance)
    print("Formatted grand_total:", grand_total)
    print("Prices:", prices)
    print("Values:", values)

    # Send values stock, shares (from table), current price, and value over to html, cash balance, and total value
    return render_template("index.html", data=indexTable, price=prices, value=values, cash_balance=cash_balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Initialize lookup function confirmation
    lookup_success = False

    # If GET, display buy form
    if request.method == "GET":
        return render_template("buy.html", lookup_success=lookup_success)

    # If POST,
    if request.method == "POST":

        # # Check if lookup or buy button was used
        # if request.form.get("action") == "lookup":

        if request.form.get("action") == "buy":

            # Grab symbol and save it as session variable to reference when the action is buy
            symbol = request.form.get("symbol").upper()
            print("symbol:", symbol)

            # Ensure stock symbol was entered
            if not symbol:
                return apology("please enter stock symbol", 400)

            print("symbol before lookup:", symbol)

            # Lookup symbol
            quote = lookup(symbol)
            if quote is None:
                return apology("invalid ticker symbol", 400)

            shares = request.form.get("shares")
            try:
                shares = float(shares)
                if not shares.is_integer() or shares < 1:
                    return apology("please enter valid amount", 400)
                shares = int(shares)
            except ValueError:
                return apology("please enter valid amount", 400)

            # Calculate total purchase amount
            purchase_amount = float(shares * quote["price"])
            print("purchase_amount:", purchase_amount)
            # Check user has enough cash for purchase
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            if purchase_amount > cash[0]['cash']:
                return apology("not enough money in your account", 400)

            user_check = db.execute("SELECT user_id, stock FROM portfolio WHERE user_id = ? AND stock = ?",
                                    session["user_id"], symbol)

            print("Symbol:", session.get("symbol"))
            print("Shares:", request.form.get("shares"))
            print("Quote Price:", quote["price"])

            # Check if user is not in the table with this stock, insert
            if len(user_check) == 0:
                db.execute("INSERT INTO portfolio (user_id, stock, shares) VALUES (?, ?, ?)",
                            session["user_id"], symbol, request.form.get("shares"))
                # Update the user's cash amount in users table
                db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?",
                            purchase_amount, session["user_id"])

                # Update transaction history table
                db.execute(
                    "INSERT INTO history (user_id, action, stock, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                    session["user_id"], "Purchase", symbol, quote["price"], request.form.get(
                        "shares"), datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                )

            # If that user already has that stock, then update the amount
            else:
                ## -- TEST INSERT NEW LINE INSTEAD OF UPDATING
                db.execute("INSERT INTO portfolio (user_id, stock, shares) VALUES (?, ?, ?)",
                            session["user_id"], symbol, request.form.get("shares"))
                # Update the user's cash amount in users table
                db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?",
                            purchase_amount, session["user_id"])

                # # Update number of shares in portfolio table
                # db.execute("UPDATE portfolio SET shares = (shares + ?) WHERE user_id = ? AND stock = ?",
                #             request.form.get("shares"), session["user_id"], symbol)
                # # Update the user's cash amount in users table
                # db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?",
                #             purchase_amount, session["user_id"])

                # Update transaction history table
                db.execute(
                    "INSERT INTO history (user_id, action, stock, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                    session["user_id"], "Purchase", symbol, quote["price"], request.form.get(
                        "shares"), datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                )

            # Release session variable
            session.pop('symbol', None)

    if request.form.get("action") != "buy":
        return apology("invalid action", 400)

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

# display html table summarizing all user's transactions listing every buy/sell in a row
    # Check if user has rows in their history table
    history_check = db.execute("SELECT user_id FROM history WHERE user_id = ?", session["user_id"])
    if len(history_check) == 0:
        return apology("no recorded transactions", 400)
    else:
        # Pull data for the user's index table
        historyTable = db.execute("SELECT * FROM history WHERE user_id = ?", session["user_id"])

        # Send values - include: bought/sold, symbol, purchase/sale price, shares, date, time
        return render_template("history.html", history=historyTable)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    # If GET, display form to request stock quote lookup
    if request.method == "GET":
        return render_template("quote.html")

    # if POST, lookup stock symbol by calling lookup() and display the results
    if request.method == "POST":

        symbol = request.form.get("symbol").upper()

        # Check if symbol was entered
        if not symbol:
            return apology("please enter symbol", 400)

        # If lookup() is successful, returns dictionary {"price": price, "symbol": symbol}
        quote = lookup(symbol)
        if quote:
            # Pass in values into template (use jinja template)
            return render_template("quoted.html", price=quote["price"], symbol=quote["symbol"], usd=usd)
        else:
            # If lookup() does not find a symbol, return apology that stock symbol doesn't exist
            return apology("stock symbol doesn't exist", 400)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # If GET, then render the register page and form
    if request.method == "GET":
        return render_template("register.html")

    # When register form is submitted (POST),
    if request.method == "POST":

        # Forget any user_id
        session.clear()

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username was entered
        if not username:
            return apology("please enter username", 400)

        # Check if password or password confirmation are blank
        if not password or not confirmation:
            return apology("enter password twice", 400)
        # Check if passwords match
        if password != confirmation:
            return apology("passwords must match", 400)

        # Insert username to db and check if username already exists
        try:
            hashed_password = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)
        except Exception as e:
            print(e)  # This will print the exception to the console
            return apology("username already exists", 400)

        # Log user in
        rows = db.execute("SELECT id FROM users WHERE username = ?", username)
        print(f"Rows: {rows}")  # Debugging print
        session["user_id"] = rows[0]["id"]

        # Redirect user to homepage
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # If GET, then render the sell page and form
    if request.method == "GET":

        # HAVE USER SELECT STOCK FROM SELECT MENU
        # Pull stocks that the user owns
        stocks = db.execute("SELECT stock FROM portfolio WHERE user_id = ?", session["user_id"])
        # Pass these stocks to the html
        return render_template("sell.html", stocks=stocks)

    # If POST,
    if request.method == "POST":

        # Check if lookup or sell button was used
        if request.form.get("action") == "lookup":
            # Grab symbol and save it as session variable to reference when the action is sell
            symbol = request.form.get("symbol")

            # Make sure the user owns shares fo the selected stock
            stock_check = db.execute(
                "SELECT user_id, stock FROM portfolio WHERE user_id = ? AND stock = ?", session["user_id"], symbol)
            if len(stock_check) == 0:
                return apology("please select stock symbol", 400)

            # Lookup price for the selected stock
            quote = lookup(symbol)
            if quote:
                # Pull stocks that the user owns
                stocks = db.execute(
                    "SELECT stock FROM portfolio WHERE user_id = ?", session["user_id"])
                # Pass in values into template (use jinja template)
                return render_template("sell.html", price=quote["price"], symbol=quote["symbol"], usd=usd, stocks=stocks)
            else:
                return apology("error finding stock price", 400)

        # LOGIC FOR WHEN SELL BUTTON IS USED
        # Check if sell button was used
        elif request.form.get("action") == "sell":
            # Ensure share amount was entered, convertable to an int, and positive
            shares = request.form.get("shares")
            try:
                shares = int(shares)
            except ValueError:
                return apology("please enter valid amount", 400)
            if shares < 1:
                return apology("please enter valid amount", 400)

            # Lookup symbol and current price
            session["symbol"] = request.form.get("symbol")
            quote = lookup(session["symbol"].upper())

            # Calculate total sell amount
            sale_amount = float(shares * quote["price"])
            print("sale_amount:", sale_amount)

            # Update the tables after sale
            # Check if share amount is greater than amount of shares owned
            share_check = db.execute(
                "SELECT shares FROM portfolio WHERE user_id = ? AND stock = ?", session["user_id"], session["symbol"])
            print("shares owned:", share_check[0]["shares"])

            # Update transaction history table
            db.execute(
                "INSERT INTO history (user_id, action, stock, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                session["user_id"], "Sale", session["symbol"], quote["price"],
                shares * -1, datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            )

            if share_check[0]["shares"] >= shares:
                # Update number of shares in portfolio table
                db.execute("UPDATE portfolio SET shares = (shares - ?) WHERE user_id = ? AND stock = ?",
                           shares, session["user_id"], session["symbol"])
                # Update the user's cash amount in users table
                db.execute("UPDATE users SET cash = (cash + ?) WHERE id = ?",
                           sale_amount, session["user_id"])
            else:
                return apology("you do not own that many", 400)

            # Get current number of shares in the table
            rows = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND stock = ?",
                              session["user_id"], session["symbol"])
            shares_left = rows[0]["shares"]
            print("shares left:", shares_left)

            # If shares goes to 0, then delete row
            if shares_left == 0:
                db.execute("DELETE FROM portfolio WHERE user_id = ? AND stock = ?",
                           session["user_id"], session["symbol"])

            # Release session variable
            session.pop('symbol', None)

            return redirect("/")


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():

    if request.method == "GET":
        return render_template("add.html")

    # ALLOW USER TO ADD MORE CASH TO THEIR BALANCE
    if request.method == "POST":
        # Ensure cash amount was entered, convertable to an int, and positive
        added_cash = request.form.get("cash")
        try:
            added_cash = int(added_cash)
        except ValueError:
            return apology("please enter valid amount", 400)
        if added_cash < 1:
            return apology("please enter positive amount", 400)

        # Check if user exists
        # Update user table with amount of cash added
        db.execute("UPDATE users SET cash = (cash + ?) WHERE id = ?",
                   added_cash, session["user_id"])

        # Update transaction history table
        db.execute(
            "INSERT INTO history (user_id, action, stock, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
            session["user_id"], "Added Cash", "-", added_cash, "-", datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        )

    return redirect("/history")
