import pygame
import serial

ser = serial.Serial("COM19", 9600)

def main():
    pygame.init()

    if pygame.joystick.get_count() == 0:
        print("Joystick device not found.")
        return

    joystick = pygame.joystick.Joystick(1)
    joystick.init()
    print("Joystick device found:", joystick.get_name())
    
    running = True
    st = ""
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.JOYBUTTONDOWN:
                st = f"{event.button}"
                send_serial_data(st)
                print("Button:", event.button, "pressed.")
            elif event.type == pygame.JOYBUTTONUP:
                print("Button:", event.button, "released.")
            elif event.type == pygame.JOYAXISMOTION:
                handle_axis_motion(event)

    pygame.quit()
    ser.close()

def send_serial_data(data):
    bytes_to_send = data.encode('utf-8')
    ser.write(bytes_to_send)

def handle_axis_motion(event):
    print("Axis:", event.axis, "Value:", event.value)
    if event.axis == 1 and int(event.value) == -1:
        send_serial_data("1")
    elif event.axis == 1 and int(event.value) == 1:
        send_serial_data("2")
    else:
        send_serial_data("0")

if __name__ == "__main__":
    main()
