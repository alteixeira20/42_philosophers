#!/usr/bin/env python3
import subprocess
import sys
import re
import argparse
import os
from threading import Timer

# ANSI colors
CYAN = "\033[96m"
YELLOW = "\033[93m"
MAGENTA = "\033[95m"
GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"

# Define all test cases
TESTS = [
    ("1 philosopher should die", ["1", "800", "200", "200"], "die"),
    ("4 philosophers, one should die", ["4", "310", "200", "100"], "die"),
    ("4 philosophers, one should die (200/205/200)", ["4", "200", "205", "200"], "die"),
    ("5 philosophers, 7 meals", ["5", "800", "200", "200", "7"], "stop"),
    ("4 philosophers, 10 meals", ["4", "410", "200", "200", "10"], "stop"),
    ("5 philosophers no death", ["5", "800", "200", "200"], "no_die"),
    ("5 philosophers no death (600/150/150)", ["5", "600", "150", "150"], "no_die"),
    ("4 philosophers no death", ["4", "410", "200", "200"], "no_die"),
    ("100 philosophers no death", ["100", "800", "200", "200"], "no_die"),
    ("105 philosophers no death", ["105", "800", "200", "200"], "no_die"),
    ("200 philosophers no death", ["200", "800", "200", "200"], "no_die"),
    ("invalid negative count", ["-5", "600", "200", "200"], "error"),
    ("invalid negative time_to_die", ["4", "-5", "200", "200"], "error"),
    ("invalid negative time_to_eat", ["4", "600", "-5", "200"], "error"),
    ("invalid negative time_to_sleep", ["4", "600", "200", "-5"], "error"),
    ("invalid negative must_eat", ["4", "600", "200", "200", "-5"], "error"),
]

def select_binary():
    has_mandatory = os.path.exists("../philo/philo")
    has_bonus = os.path.exists("../philo_bonus/philo_bonus")

    if has_mandatory and has_bonus:
        print(f"{CYAN}Both mandatory and bonus binaries detected.{RESET}")
        choice = input(f"{YELLOW}Which version do you want to test? (m = mandatory, b = bonus): {RESET}").strip().lower()
        while choice not in ["m", "b"]:
            choice = input(f"{RED}Invalid input. Please enter 'm' or 'b': {RESET}").strip().lower()
        return "../philo/philo" if choice == "m" else "../philo_bonus/philo_bonus"
    elif has_bonus:
        print(f"{CYAN}Only bonus binary found. Testing bonus version...{RESET}")
        return "../philo_bonus/philo_bonus"
    elif has_mandatory:
        print(f"{CYAN}Only mandatory binary found. Testing mandatory version...{RESET}")
        return "../philo/philo"
    else:
        print(f"{RED}No philo or philo_bonus binary found. Please compile first.{RESET}")
        sys.exit(1)

# Default binary path, overridden by --bin if provided
default_bin = select_binary()

parser = argparse.ArgumentParser(description="42 Philosophers Tester (automatic)")
parser.add_argument("--bin", default=default_bin, help="Path to philo or philo_bonus binary (default: auto-select)")
parser.add_argument("--repeat", type=int, default=3, help="Repetitions per test (default: 3)")
parser.add_argument("--timeout", type=int, default=10, help="Timeout in seconds per test (default: 10)")
args = parser.parse_args()

def run_test(test_args, expected, timeout, binary):
    cmd = [binary] + test_args
    try:
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        timer = Timer(timeout, proc.kill)
        timer.start()
        out, err = proc.communicate()
        timer.cancel()
    except Exception as e:
        return False, f"Error running {cmd}: {e}"

    combined_output = out + err
    died_lines = [line for line in combined_output.splitlines() if "died" in line]

    try:
        children = subprocess.check_output(["pgrep", "-P", str(proc.pid)])
        if children.strip():
            return False, "Orphan processes detected after termination:\n" + children.decode()
    except subprocess.CalledProcessError:
        pass

    if "Error" in combined_output or "error" in combined_output:
        if expected == "error":
            return True, combined_output
        else:
            return False, combined_output

    if expected == "error":
        return False, combined_output

    eaten_counts = {}
    for line in combined_output.splitlines():
        m = re.match(r"\s*\d+\s+(\d+)\s+is eating", line)
        if m:
            pid = int(m.group(1))
            eaten_counts[pid] = eaten_counts.get(pid, 0) + 1

    if expected == "die":
        if not died_lines:
            return False, "Expected a philosopher to die, but none did.\n\n" + combined_output
    if expected == "no_die":
        if died_lines:
            return False, "Expected no philosophers to die, but some did.\n\n" + combined_output
    if expected == "stop":
        if died_lines:
            return False, "A philosopher died but should not have.\n\n" + combined_output
        if len(test_args) == 5:
            try:
                must_eat = int(test_args[4])
                for philo in range(1, int(test_args[0]) + 1):
                    count = eaten_counts.get(philo, 0)
                    if count < must_eat:
                        return False, f"Philosopher {philo} only ate {count} times (expected at least {must_eat})."
            except ValueError:
                pass
        return True, combined_output

    return True, combined_output

def extract_context(output, keyword):
    lines = output.strip().splitlines()
    matches = [i for i, line in enumerate(lines) if keyword in line]
    if matches:
        i = matches[0]
        start = max(i - 4, 0)
        end = i + 1
        return "\n".join(lines[start:end])
    if lines:
        return "\n".join(lines[:10])
    return "(No output)"

def main():
    all_results = []

    for description, test_args, expected in TESTS:
        print(f"\n{CYAN}======================================")
        print(f"Running Test: {description}{RESET}")
        print(f"{YELLOW}Command:{RESET} {args.bin} {' '.join(test_args)}")
        print(f"{MAGENTA}Expected behavior:{RESET} {expected}")

        for i in range(args.repeat):
            ok, output = run_test(test_args, expected, args.timeout, args.bin)
            result = "PASS" if ok else "FAIL"
            all_results.append((result, description, test_args, expected, output.strip()))
            color = GREEN if ok else RED
            print(f"  [{i+1}/{args.repeat}] {color}{result}{RESET}")

    passed = [r for r in all_results if r[0] == "PASS"]
    failed = [r for r in all_results if r[0] == "FAIL"]

    print(f"\n{CYAN}=== TEST RESULTS SUMMARY ==={RESET}\n")

    print(f"{GREEN}--- PASSED TESTS ---{RESET}")
    for result, desc, *_ in passed:
        print(f"{GREEN}PASS{RESET} - {desc}")

    if failed:
        print(f"\n{RED}--- FAILED TESTS ---{RESET}")
        for result, desc, test_args, expected, out in failed:
            print(f"\n{RED}FAIL{RESET} - {desc}")
            print(f"{MAGENTA}Expected:{RESET} {expected}")
            print(f"{YELLOW}Command:{RESET} {args.bin} {' '.join(test_args)}")
            print(f"{RED}Relevant Output:{RESET}")
            if "Expected a philosopher to die" in out:
                print(extract_context(out, "died"))
            elif "Expected no philosophers to die" in out:
                print(extract_context(out, "died"))
            elif "Expected simulation to stop" in out:
                print(extract_context(out, "eaten"))
            else:
                print(extract_context(out, ""))
            print()

if __name__ == "__main__":
    main()
