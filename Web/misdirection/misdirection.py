"""
@filename: misdirection.py

@Description:
This is a simple flask app that redirects through a number of 
pages in order to get a flag.

The flag is RS{4!way5_ke3p-m0v1ng}

@Author: Jon Bauer (JonBauer123)
"""
### Python Imports
from flask import Flask, render_template, redirect, url_for, request, abort

### Setting up the Flask App, Login Manager, and Database
app = Flask(__name__, template_folder="templates")

### The Flask Apps Code
@app.route('/')
def home():
    return redirect('/R')

@app.route('/R')
def R():
    return redirect('/S')

@app.route('/S')
def S():
    return redirect('/{')

@app.route('/{')
def open():
    return redirect('/4')

@app.route('/4')
def four():
    return redirect('/!')

@app.route('/!')
def ex():
    return redirect('/w')

@app.route('/w')
def w():
    return redirect('/a')

@app.route('/a')
def a():
    return redirect('/y')

@app.route('/y')
def y():
    return redirect('/5')

@app.route('/5')
def five():
    return redirect('/_')

@app.route('/_')
def under():
    return redirect('/K')

@app.route('/K')
def K():
    return redirect('/e')

@app.route('/e')
def e():
    return redirect('/3')

@app.route('/3')
def three():
    return redirect('/p')

@app.route('/p')
def p():
    return redirect('/-')

@app.route('/-')
def dash():
    return redirect('/m')

@app.route('/m')
def m():
    return redirect('/0')

@app.route('/0')
def zero():
    return redirect('/v')

@app.route('/v')
def v():
    return redirect('/1')

@app.route('/1')
def one():
    return redirect('/n')

@app.route('/n')
def n():
    return redirect('/g')

@app.route('/g')
def g():
    return redirect('/}')

@app.route('/}')
def close():
    return redirect('/R')

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0')