#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import argparse
import textwrap
import hextool


def round_up_to_block_size(length, block_size):
    return (length + block_size - 1) // block_size * block_size


def bytes_amount(string_amount):
    """Parse a byte amount string, possibly including a suffix"""

    suffixes = [
        ("k", 1024),
        ("kb", 1024),
        ("kib", 1024),
        ("m", 1024 * 1024),
        ("mb", 1024 * 1024),
        ("mib", 1024 * 1024),
    ]

    value = string_amount.lower()

    # Match suffix
    for s in suffixes:
        if value.endswith(s[0]):
            value = value[: -len(s[0])]
            break
    else:
        # No suffix found
        s = ("", 1)

    try:
        # Convert to int and apply suffix weight
        return int(value, 0) * s[1]
    except ValueError:
        # Invalid input string
        raise argparse.ArgumentTypeError("invalid value %s" % string_amount)


def int_base_0(string_value):
    """Parser for integers in decimal or hexadecimal"""

    try:
        return int(string_value, 0)
    except ValueError:
        # Invalid input string
        raise argparse.ArgumentTypeError("invalid value %s" % string_value)


def create_argument_parser(pgmname):
    """Create a parser for parsing the command line"""

    # Determine help text width
    try:
        help_width = int(os.environ["COLUMNS"])
    except (KeyError, ValueError):
        help_width = 80
    help_width -= 2

    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=textwrap.fill(
            "A tool to embed a bootloader updater in a Wirepas Mesh stack firmware",
            help_width,
        ),
    )

    parser.add_argument(
        "output_hex",
        metavar="OUTPUT",
        help="output Intel HEX file",
    )

    parser.add_argument(
        "--stack_hex",
        "-s",
        metavar="FILE",
        required=True,
        help="input Intel HEX file for the Wirepas Mesh stack",
    )

    parser.add_argument(
        "--bl_updater_hex",
        "-u",
        metavar="FILE",
        required=True,
        help="input Intel HEX file for the bootloader updater",
    )

    parser.add_argument(
        "--bootloader_hex",
        "-b",
        metavar="FILE",
        required=True,
        help="input Intel HEX file for the new bootloader",
    )

    parser.add_argument(
        "--stack_area_addr",
        "-a",
        type=int_base_0,
        metavar="ADDR",
        required=True,
        help="start address of stack area",
    )

    parser.add_argument(
        "--stack_area_length",
        "-l",
        type=int_base_0,
        metavar="LENGTH",
        required=True,
        help="length of stack area",
    )

    parser.add_argument(
        "--erase_block_size",
        "-e",
        type=bytes_amount,
        metavar="SIZE",
        required=True,
        help="erase block size of internal Flash memory",
    )

    return parser


def main():
    """Main program"""

    # Determine program name, for error messages
    pgmname = os.path.split(sys.argv[0])[-1]

    try:
        # Parse command line arguments
        args = create_argument_parser(pgmname).parse_args()

        # Read stack hex file and convert to bytearray()
        stack_data = hextool.Memory(gap_fill_byte=0xFF)
        hextool.load_intel_hex(stack_data, filename=args.stack_hex)
        stack_data = stack_data[stack_data.min_address : stack_data.max_address]

        # Read bootloader updater hex file and convert to bytearray()
        bl_updater_data = hextool.Memory(gap_fill_byte=0xFF)
        hextool.load_intel_hex(bl_updater_data, filename=args.bl_updater_hex)
        bl_updater_data = bl_updater_data[
            bl_updater_data.min_address : bl_updater_data.max_address
        ]

        # Read new bootloader hex file and convert to bytearray()
        bootloader_data = hextool.Memory(gap_fill_byte=0xFF)
        hextool.load_intel_hex(bootloader_data, filename=args.bootloader_hex)
        bootloader_data = bootloader_data[
            bootloader_data.min_address : bootloader_data.max_address
        ]

        # Pad data sizes to erase block size
        stack_size = round_up_to_block_size(len(stack_data), args.erase_block_size)
        bl_updater_size = round_up_to_block_size(
            len(bl_updater_data), args.erase_block_size
        )
        bootloader_size = round_up_to_block_size(
            len(bootloader_data), args.erase_block_size
        )

        if stack_size + bl_updater_size + bootloader_size > args.stack_area_length:
            raise ValueError("no space for bootloader updater in stack area")

        output = hextool.Memory(gap_fill_byte=0xFF)

        # Add bootloader updater
        output.cursor = args.stack_area_addr
        output += bl_updater_data

        # Add the end part of stack
        output.cursor = args.stack_area_addr + bl_updater_size
        output += stack_data[bl_updater_size:]

        # Add the part of the stack that is replaced by the bootloader updater
        # to the end of the stack area, before the new bootloader
        output.cursor = (
            args.stack_area_addr
            + args.stack_area_length
            - bootloader_size
            - bl_updater_size
        )
        output += stack_data[:bl_updater_size]

        # Add the new bootloader at the end of the stack area
        output.cursor = args.stack_area_addr + args.stack_area_length - bootloader_size
        output += bootloader_data

        # Write output file
        hextool.save_intel_hex(output, filename=args.output_hex)
    except (ValueError, IOError, OSError) as exc:
        sys.stdout.write("%s: %s\n" % (pgmname, exc))
        return 1


# Run main
if __name__ == "__main__":
    sys.exit(main())
