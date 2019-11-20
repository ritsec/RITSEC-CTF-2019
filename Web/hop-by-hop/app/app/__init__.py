from flask import Flask
from flask_talisman import Talisman
from flask_seasurf import SeaSurf

def create_app():
    app = Flask(__name__)
    #csrf = SeaSurf(app)
    csp = {
        'script-src': '\'self\' \'unsafe-eval\' dal2iwprittp7.cloudfront.net code.jquery.com',
        'default-src': '\'self\' dal2iwprittp7.cloudfront.net',
        'style-src': '\'self\' dal2iwprittp7.cloudfront.net fonts.googleapis.com fonts.gstatic.com',
        'font-src': '\'self\' use.fontawesome.com dal2iwprittp7.cloudfront.net fonts.googleapis.com fonts.gstatic.com data:',
        'img-src': '\'self\' dal2iwprittp7.cloudfront.net data:',
        'worker-src': '\'self\' blob:'
    }
    Talisman(app, \
        strict_transport_security=False, \
        force_https=False, \
        session_cookie_secure=False, \
        content_security_policy=csp)
    app.config.from_pyfile('settings.py')
    #app.secret_key = 'gthSckChOFUF0bclBO3s'


     # import blueprints
    from .base.views import base_app

    # register blueprints
    app.register_blueprint(base_app)

    return app

app = create_app()
