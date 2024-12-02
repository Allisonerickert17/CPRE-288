import socket
import matplotlib.pyplot as plt

HOST = "192.168.1.1"
PORT = 288

cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cybot_socket.connect((HOST, PORT))

print("Use 'w', 'a', 's', 'd' for movement, 't' to toggle mode, 'm' for scan, 'h' to proceed, 'quit' to exit.")

def plot_scan_data(angles, distances):
    plt.figure(figsize=(10, 6))
    plt.plot(angles, distances, marker='o')
    plt.title("CyBot Scan Data")
    plt.xlabel("Angle (degrees)")
    plt.ylabel("Distance (cm)")
    plt.grid(True)
    plt.show()

current_mode = "manual"

while True:
    command = input("Enter a command: ").strip()

    if command in ['w', 'a', 's', 'd']:
        if current_mode == "manual":
            cybot_socket.sendall(command.encode())
            response = cybot_socket.recv(1024).decode().strip()
            print(f"CyBot response: {response}")
        else:
            print("Movement commands are only allowed in manual mode.")

    elif command == 't':
        current_mode = "autonomous" if current_mode == "manual" else "manual"
        print(f"Switched to {current_mode} mode.")
        continue

    elif command in ['m', 'h']:
        if current_mode == "autonomous":
            cybot_socket.sendall(command.encode())
            response = cybot_socket.recv(2048).decode().strip()

            if response.startswith("SCAN:"):
                print("Received scan data.")
                response = response[5:]  # Remove the "SCAN:" prefix

                angles = []
                distances = []

                scan_data, obj_data = response.split("OBJECTS:")
                scan_entries = scan_data.split(';')

                # Parse scan data
                for entry in scan_entries:
                    if ',' in entry:
                        angle, distance = map(int, entry.split(','))
                        angles.append(angle)
                        distances.append(distance)

                # Plot scan data if valid
                if angles and distances:
                    plot_scan_data(angles, distances)
                else:
                    print("No valid scan data to plot.")

                # Parse detected object data
                objects = obj_data.split(';')
                if objects:
                    print("\nDetected Objects:")
                    print("Index\tStart Angle\tEnd Angle")
                    print("--------------------------------")
                    for obj in objects:
                        if ',' in obj:
                            index, start_angle, end_angle = obj.split(',')
                            print(f"{index}\t{start_angle}\t{end_angle}")
                else:
                    print("No objects detected.")

            else:
                print(f"CyBot response: {response}")

        else:
            print("Scan commands ('m' or 'h') are only allowed in autonomous mode.")

    elif command == 'quit':
        cybot_socket.sendall(command.encode())
        break

    else:
        print("Invalid command. Please use 'w', 'a', 's', 'd', 't', 'm', 'h', or 'quit'.")

cybot_socket.close()
