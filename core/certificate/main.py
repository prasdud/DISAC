import csv
import os
import subprocess

# Path to the CSV file
csv_file = 'sample.csv'

# Template for the LaTeX document
latex_template = r'''
\documentclass[a4paper,12pt]{article}
\usepackage{graphicx}
\usepackage{geometry}
\geometry{a4paper, margin=10pt}
\usepackage{array}

\begin{document}

% Header Section with VTU information
\begin{center}
    \textbf{VTU PROVISIONAL RESULTS OF UG / PG June/July-2024 EXAMINATION}
\end{center}

\vspace{0.5cm}

% Student Information Section
\noindent
\textbf{USN:} {usn} \\
\textbf{Student Name:} {name}

\vspace{0.5cm}

\noindent
\textbf{Semester: 4}

\vspace{0.5cm}

% Centered Table
\begin{center}
\begin{tabular}{|m{2cm}|m{7cm}|m{2cm}|m{2cm}|m{2cm}|m{2cm}|}
    \hline
    \textbf{Subject Code} & \textbf{Subject Title} & \textbf{Internal Marks} & \textbf{External Marks} & \textbf{Total Marks} & \textbf{Subject Result} \\
    \hline
    {table_content}
\end{tabular}
\end{center}

\vspace{1cm}

\vfill

% Registrar signature
\begin{flushright}
    Sd/- \\
    \textbf{Principle}
\end{flushright}

\vspace{0.5cm}

% Footer with result sheet ID and date
\noindent

\end{document}
'''

# Function to generate the LaTeX content
def generate_latex(usn, name, subjects):
    table_content = ''
    for subject in subjects:
        table_content += f"{subject['SubjectCode']} & {subject['SubjectTitle']} & {subject['InternalMarks']} & {subject['ExternalMarks']} & {subject['TotalMarks']} & {subject['Result']} \\\\ \\hline\n"
    
    latex_content = latex_template.format(usn=usn, name=name, table_content=table_content)
    
    # Save the LaTeX content to a file
    filename = f"{usn}_certificate.tex"
    with open(filename, 'w') as f:
        f.write(latex_content)
    
    return filename

# Function to compile LaTeX to PDF
def compile_latex(latex_file):
    subprocess.run(['pdflatex', latex_file])

# Read data from CSV
students_data = {}
with open(csv_file, mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        usn = row['USN']
        name = row['Name']
        subject_info = {
            'SubjectCode': row['SubjectCode'],
            'SubjectTitle': row['SubjectTitle'],
            'InternalMarks': row['InternalMarks'],
            'ExternalMarks': row['ExternalMarks'],
            'TotalMarks': row['TotalMarks'],
            'Result': row['Result']
        }
        
        if usn not in students_data:
            students_data[usn] = {'name': name, 'subjects': []}
        
        students_data[usn]['subjects'].append(subject_info)

# Generate LaTeX and compile for each student
for usn, student_info in students_data.items():
    latex_file = generate_latex(usn, student_info['name'], student_info['subjects'])
    compile_latex(latex_file)
    os.remove(latex_file)  # Clean up the .tex file after compiling
    os.remove(latex_file.replace('.tex', '.log'))  # Clean up auxiliary files
    os.remove(latex_file.replace('.tex', '.aux'))
