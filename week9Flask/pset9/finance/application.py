import os

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
    # find the user infomation by user_id
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    userName = rows[0]["username"]
    userCash = rows[0]["cash"]

    # create a new table which named by username, if not exists
    db.execute("CREATE TABLE IF NOT EXISTS ?(deal_id INTEGER, uid INTEGER, date TIMESTAMP, BuyorSell TEXT NOT NULL, company TEXT NOT NULL, symbol TEXT NOT NULL, price NUMERIC NOT NULL, share NUMERIC NOT NULL, total NUMERIC NOT NULL,PRIMARY KEY(deal_id), FOREIGN KEY(uid) REFERENCES users(id))", userName)

    # store the stock company name and shares
    holding = {}
    operates = db.execute("SELECT symbol, BuyorSell, SUM(share) as S FROM ? GROUP BY symbol, BuyorSell", userName)

    # calculate the shard of each stock after all operate has done
    for op in operates:
        if not holding.__contains__(op["symbol"]):
            holding[op["symbol"]] = 0
        if op["BuyorSell"] == "Buy":
            holding[op["symbol"]] += op["S"]
        elif op["BuyorSell"] == "Sell":
            holding[op["symbol"]] -= op["S"]

    # store the hoding stock information
    stocks = []

    # store all stock value
    totalStock = 0

    for com_symbol, hold in holding.items():
        if hold != 0:
            stock = lookup(com_symbol)
            stock["hold_share"] = hold
            stock["total_value"] = hold * stock["price"]
            totalStock += stock["total_value"]
            stocks.append(stock)

    # store the cash balance.
    CashBalance = {}
    CashBalance["cash"] = rows[0]["cash"]
    CashBalance["total"] = CashBalance["cash"] + totalStock
    return render_template("index.html", stocks=stocks, CashBalance=CashBalance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # if submit a form
    if request.method == "POST":
        """Buy shares of stock"""

        # get the stock symbol
        symbol = request.form.get("symbol")

        # ensure symbol has value
        if not symbol:
            return apology("Must provide stock symbol", 400)

        # lookup the stock
        stock = lookup(symbol)

        # ensure has a valid stock
        if not stock:
            return apology("Invalid stock symbol", 400)

        # get the number of share
        shares = request.form.get("shares")
        if not shares.isdigit():
            return apology("Must provide numeric shares.", 400)

        shares = float(shares)

        # ensure shares is positive number
        if shares <= 0:
            return apology("Must provide positive shares", 400)

        # calculate the total value user want to buy
        userBuy = shares * stock["price"]

        # get the user information
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        userName = rows[0]["username"]
        userCash = rows[0]["cash"]

        # ensure user has enough money to buy
        if userBuy > userCash:
            return apology("Your cash is not enough to buy", 400)

        # create table if not exist
        db.execute("CREATE TABLE IF NOT EXISTS ?(deal_id INTEGER, uid INTEGER, date TIMESTAMP, BuyorSell TEXT NOT NULL, company TEXT NOT NULL, symbol TEXT NOT NULL, price NUMERIC NOT NULL, share NUMERIC NOT NULL, total NUMERIC NOT NULL,PRIMARY KEY(deal_id), FOREIGN KEY(uid) REFERENCES users(id))", userName)

        # store the information in a table named by username
        db.execute("INSERT INTO ? (uid, date, BuyorSell,company, symbol, price, share, total) VALUES(?, datetime('now'), ?, ?, ?, ?, ?, ?)",
                   userName, session["user_id"], "Buy", stock["name"], stock["symbol"], stock["price"], shares, userBuy)

        # update the user cash
        userCash = userCash - userBuy
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash, session["user_id"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # find the user infomation by user_id
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    userName = rows[0]["username"]

    # SELECT the transaction in the table named by username
    stockHistory = db.execute("SELECT * FROM ?", userName)

    return render_template("history.html", stockHistory=stockHistory)


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
            return apology("must provide password", 404)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 405)

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

        # get the stock symbol wanted to quote
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not symbol:
            return apology("The stock symbol is empty", 400)
        # ensure symbol is valid
        if not stock:
            return apology("The stock was not found", 400)

        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # get the username
        name = request.form.get("username")

        # ensure the username has value
        if not name:
            return apology("Name can not be blank", 400)

        # ensure the username can be user
        checkName = db.execute("SELECT * FROM users WHERE username = ?", name)
        if len(checkName) != 0:
            return apology("Username already exists", 400)

        # get the password
        pwd = request.form.get("password")
        ConfiPwd = request.form.get("confirmation")

        # ensure the name and
        if not pwd or not ConfiPwd:
            return apology("Password can not be blank", 400)

        # ensure type the same password
        if pwd != ConfiPwd:
            return apology("Type the same password in both text boxes.", 400)

        # register
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", name, generate_password_hash(pwd))

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # get the information of user
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    userName = rows[0]["username"]
    userCash = rows[0]["cash"]

    # check the table is exists
    db.execute("CREATE TABLE IF NOT EXISTS ?(deal_id INTEGER, uid INTEGER, date TIMESTAMP, BuyorSell TEXT NOT NULL, company TEXT NOT NULL,symbol TEXT NOT NULL, price NUMERIC NOT NULL, share NUMERIC NOT NULL, total NUMERIC NOT NULL,PRIMARY KEY(deal_id), FOREIGN KEY(uid) REFERENCES users(id))", userName)

    # get all the transaction history
    operates = db.execute("SELECT symbol, BuyorSell, SUM(share) as S FROM ? GROUP BY symbol, BuyorSell", userName)

    # calculate the holding stock and share
    holding = {}
    for op in operates:
        if not holding.__contains__(op["symbol"]):
            holding[op["symbol"]] = 0
        if op["BuyorSell"] == "Buy":
            holding[op["symbol"]] += op["S"]
        elif op["BuyorSell"] == "Sell":
            holding[op["symbol"]] -= op["S"]

    # select the stock can be selled
    symbols = []
    for com_symbol, hold in holding.items():
        if hold > 0:
            symbols.append(com_symbol)

    # if user want to sell
    if request.method == "POST":

        # get the stock symbol
        stock = request.form.get("symbol")

        # ensure the symbol is valid
        if stock not in symbols:
            return apology("You don't have this stock", 601)

        # ensure the shares is positive
        sell_shares = float(request.form.get("shares"))
        if sell_shares <= 0:
            return apology("Must provide positive shares", 602)

        # get the information of the stock
        current_stock = lookup(stock)

        # ensure has enough share to sell
        if sell_shares > holding[current_stock["symbol"]]:
            return apology("You don't have enough shares", 400)

        # calculate how much user can geted
        sellstock = sell_shares * current_stock["price"]

        # record this sell behavior
        db.execute("INSERT INTO ? (uid, date, BuyorSell, company, symbol, price, share, total) VALUES(?, datetime('now'), ?, ?, ?, ?, ?, ?)",
                   userName, session["user_id"], "Sell", current_stock["name"], current_stock["symbol"], current_stock["price"], sell_shares, sellstock)

        # calculate the cash after sell
        userCash = userCash + sellstock

        # update the cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash, session["user_id"])
        return redirect("/")
    else:
        return render_template("sell.html", symbols=symbols)


@app.route("/changepwd", methods=["GET", "POST"])
@login_required
def changepwd():

    if request.method == "POST":

        # get the old password
        oldPWD = request.form.get("oldPWD")

        # ensure old password is not empty
        if not oldPWD:
            return apology("Must provide old password", 701)

        # get the new password
        pwd = request.form.get("password")
        confirm = request.form.get("confirm")

        # ensure pwd and confirm not empty
        if not pwd or not confirm:
            return apology("Must provide new password", 702)

        # get the user from users table
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # ensure old password match
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], oldPWD):
            return apology("invalid old password", 703)

        # ensure type the same new password
        if confirm != pwd:
            return apology("Type the same password in both text boxes.", 704)

        # change the password in the users table
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(pwd), session["user_id"])
        return render_template("login.html")
    else:
        return render_template("pwd.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
