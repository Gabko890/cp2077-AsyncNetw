import socket

HOST = '0.0.0.0'
PORT = 12345

def run_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind((HOST, PORT))
        server.listen()
        print(f"Server listening on {HOST}:{PORT}")

        try:
            while True:
                conn, addr = server.accept()
                print('Connected by', addr)
                with conn:
                    while True:
                        data = conn.recv(1024)
                        if not data:
                            break
                        message = data.decode('utf-8').strip()
                        print("Received:", message)
        except KeyboardInterrupt:
            print("\nServer shutting down.")

if __name__ == "__main__":
    run_server()