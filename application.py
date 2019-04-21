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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # go ahead and pull the current price, holding value, grand total to just update later
    sqls = "select symbol, cash, sum(shares) shares, null current_price, null holding_value, null grand_total "
    sqls += "from users u, transactions t where u.id = t.user_id and u.id = :user_id group by symbol, cash "

    rows = db.execute(sqls, user_id=session["user_id"])
    # need to handle if the user hasn't made any transactions yet
    if len(rows) != 0:
        grand_total = rows[0]["cash"]

        for row in rows:
            stock = lookup(row["symbol"])
            row["current_price"] = usd(stock["price"])
            row["holding_value"] = usd(stock["price"] * row["shares"])
            grand_total += stock["price"] * row["shares"]

    else:
        sqls = "select null symbol, cash, null shares, null current_price, null holding_value, null grand_total "
        sqls += "from users u where u.id = :user_id "
        rows = db.execute(sqls, user_id=session["user_id"])
        grand_total = rows[0]["cash"]

    rows[0]["cash"] = usd(rows[0]["cash"])
    rows[0]["grand_total"] = usd(grand_total)

    return render_template("index.html", rows=rows)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))

        if stock == "None" or not stock:
            return apology("Stock Symbol does not exist!", 400)

        rows = db.execute("SELECT cash from users where id = :user_id", user_id=session["user_id"])

        # double check the shares got entered properly
        try:
            if int(request.form.get("shares")) < 1 or float(request.form.get("shares")) != int(request.form.get("shares")):
                return apology("Shares must be positive integer!")

        except ValueError:
            return apology("Shares must be an integer!")

        shares = int(request.form.get("shares"))

        price = stock["price"]

        if shares * price > rows[0]["cash"]:
            return apology("You do not have enough cash to buy stock at current price!", 400)

        rows = db.execute("INSERT INTO transactions(user_id, transaction_type_id, symbol, price, shares, total) VALUES(:user_id, 1, :symbol, :price, :shares, :total)",
                          user_id=session["user_id"], symbol=stock["symbol"], price=price,
                          shares=shares, total=shares * price
                          )

        rows = db.execute("UPDATE users SET cash = cash - :total", total=shares*price)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    username = request.args.get("username")
    # only check if they've entered a value
    if len(username) >= 1:
        rows = db.execute("select * from users where username = :username", username=username)
        if len(rows) != 0:
            return jsonify(False)
        else:
            return jsonify(True)

    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    sqls = "select symbol, shares, description transaction_type, price, total total_price, transaction_date "
    sqls += "from transactions t, transaction_type d where t.transaction_type_id = d.transaction_type_id "
    sqls += "and t.user_id = :user_id"

    rows = db.execute(sqls, user_id=session["user_id"])

    for row in rows:
        row["price"] = usd(row["price"])
        row["total_price"] = usd(row["total_price"])

    return render_template("history.html", rows=rows)


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
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        if stock == "None" or not stock:
            return apology("Stock Symbol does not exist!", 400)

        stock["price"] = usd(stock["price"])

        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Mismatched passwords", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 0:
            return apology("Username already exists", 400)

        hashed = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        rows = db.execute("INSERT INTO users(username, hash) VALUES (:username, :password)",
                          username=request.form.get("username"), password=hashed)

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))

        if stock == "None" or not stock:
            return apology("Stock Symbol does not exist!", 400)

        rows = db.execute("SELECT sum(shares) shares from transactions where user_id = :user_id and symbol = :symbol",
                          user_id=session["user_id"], symbol=stock["symbol"])

        # confirm shares is valid
        try:
            if int(request.form.get("shares")) < 1 or float(request.form.get("shares")) != int(request.form.get("shares")):
                return apology("Shares must be positive integer!")

        except ValueError:
            return apology("Shares must be an integer!")

        shares = int(request.form.get("shares"))

        price = stock["price"]

        if shares > rows[0]["shares"]:
            return apology("You do not have that many shares to sell!", 400)

        rows = db.execute("INSERT INTO transactions(user_id, transaction_type_id, symbol, price, shares, total) VALUES(:user_id, 2, :symbol, :price, :shares, :total)",
                          user_id=session["user_id"], symbol=stock["symbol"], price=price,
                          shares=-shares, total=shares * price
                          )

        rows = db.execute("UPDATE users SET cash = cash + :total", total=shares*price)

        return redirect("/")
    else:

        rows = db.execute("SELECT symbol from transactions where user_id = :user_id group by symbol having sum(shares) <> 0",
                          user_id=session["user_id"])

        return render_template("sell.html", rows=rows)


@app.route("/changepw", methods=["GET", "POST"])
def changepw():
    """Change user's password"""

    if request.method == "POST":
        # Ensure current password was submitted
        if not request.form.get("currpassword"):
            return apology("must provide current password", 400)

        # Ensure password was submitted
        elif not request.form.get("newpassword"):
            return apology("must provide new password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

        # Ensure username exists and password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("currpassword")):
            return apology("Incorrect current password", 400)

        hashed = generate_password_hash(request.form.get("newpassword"), method='pbkdf2:sha256', salt_length=8)
        rows = db.execute("UPDATE users SET hash = :password", password=hashed)

        return render_template("changedpw.html")
    else:
        return render_template("changepw.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
