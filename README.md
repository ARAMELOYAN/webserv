# webserv

**webserv** is a custom **HTTP/1.1 web server** written in **C++**, developed as part of the 42 curriculum.  
The project aims to reproduce the core behavior of a real web server (inspired by NGINX) while respecting strict performance and system-level constraints.

---

## 🧠 Project Overview

The goal of **webserv** is to build a fully functional web server from scratch, handling:

- TCP connections
- HTTP request parsing
- Response generation
- Configuration-based routing
- Multiple clients simultaneously (non-blocking I/O)

This project focuses heavily on **network programming**, **system calls**, and **robust C++ design**.

---

## ✨ Features

- ✅ HTTP/1.1 compliant
- 🔌 Non-blocking sockets
- 🔁 Multiplexing with `poll()` (or equivalent)
- 📁 Static file serving
- 📂 Directory listing (autoindex)
- 📤 File uploads (POST)
- ⚙️ Multiple server blocks
- 🧩 Config file parsing (NGINX-like syntax)
- ❌ Proper error handling & HTTP status codes

---

## 🛠️ Tech Stack

- **Language:** C++ (C++98 / C++11 depending on implementation)
- **Networking:** BSD sockets
- **I/O Multiplexing:** `poll()`
- **Build:** Makefile
- **OS:** Linux / macOS

---

## 📂 Project Structure

> (adjust if needed)

.
├── srcs/ # server implementation
├── includes/ # headers
├── config/ # example configuration files
├── www/ # static website root
├── Makefile
└── README.md


---

## ⚙️ Configuration File

The server is configured using a custom config file inspired by **NGINX**.

Example:

```conf
server {
    listen 8080;
    server_name localhost;

    root www;
    index index.html;

    location /upload {
        methods POST;
        upload_dir uploads;
    }
}


Supported directives may include:

listen

server_name

root

index

error_page

location

methods

autoindex

🚀 Build & Run
Build
make
Run
./webserv config/default.conf
If no config file is provided, a default configuration is used.

🌐 Usage
Once running, access the server via browser or curl:

curl http://localhost:8080
Test POST upload:

curl -X POST -F "file=@test.txt" http://localhost:8080/upload
🧪 Testing
The server can be tested using:

Browser (Chrome / Firefox)

curl

ab (ApacheBench)

siege

Focus areas:

Multiple simultaneous clients

Invalid HTTP requests

Large file uploads

Incorrect configurations

⚠️ Constraints (42 rules)
❌ No external libraries (except standard C/C++)

❌ No fork-based server model

❌ No blocking I/O

✅ One event loop

✅ Must never block on a single client

🧠 What This Project Teaches
Low-level networking

Event-driven architecture

HTTP protocol internals

Robust C++ design

Memory & resource management

🧩 Possible Improvements
CGI support

HTTPS (TLS)

Chunked transfer encoding

HTTP keep-alive optimization

Better config validation

📄 License
This project is for educational purposes as part of the 42 curriculum.

👤 Author
Aram Eloyan
🔗 GitHub: https://github.com/ARAMELOYAN
✉️  e-mail: arameloyan01@gmail.com

