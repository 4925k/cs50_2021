import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        # get form values
        name = request.form.get("name")
        day = request.form.get("day")
        month = request.form.get("month")
        # check for empty values
        if not name or not month or not day:
            return "missing fields"
        # add birthday to birthdays
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
        return redirect("/")
    else:
        # TODO: Display the entries in the database on index.html
        # fetch table from birthdays db
        birthdays = db.execute("SELECT * FROM birthdays")
        # pass birthdays table and render index.html
        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=["POST"])
def delete():
    name=request.args.get("name")
    month=request.args.get("month")
    day=request.args.get("day")
    db.execute("DELETE FROM birthdays WHERE name=? AND day=? AND month=?", name, day, month)
    return redirect("/")


@app.route("/edit", methods=["GET", "POST"])
def edit():
    if request.method == "GET":
        bid=request.args.get("id")
        print(bid)
        birthday = db.execute("SELECT * FROM birthdays WHERE id=?", bid)
        return render_template("edit.html", birthday=birthday[0])
    else:
        bid = request.args.get("id")
        name = request.form.get("name")
        day = request.form.get("day")
        month = request.form.get("month")
        db.execute("UPDATE birthdays SET name=?,month=?,day=? WHERE id=?", name, month, day, bid)
        return redirect("/")

