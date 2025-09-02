#!/usr/bin/env python3
"""
OSC to MIDI CC Bridge
Receives OSC messages from ESP32 and converts them to MIDI CC messages
"""

import argparse

import mido
from mido import Message
from pythonosc import dispatcher, osc_server


class OSCtoMIDI:
    def __init__(self, midi_port_name=None):
        # Open MIDI output port
        available_ports = mido.get_output_names()
        print(f"Available MIDI ports: {available_ports}")

        if midi_port_name and midi_port_name in available_ports:
            self.midi_out = mido.open_output(midi_port_name)
            print(f"Using MIDI port: {midi_port_name}")
        elif available_ports:
            self.midi_out = mido.open_output(available_ports[0])
            print(f"Using default MIDI port: {available_ports[0]}")
        else:
            # Create virtual port if no ports available
            self.midi_out = mido.open_output("OSC2MIDI", virtual=True)
            print("Created virtual MIDI port: OSC2MIDI")

        # Button to CC mapping (customize as needed)
        self.button_to_cc = {
            1: 10,  # Button 1 -> CC 10
            2: 11,  # Button 2 -> CC 11
            3: 12,  # Button 3 -> CC 12
            4: 13,  # Button 4 -> CC 13
            5: 14,  # Button 5 -> CC 14
            6: 15,  # Button 6 -> CC 15
            7: 16,  # Button 7 -> CC 16
            8: 17,  # Button 8 -> CC 17
            9: 18,  # Button 9 -> CC 18
            10: 19,  # Button 10 -> CC 19
            11: 20,  # Button 11 -> CC 20
            12: 21,  # Button 12 -> CC 21
            13: 22,  # Button 13 -> CC 22
            14: 23,  # Button 14 -> CC 23
            15: 24,  # Button 15 -> CC 24
            16: 25,  # Button 16 -> CC 25
            17: 26,  # Button 17 -> CC 26
            18: 27,  # Button 18 -> CC 27
            19: 28,  # Button 19 -> CC 28
        }

        self.midi_channel = 0  # MIDI channel (0-15, displayed as 1-16)

    def button_handler(self, address, *args):
        """Handle /button OSC messages"""
        if len(args) >= 2:
            button_num = args[0]
            state = args[1]

            # Get CC number for this button
            cc_num = self.button_to_cc.get(button_num, button_num)

            # Convert state to MIDI value (0 or 127)
            midi_value = 127 if state == 1 else 0

            # Send MIDI CC
            msg = Message(
                "control_change",
                channel=self.midi_channel,
                control=cc_num,
                value=midi_value,
            )
            self.midi_out.send(msg)

            print(
                f"Button {button_num} {'pressed' if state else 'released'} "
                f"-> CC {cc_num} = {midi_value}"
            )

    def close(self):
        """Clean up MIDI port"""
        if self.midi_out:
            self.midi_out.close()


def main():
    parser = argparse.ArgumentParser(description="OSC to MIDI CC Bridge")
    parser.add_argument(
        "--ip", default="0.0.0.0", help="IP address to listen on (default: 0.0.0.0)"
    )
    parser.add_argument(
        "--port", type=int, default=8000, help="OSC port to listen on (default: 8000)"
    )
    parser.add_argument(
        "--midi", type=str, default=None, help="MIDI port name (default: auto-select)"
    )

    args = parser.parse_args()

    # Create OSC to MIDI bridge
    osc2midi = OSCtoMIDI(args.midi)

    # Setup OSC dispatcher
    disp = dispatcher.Dispatcher()
    disp.map("/button", osc2midi.button_handler)

    # Create and start OSC server
    server = osc_server.ThreadingOSCUDPServer((args.ip, args.port), disp)
    print(f"OSC Server listening on {args.ip}:{args.port}")
    print("Press Ctrl+C to stop...")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down...")
        osc2midi.close()


if __name__ == "__main__":
    main()
