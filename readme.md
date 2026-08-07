# 🚀 Redis-like TCP Server (C++)

A minimal Redis-inspired TCP server built from scratch using low-level Linux system programming.

This project is focused on understanding how real-world systems like Redis and Nginx work internally — especially around networking, concurrency, and OS-level behavior.

---

## 📌 Features (Current State)

- TCP server using `socket`, `bind`, `listen`, `accept`
- Non-blocking sockets (`fcntl`)
- Event-driven architecture using `epoll`
- Handles multiple concurrent clients
- Proper accept loop (`accept` until `EAGAIN`)
- Basic request handling using `recv`
- Graceful client disconnect handling
- Port reuse using `SO_REUSEADDR`

---

## 🧠 Concepts Covered

### 1. Non-blocking I/O
All sockets are configured as non-blocking:
```cpp
fcntl(fd, F_SETFL, O_NONBLOCK);