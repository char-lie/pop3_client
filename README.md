# POP3 Client (Ubuntu Linux)

## Requirements

- Boost library: `apt-get install libboost-all-dev`
- Open SSL: `apt-get install libssl-dev`

## Build

Just use `make all`.

## Usage

```
Usage: pop3_client [options]
Allowed options:
  -h [ --help ]            display this help message
  -l [ --login ] arg       username
  -p [ --password ] arg    password (optional)
  -s [ --server_name ] arg host:port
```
