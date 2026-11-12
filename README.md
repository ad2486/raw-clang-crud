# raw-crud

CRUD HTTP server in C built from scratch using POSIX sockets, `pthread`, and SQLite.

## Build

```sh
make              # Build with ASan + UBSan (./server)
make server_tsan  # Build with ThreadSanitizer (./server_tsan)
make clean        # Remove binaries
```

## Run

```sh
./server
```

Listens on port `8080`.

## Current Endpoints (Phase 2)

- `GET /` — returns `200 OK` (`Hello World!`)
- `POST /echo` — returns `200 OK` echoing the request body
- Any unknown route — returns `404 Not Found`
- Malformed HTTP request — returns `400 Bad Request`

## Progress

- [x] Phase 1: Basic TCP server (`socket`, `bind`, `listen`, `accept`, `SO_REUSEADDR`, `SIGPIPE`)
- [x] Phase 2: Manual HTTP/1.1 parser (`read_http_request`, `Content-Length`, `build_http_response`, `send_http_response`)
- [ ] Phase 3: Concurrency with `pthread`
- [ ] Phase 4: SQLite persistence
- [ ] Phase 5: CRUD routing and JSON
- [ ] Phase 6: Robustness and cleanup
