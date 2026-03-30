# Network Protocol Documentation

## 2024-xx-xx - Fix HTTP Connection Leak

### Problem
TCP connections were not being closed properly after HTTP responses. Using netstat, the user observed many ESTABLISHED connections that never disappeared.

### Root Cause 1
The `HTTPclose()` function in `HTTPServer.c` only cleared internal flags (RX_received, socket_connected, SOCK_responding) but did NOT call `CH395TCPDisconnect()` to actually close the TCP connection.

### Solution 1
Added `CH395TCPDisconnect(i)` call in the `HTTPclose()` function within USER CODE section.

### Modified Files
- `MCU2/Src/HTTPServer.c`

### Code Change 1
```c
void HTTPclose(uint8_t i)
{
    /* USER CODE BEGIN HTTPclose */
    // Actually disconnect the TCP connection to release socket resources
    CH395TCPDisconnect(i);
    /* USER CODE END HTTPclose */
    ch395.RX_received &= ~(1 << i);
    ch395.socket_connected &= ~(1 << i);
    ch395.SOCK_responding = -1;
    parseStates[i - 1].connection = CLOSED;
}
```

### Root Cause 2
When HTTP parsing fails (malformed request), no response is sent and the connection is not closed. The connection stays in ESTABLISHED state until 8-second timeout.

### Solution 2
Added connection close logic in `main.c` when `parse_http()` returns false.

### Modified Files
- `MCU2/Src/main.c`

### Code Change 2
```c
/* CASE DEFAULT: plain HTTP */
else
{
    if (parse_http(pS, ch395.buffer))
    {
        ch395.RX_received |= (1 << i);
        if (ch395.SOCK_responding < 0)
            ch395.SOCK_responding = i;
    }
    /* USER CODE BEGIN HTTP_PARSE_FAILED */
    else
    {
        // HTTP parsing failed, close the connection to prevent resource leak
        CH395TCPDisconnect(i);
        HTTPclose(i);
        resetHTTPParseState(pS);
    }
    /* USER CODE END HTTP_PARSE_FAILED */
}
```

### Expected Behavior
- Plain HTTP requests (index.html, CSS, JS, etc.): Connection properly closed after response sent
- WebSocket connections: Remain open for bidirectional communication
- TCP connections: Remain open for persistent communication
- Idle connections: Still subject to 8-second timeout in main.c loop
- Malformed HTTP requests: Connection closed immediately upon parse failure

---

## 2024-xx-xx - Optimize /socket Response Protocol

### Problem
The `/socket` response was causing buffer overflow. The original format was too verbose.

### Solution
Changed protocol to compact single-character format to reduce buffer usage.

### New Protocol Format
```
Old: sock0=HTTP:80&sock1=WS:80&sock2=DISCONNECTED&... (120-160 bytes)
New: s0=H&s1=W&s2=-&s7=U (about 35 bytes)
```

### Status Codes
- `H` = HTTP
- `W` = WebSocket
- `T` = TCP
- `U` = UDP
- `-` = DISCONNECTED
- `I` = IDLE

### Modified Files
- `MCU2/Src/HTTPResponders.c` - make_socket_status_str()
- `data_uncomp/index.html` - update_socket_from_response()