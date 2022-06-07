import http.client

conn = http.client.HTTPConnection("178.242.9.4")
payload = "{\"Request\":{\"Event\":256}}"
headers = {}
conn.request("POST", "/", payload, headers)
res = conn.getresponse()
data = res.read()
print(data.decode("utf-8"))