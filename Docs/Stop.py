import http.client

conn = http.client.HTTPConnection("178.242.8.147")
payload = "{\"Request\":{\"Event\":257}}"
headers = {}
conn.request("POST", "/", payload, headers)
res = conn.getresponse()
data = res.read()
print(data.decode("utf-8"))