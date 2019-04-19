import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import * #apology, login_required, lookup, usd

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

    user_id = session["user_id"]

    #user’s current cash balance along with a grand total (i.e., stocks' total value plus cash).
    rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
    cash = rows[0]["cash"]
    grandtotal = rows[0]["cash"]

    stocks = db.execute("SELECT stock_symbol, shares FROM portfolio WHERE user_id = :user_id", user_id=user_id)

    # set up values to pull into table for the user
    for stock in stocks:
        stock["symbol"] = stock["stock_symbol"].upper()
        stock["shares"] = int(stock["shares"])
        quote = lookup(stock["stock_symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["total"] = quote["price"] * stock["shares"]
        grandtotal += stock["total"]

    return render_template("index.html", stocks = stocks, cash = usd(cash), grandtotal = usd(grandtotal))
    #return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))

        # Ensure valid symbol was submitted
        if not symbol:
            return apology("must enter symbol")

        # Ensure valid # of shares was submitted
        elif not shares or shares <= 0:
            return apology("must enter number of shares")


        # get a valid stock symbol from the user
        quote = lookup(symbol)

        if not quote:
            return apology("symbol invalid")

        # check user's cash on hand
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=user_id)

        # check if user has enough cash
        price = quote["price"] * shares

        if cash[0]["cash"] >= price:
            #record transaction and update portfolio
            transaction = db.execute("INSERT INTO records (user_id, stock_symbol, shares, price) VALUES (:user_id, :stock_symbol, :shares, :price)",\
                                    user_id=user_id,\
                                    stock_symbol=symbol,\
                                    shares=shares,\
                                    price=price)
            rows = db.execute("SELECT * FROM portfolio where user_id = :user_id AND stock_symbol = :stock_symbol",\
                                user_id=user_id,stock_symbol=symbol)
            if len(rows) == 0:
                db.execute("INSERT INTO portfolio (user_id,stock_symbol,shares) VALUES (:user_id,:stock_symbol,:shares)",\
                            user_id=user_id,stock_symbol=symbol,shares=shares)
            else:
                db.execute("UPDATE portfolio SET shares = shares + :shares WHERE user_id = :user_id AND stock_symbol = :stock_symbol",\
                            user_id=user_id,stock_symbol=symbol,shares=shares)


            #update cash
            update_cash = db.execute("UPDATE users SET cash = cash - :price WHERE id = :user_id",\
                                    price=price, user_id=user_id)
            flash('Bought!')
            #route to home page
            return redirect("/")

        else:
            return apology("not enough cash")

    # User reached route via GET
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session["user_id"]
    records = db.execute("SELECT stock_symbol, shares, price, date FROM records WHERE user_id = :user_id", user_id=user_id)

    # set up values to pull into table for the user
    for record in records:
        record["symbol"] = record["stock_symbol"].upper()
        record["shares"] = int(record["shares"])
        quote = lookup(record["stock_symbol"])
        record["price"] = usd(quote["price"])
        record["date"] = record["date"]

    return render_template("history.html", records = records)


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

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must enter symbol")

        # get a valid stock symbol from the user
        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("symbol invalid")

        return render_template("quoted.html", quote=quote)

    # User reached route via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password (again)")

        #check that passwords match, then encrypt
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords must match")

         #enure unique username
        new_user_id = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",\
                                username=request.form.get("username"),\
                                hash=generate_password_hash(request.form.get("password")))
        if not new_user_id:
            return apology("Username taken")

        #once successfully registered, log in automatically
        session["user_id"] = new_user_id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))

        # Ensure valid symbol was submitted
        if not symbol:
            return apology("must enter symbol")

        # Ensure valid # of shares was submitted
        elif not shares or shares <= 0:
            return apology("must enter number of shares")


        # get a valid stock symbol from the user
        quote = lookup(symbol)

        if not quote:
            return apology("symbol invalid")

        # setting variables
        user_id = session["user_id"]
        price = quote["price"] * shares

        #check if user has enough shares to sell
        user_shares = db.execute("SELECT shares FROM portfolio where user_id = :user_id AND stock_symbol = :stock_symbol",\
                                user_id=user_id,stock_symbol=symbol)

        #update records and protfolio accordingly
        if user_shares[0]["shares"] >= shares:

            #record transaction
            transaction = db.execute("INSERT INTO records (user_id, stock_symbol, shares, price) VALUES (:user_id, :stock_symbol, :shares, :price)",\
                                    user_id=user_id,\
                                    stock_symbol=symbol,\
                                    shares=-shares,\
                                    price=price)

            # remove from portfolio if selling all shares
            if user_shares[0]["shares"] == shares:

                db.execute("DELETE FROM portfolio WHERE user_id = :user_id AND stock_symbol = :stock_symbol",\
                                user_id=user_id,stock_symbol=symbol)

            # update portfolio if not selling all shares
            else:
                db.execute("UPDATE portfolio SET shares = shares - :shares WHERE user_id = :user_id AND stock_symbol = :stock_symbol",\
                        user_id=user_id,stock_symbol=symbol,shares=shares)

           #update cash
            update_cash = db.execute("UPDATE users SET cash = cash + :price WHERE id = :user_id",\
                                        price=price, user_id=user_id)
            flash('Sold!')

        else:
            return apology("not enough shares")


        #route to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("sell.html")



@app.route("/sell2", methods=["GET", "POST"])
@login_required
def sell2():
    """Sell shares of stock"""

    if request.method == "POST":
        
        user_id = session["user_id"]
        
        records = db.execute("SELECT shares FROM portfolio where user_id = :user_id",user_id=user_id)
        
        for record in records:
            record["symbol"] = record["stock_symbol"].upper()
        
        symbol = record["symbol"]            
        shares = int(request.form.get("shares"))

        # Ensure valid symbol was submitted
        if not symbol:
            return apology("must enter symbol")

        # Ensure valid # of shares was submitted
        elif not shares or shares <= 0:
            return apology("must enter number of shares")


        # get a valid stock symbol from the user
        quote = lookup(symbol)

        if not quote:
            return apology("symbol invalid")

        # setting variables
        price = quote["price"] * shares

        #check if user has enough shares to sell
        user_shares = db.execute("SELECT shares FROM portfolio where user_id = :user_id AND stock_symbol = :stock_symbol",\
                                user_id=user_id,stock_symbol=symbol)

        #update records and protfolio accordingly
        if user_shares[0]["shares"] >= shares:

            #record transaction
            transaction = db.execute("INSERT INTO records (user_id, stock_symbol, shares, price) VALUES (:user_id, :stock_symbol, :shares, :price)",\
                                    user_id=user_id,\
                                    stock_symbol=symbol,\
                                    shares=-shares,\
                                    price=price)

            # remove from portfolio if selling all shares
            if user_shares[0]["shares"] == shares:

                db.execute("DELETE FROM portfolio WHERE user_id = :user_id AND stock_symbol = :stock_symbol",\
                                user_id=user_id,stock_symbol=symbol)

            # update portfolio if not selling all shares
            else:
                db.execute("UPDATE portfolio SET shares = shares - :shares WHERE user_id = :user_id AND stock_symbol = :stock_symbol",\
                        user_id=user_id,stock_symbol=symbol,shares=shares)

           #update cash
            update_cash = db.execute("UPDATE users SET cash = cash + :price WHERE id = :user_id",\
                                        price=price, user_id=user_id)
            flash('Sold!')

        else:
            return apology("not enough shares")


        #route to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("sell2.html")






def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
