from flask import Flask, request
from flask.templating import render_template
from flask.helpers import make_response
from sympy import symbols,integrate
from sympy.parsing.sympy_parser import (parse_expr)
from flaskwebgui import FlaskUI

app = Flask(__name__)
ui = FlaskUI(app)

@app.route('/', methods=['POST', 'GET'])
def home(name=None):
    return render_template('index.html', name=name)


@app.route('/download')
def calculate():
    f = parse_expr(request.args['f'])
    y0=int(request.args['y'])
    n=int(request.args['n'])
    x, y= symbols('x y')
    s=[y0]
    for i in range(1,n):
        s.append(y0+integrate(f.subs(y,s[i-1]), x))
    ans=""
    for i in range(0,n):
        ans+=f"n={i+1} : y({i}) = {s[i]}\n"

    response = make_response(ans)
    response.headers.set('Content-Type', 'text/plain')
    response.headers.set(
        'Content-Disposition', 'attachment', filename='answer.txt')
    return response


if __name__ == "__main__":
    # app.secret_key = "super secret key"
    # app.run("127.0.0.1",5002)
    ui.run()
