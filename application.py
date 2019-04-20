import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # check and see if anything is missing for good measure
    if not request.form.get("name") or not request.form.get("leigelord") or not request.form.get("weapon") or not request.form.get("why"):
        return render_template("error.html", message="Missing Values")
    # write to the file
    with open("survey.csv", "a") as file:
        writer = csv.DictWriter(file, fieldnames=["name", "leigelord", "weapon", "why", "otherreason"])
        writer.writerow(
            {"name": request.form.get("name"),
                "leigelord": request.form.get("leigelord"),
                "weapon": request.form.get("weapon"),
                "why": request.form.get("why"),
                "otherreason": request.form.get("otherreason")
             })
    return get_sheet()


@app.route("/sheet", methods=["GET", "POST"])
def get_sheet():

    with open("survey.csv", "r") as file:
        reader = csv.DictReader(file)
        watch = list(reader)
    return render_template("sheet.html", watch=watch)

