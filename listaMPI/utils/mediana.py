import re
import csv
import statistics
import sys

def parse_and_calculate_median(input_file, output_file):
    data = {}

    with open(input_file, 'r') as file:
        content = file.read()

    # Match blocks of "Testing with n=..."
    test_blocks = re.findall(r"Testing with n=(\d+), p=(\d+)\.\.\..*?(Elapsed time = [\d.e+-]+(?:\n|$)+)+", content, re.DOTALL)

    for block in test_blocks:
        n, p, times_block = block
        times = [float(m.group(1)) for m in re.finditer(r"Elapsed time = ([\d.e+-]+)", times_block)]
        median_time = statistics.median(times)
        data.setdefault(int(n), {})[int(p)] = median_time

    # Write results to a CSV
    with open(output_file, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["n", "p", "Median Elapsed Time (s)"])
        for n in sorted(data):
            for p in sorted(data[n]):
                writer.writerow([n, p, data[n][p]])


if len(sys.argv) != 3:
    print("Usage: python mediana.py <input_file> <output_file>")
    sys.exit(1)

input_file = sys.argv[1]

# Example usage
# Example usage
parse_and_calculate_median(input_file, f"{input_file}.csv")