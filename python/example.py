#pip import pyserial
import argparse
import enum
import serial
import time
import sys

# --- Constants ---
DEFAULT_PORT = "COM4"
DEFAULT_BAUD_RATE = 115200
COMMAND_PREFIX = 'A'

class Animation(enum.IntEnum):
    """Maps animation names to their corresponding integer IDs for the Arduino."""
    WAKEUP = 0
    RESET = 1
    MOVE_RIGHT_BIG = 2
    MOVE_LEFT_BIG = 3
    BLINK_LONG = 4
    BLINK_SHORT = 5
    HAPPY = 6
    SLEEP = 7
    SACCADE_RANDOM = 8

def send_animation_command(display: serial.Serial, anim: Animation):
    """
    Sends an animation command to the Arduino display.

    The command is a string starting with COMMAND_PREFIX followed by the animation index.
    """
    cmd = f'{COMMAND_PREFIX}{int(anim.value)}'
    cmd_byte = cmd.encode('utf-8')
    print(f"Sending command for {anim.name}: {cmd_byte}")
    display.write(cmd_byte)
    
    # Wait for and read the confirmation from the Arduino to clear the buffer
    # and ensure the command was processed.
    try:
        response = display.readline().decode('utf-8').strip()
        print(f"Received from Arduino: '{response}'")
    except UnicodeDecodeError:
        print("Received non-UTF8 response from Arduino.", file=sys.stderr)



def main():
    """
    Main function to connect to the Arduino and cycle through eye animations.
    """
    parser = argparse.ArgumentParser(description="Controls an Arduino-powered OLED eye display.")
    parser.add_argument(
        "--port",
        default=DEFAULT_PORT,
        help=f"The serial port for the Arduino. Defaults to {DEFAULT_PORT}.",
    )
    args = parser.parse_args()

    try:
        print(f"Attempting to connect to Arduino on {args.port} at {DEFAULT_BAUD_RATE} baud...")
        # Use a longer timeout for the initial handshake
        arduino = serial.Serial(args.port, DEFAULT_BAUD_RATE, timeout=10) # Increased to 10s
        print("Waiting for Arduino to become ready...")
        
        # Wait for the "READY" signal from the Arduino
        ready_signal = arduino.readline().decode('utf-8').strip()
        if ready_signal == "READY":
            print("Arduino is ready. Starting animation sequence.")
        else:
            print(f"Error: Did not receive READY signal. Got: '{ready_signal}'", file=sys.stderr)
            sys.exit(1)

    except serial.SerialException as e:
        print(f"Error: Could not open serial port '{args.port}'.", file=sys.stderr)
        print(f"Please check the port and ensure the Arduino is connected. Details: {e}", file=sys.stderr)
        sys.exit(1)

    # Cycle through all available animations, resetting after each one.
    for anim in Animation:
        # We don't need to run RESET as a standalone animation in the loop
        # since we use it for cleanup after every other animation.
        if anim == Animation.RESET:
            continue

        send_animation_command(arduino, anim)

        print("Sending RESET command...")
        send_animation_command(arduino, Animation.RESET)
        time.sleep(0.5) # Brief pause after reset for visual clarity

    print("Animation cycle complete.")
    arduino.close()


if __name__ == "__main__":
    main()
