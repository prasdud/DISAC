import csv
import random
import os

# Data extracted from the table (subject codes, titles, marks, results)
data = [
    ['BCS401', 'ANALYSIS AND DESIGN OF ALGORITHMS', 50, 41, 91, 'P'],
    ['BCS402', 'MICROCONTROLLERS', 50, 18, 68, 'P'],
    ['BCS403', 'DATABASE MANAGEMENT SYSTEMS', 49, 36, 85, 'P'],
    ['BCSL404', 'ANALYSIS AND DESIGN OF ALGORITHMS LAB', 50, 50, 100, 'P'],
    ['BBOC407', 'BIOLOGY FOR COMPUTER ENGINEERS', 44, 30, 74, 'P'],
    ['BUHK408', 'UNIVERSAL HUMAN VALUES COURSE', 50, 42, 92, 'P'],
    ['BPEK459', 'PHYSICAL EDUCATION', 94, 0, 94, 'P'],
    ['BCS405B', 'GRAPH THEORY', 44, 31, 75, 'P'],
    ['BCSL456D', 'TECHNICAL WRITING USING LATEX LAB', 50, 50, 100, 'P']
]

# Column headers for the CSV file
headers = ['Subject Code', 'Subject Title', 'Internal Marks', 'External Marks', 'Total Marks', 'Subject Result']


# Function to randomize numeric and alphanumeric parts separately
def randomize_and_save_data(data, num_files=20):
    if not os.path.exists("randomized_csv"):
        os.makedirs("randomized_csv")

    # Extract the individual components: subject codes, titles, numeric marks, results
    subject_codes = [row[0] for row in data]
    titles = [row[1] for row in data]
    internal_marks = [row[2] for row in data]
    external_marks = [row[3] for row in data]
    total_marks = [row[4] for row in data]
    results = [row[5] for row in data]

    for i in range(1, num_files + 1):
        # Shuffle each component independently
        random.shuffle(subject_codes)
        random.shuffle(titles)
        random.shuffle(internal_marks)
        random.shuffle(external_marks)
        random.shuffle(total_marks)
        random.shuffle(results)

        # Rebuild the randomized data row by row
        randomized_data = []
        for j in range(len(data)):
            randomized_row = [
                subject_codes[j],
                titles[j],
                internal_marks[j],
                external_marks[j],
                total_marks[j],
                results[j]
            ]
            randomized_data.append(randomized_row)

        # Save to CSV
        filename = f"randomized_data_{i}.csv"
        filepath = os.path.join("randomized_csv", filename)

        with open(filepath, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(headers)
            writer.writerows(randomized_data)
        print(f"Saved: {filename}")


# Randomize data and save 20 times
randomize_and_save_data(data, 20)
