import websocket

websocket.WebSocket.create

def on_message(ws, message):
    print("Received:", message)

def on_open(ws):
    print("WebSocket connection opened")
    ws.send("Hello, WebSocket Server!")

def on_close(ws, close_status_code, close_msg):
    print("WebSocket connection closed")

if __name__ == "__main__":
    websocket_url = "ws://localhost:8888/websocket"  # Replace with your server's URL
    
    ws = websocket.WebSocketApp(websocket_url,
                                on_message=on_message,
                                on_open=on_open,
                                on_close=on_close)
    
    ws.run_forever()
