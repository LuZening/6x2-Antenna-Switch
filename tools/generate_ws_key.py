
import hashlib
import base64

def generate_ws_accept(key):
    magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
    combined = key + magic
    sha1 = hashlib.sha1(combined.encode()).digest()
    return base64.b64encode(sha1).decode()

client_key = "hU4MBoPgbPjZsPbd4uTbmw=="
accept_key = generate_ws_accept(client_key)
print(f"Sec-WebSocket-Accept: {accept_key}")
