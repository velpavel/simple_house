from flask import Flask, render_template, request
import sqlite3

from configs import db_file

app = Flask(__name__)
app.config['SECRET_KEY'] = 'top_secret_KeY1!2!'

@app.route("/")
def main():
    sql = '''select cs.device_id, d.comment, cs.current_val, cs.download 
            from current_status cs, devices d where cs.device_id=d.id'''
    conn = sqlite3.connect(db_file)
    c = conn.cursor()
    c.execute(sql)
    result = c.fetchall()
    conn.close()
    return render_template('main.html', status = result)

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=8181)