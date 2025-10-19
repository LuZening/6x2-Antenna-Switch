import asyncio
import websockets

HOST = '127.0.0.1'
PORT = 80
DISCONNECT_INTERVAL = 160  # 秒

async def disconnect_after(websocket):
    await asyncio.sleep(DISCONNECT_INTERVAL)
    print("为了测试，断开客户端连接。")
    await websocket.close()

async def handler(websocket):
    # 启动定时断开任务
    disconnect_task = asyncio.create_task(disconnect_after(websocket))
    try:
        while True:
            try:
                msg : str = await websocket.recv()
            except websockets.exceptions.ConnectionClosed:
                print("客户端已断开连接。")
                break

            print(f"收到客户端消息: {msg}")

            if msg.find("URI=/getalloc") >= 0:
                response = "URI=/getalloc&sel1=3&sel2=6"
            elif msg.find("URI=/switch") >= 0:
                response = "URI=/switch&response=OK"
            elif msg.find("URI=/getlabel") >= 0:
                response = "URI=/getlabel&ant1=ant1&ant2=ant2&ant3=ant3&ant4=ant4&ant5=ant5&ant6=ant6"
            elif msg.find("URI=/setlabel") >= 0:
                response = "URI=/setlabel&response=OK"
            elif msg.find("URI=/getport") >= 0:
                response = "URI=/getport&portHTTP=80&portTCP=23"
            elif msg.find("URI=/setport") >= 0:
                response = "URI=/setport&response=OK"
            elif msg.find("ping") >= 0:
                response = "pong"
            else:
                response = "Unknown command"

            await websocket.send(response)
    finally:
        disconnect_task.cancel()

async def main():
    async with websockets.serve(handler, HOST, PORT):
        print(f"WebSocket服务器已启动，地址为 ws://{HOST}:{PORT}")
        await asyncio.Future()  # 一直运行

if __name__ == "__main__":
    asyncio.run(main())
