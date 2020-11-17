    # Update table of users stocks grouped by company



    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        # Ensure stock name was selected
        if not symbol:
            return apology("must provide a stock symbol")

        # Ensure they own shares for the stock they want to sell
        elif not db.execute("SELECT symbol FROM current_stocks WHERE symbol = :symbol", symbol = symbol)[0]['symbol']:
            return apology("you do not own any shares of this stock")

        shares = request.form.get("shares")
        # Ensure a positive integer entered for number of shares
        if shares < 1:
            return apology("Please provide a valid no. of shares")

        # Ensure they have enough shares to sell
        elif db.execute("SELECT shares FROM user_stocks WHERE symbol = :symbol", symbol = symbol)[0]['shares'] < shares:
            return apology("you do not own enough shares of this stock")



        # Calculate value of transaction and users current balance
        value = (lookup(symbol))["price"] * shares

        current_balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash']


    # Redirect user to homepage
    return redirect("/")