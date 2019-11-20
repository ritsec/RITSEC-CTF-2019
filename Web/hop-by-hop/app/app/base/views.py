from flask import Blueprint, render_template, request

base_app = Blueprint('base_app', __name__)

@base_app.route('/', methods=['GET'])
def index():
    return render_template('index.html')

@base_app.route('/blocked', methods=['GET'])
def blocked():
    return "You have been stopped by The Good Good Proxy WAF"

@base_app.route('/admin', methods=['GET'])
def verify():
    allowed_ips = ["direct", "8.8.8.8", "1.1.1.1"]
    try:
        source_ip = request.headers['X-Forwarded-For']
    except KeyError:
        source_ip = "direct"
    print(source_ip)
    if source_ip not in allowed_ips:
        return render_template('bad.html', ip_addr=source_ip)
    return render_template('admin.html')

@base_app.route('/logoout', methods=['GET'])
def logout():
    return "You have been stopped by 2Good2WAF"
