#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# This file is part of the card10 project card10.badge.events.ccc.de
#
# The MIT License (MIT)
#
# Copyright (c) 2019 Alexander Böhm
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

"""
card10 interface

This module provides is an extenstion of the Pyboard class, used to
communicate with the card10 badge over the serial connection via USB.

For description of the card10 board look at the project website <https://card10.badge.events.ccc.de>.

Python2 isn't supported.

Example usage:

    import pycard10
    card10 = pycard10.PyCard10('/dev/ttyACM0')

Then:

    card10.enter_raw_repl()
    card10.exec('import leds')
    card10.exec('leds.set_rocket(0, 31)')
    card10.exit_raw_repl()

    import pycard10
    pycard10.execfile('test.py', device='/dev/ttyACM0')

This script can also be run directly.  To execute a local script, use:

    tools/pycard10.py test.py

Or:
    python3 tools/pycard10.py test.py

"""

__author__ = "Alexander Böhm"
__copyright__ = "Copyright 2019, Alexander Böhm"
__license__ = "MIT"
__email__ = "alexander.boehm@malbolge.net"

import sys
import time
import os
from pyboard import (
    stdout_write_bytes,
    PyboardError,
    Pyboard,
    ProcessPtyToTerminal,
    ProcessToSerial,
    TelnetToSerial,
)


class PyCard10(Pyboard):
    """
    Python card10 connector.
    """

    def __init__(self, device, wait=0):
        """
        Open a connection to the card10 over the serial device *device*.
        """
        Pyboard.__init__(
            self, device=device, baudrate=115200, user=None, password=None, wait=wait
        )

    def exec_raw_no_follow(self, command):
        """
        Execute a the command 'command' on the card10.

        Parameters:
        command (bytes): Command or multiple commands

        Returns:
        None
        """

        if isinstance(command, bytes):
            command_bytes = command
        else:
            command_bytes = bytes(command, encoding="utf8")

        data = self.read_until(1, b">")
        if not data.endswith(b">"):
            raise PyboardError("card10 not in raw repl mode: (response: %r)" % (data))

        # write command
        for i in range(0, len(command_bytes), 256):
            self.serial.write(command_bytes[i : min(i + 256, len(command_bytes))])
            time.sleep(0.01)

        self.serial.write(b"\x04")

        # check if we could exec command
        data = self.serial.read(2)
        if data != b"OK":
            raise PyboardError("could not exec command (response: %r)" % data)

    def enter_raw_repl(self):
        """
        Enter the RAW repl mode. After the prompt character ('>') left in the buffer of the serial line.

        Returns:
        None
        """
        self.serial.write(b"\x03\x03")  # ctrl-C twice: interrupt any running program

        # flush input (without relying on serial.flushInput())
        n = self.serial.inWaiting()
        while n > 0:
            self.serial.read(n)
            n = self.serial.inWaiting()

        self.serial.write(b"\x01")  # ctrl-A: enter raw REPL
        data = self.read_until(1, b"raw REPL; CTRL-B to exit\r\n>")
        if not data.endswith(b"raw REPL; CTRL-B to exit\r\n>"):
            raise PyboardError("could not enter raw repl")

        self.serial.write(b"\r\x04")  # execute nothing
        data = self.serial.read(2)
        if data != b"OK":
            raise PyboardError("could not enter raw repl")

    def exec(self, command):
        """
        Execute a command on the card10 and return the output of the card10.

        Parameters:
        command (bytes): Command or multiple commands

        Returns:
        data(bytes), data_err(bytes): data is standard out, data_err is standard error
        """
        return self.exec_(command)

    def soft_reset(self):
        """
        Doing a soft reset on the board and going to menu.

        Returns:
        None
        """

        self.serial.write(b"\x03\x03")  # ctrl-C twice: interrupt any running program

        self.serial.write(b"\x01")  # ctrl-B: ensue it's the normal mode

        self.serial.write(b"\x04")  # ctrl-D: do the reset

        n = self.serial.inWaiting()
        while n > 0:
            self.serial.read(n)
            n = self.serial.inWaiting()


def execfile(filename, device="/dev/ttyACM0"):
    """
    Execute python source from *filename* via the RAW repl mode on the card10 connected via serial line *device*.

    Parameters:
    filename(str): Path text file with commands
    device(str): Path to the card10 device.

    Returns:
    None
    """
    c = PyCard10(device)
    c.enter_raw_repl()
    output = c.execfile(filename)
    stdout_write_bytes(output)
    c.exit_raw_repl()
    c.close()


def main():
    """
    The main method.

    Returns:
    None
    """

    import argparse

    cmd_parser = argparse.ArgumentParser(description="Run scripts on the card10.")
    cmd_parser.add_argument(
        "--device", default="/dev/ttyACM0", help="the serial device of the card10"
    )
    cmd_parser.add_argument("-c", "--command", help="program passed in as string")
    cmd_parser.add_argument(
        "--set-time",
        action="store_true",
        help="Set card10 system time to this host's time",
    )
    cmd_parser.add_argument(
        "-w",
        "--wait",
        default=0,
        type=int,
        help="seconds to wait for USB connected board to become available",
    )
    cmd_parser.add_argument(
        "--follow",
        action="store_true",
        help="follow the output after running the scripts [default if no scripts given]",
    )
    cmd_parser.add_argument(
        "--reset", action="store_true", help="Soft reseting the card10"
    )
    cmd_parser.add_argument("files", nargs="*", help="input files")
    args = cmd_parser.parse_args()

    # open the connection to the card10
    try:
        card10 = PyCard10(args.device, args.wait)
    except PyboardError as er:
        print(er)
        sys.exit(1)

    if args.reset:
        card10.soft_reset()

    elif args.set_time or args.command is not None or len(args.files):
        # we must enter raw-REPL mode to execute commands
        # this will do a soft-reset of the board
        try:
            card10.enter_raw_repl()
        except PyboardError as er:
            print(er)
            card10.close()
            sys.exit(1)

        def execbuffer(buf):
            try:
                ret, ret_err = card10.exec_raw(
                    buf, timeout=None, data_consumer=stdout_write_bytes
                )
            except PyboardError as er:
                print(er)
                card10.close()
                sys.exit(1)
            except KeyboardInterrupt:
                sys.exit(1)
            if ret_err:
                card10.exit_raw_repl()
                card10.close()
                stdout_write_bytes(ret_err)
                sys.exit(1)

        # Set card10 system time
        if args.set_time:
            now = round(time.time())
            code = """\
import utime
utime.set_unix_time({time})
print("Time was set to {time}!")
"""
            execbuffer(code.format(time=now))

        # run the command, if given
        if args.command is not None:
            execbuffer(args.command.encode("utf-8"))

        # run any files
        for filename in args.files:
            with open(filename, "rb") as f:
                pyfile = f.read()
                execbuffer(pyfile)

        # exiting raw-REPL just drops to friendly-REPL mode
        card10.exit_raw_repl()

    # if asked explicitly, or no files given, then follow the output
    elif args.follow or (args.command is None and len(args.files) == 0):
        try:
            ret, ret_err = card10.follow(timeout=None, data_consumer=stdout_write_bytes)
        except PyboardError as er:
            print(er)
            sys.exit(1)
        except KeyboardInterrupt:
            sys.exit(1)
        if ret_err:
            card10.close()
            stdout_write_bytes(ret_err)
            sys.exit(1)

    # close the connection to the card10
    card10.close()


if __name__ == "__main__":
    main()
