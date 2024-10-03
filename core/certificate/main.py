import pandas as pd
import subprocess
import os

# Function to generate LaTeX document from CSV
def generate_latex_from_csv(csv_file, output_file):
    # Read the CSV file
    df = pd.read_csv(csv_file)

    # Start writing the LaTeX document
    latex_content = r"""\documentclass[a4paper,12pt]{article}
\usepackage{graphicx}
\usepackage{geometry}
\geometry{a4paper, margin=10pt}
\usepackage{array}

\begin{document}

% Header Section with VTU information and logo
\begin{minipage}{0.2\textwidth}
    \includegraphics[width=\textwidth]{vtulogo.png} % Replace 'vtulogo.jpg' with the path to your image file
\end{minipage}
\begin{minipage}{0.5\textwidth}
    \centering
    \textbf{\Large Visvesvaraya Technological University} \\
    \vspace{0.2cm}
    \textbf{\textsc{\large Vemana Institute of Technology}} \\
    \vspace{0.2cm}
    % "Jnana Sangama" Belagavi-590018, Karnataka, India
\end{minipage}
\begin{minipage}{0.2\textwidth}
    \includegraphics[width=\textwidth]{vitlogo.jpg} % Replace 'vitlogo.jpg' with the path to your image file
\end{minipage}

% Header Section with VTU information
\begin{center}
    \textbf{VTU PROVISIONAL RESULTS OF UG / PG June/July-2024 EXAMINATION}
\end{center}

\vspace{0.5cm}

% Student Information Section
\noindent
\begin{minipage}{0.7\textwidth}
\textbf{USN:} 1VI22CS106 \\\\
\textbf{Student Name:} SANTHOSH KUMAR S\\\\
\textbf{Semester: } 4\\\\
\textbf{Month / Year: } August 2004
\end{minipage}
\begin{minipage}{0.2\textwidth}
    \includegraphics[width=\textwidth]{vtulogo.png} % Replace 'vtulogo.jpg' with the path to your image file
\end{minipage}
\vspace{0.5cm}

% Centered Table
\begin{center}
\begin{tabular}{|m{2cm}|m{7cm}|m{2cm}|m{2cm}|m{2cm}|m{2cm}|}
    \hline
    \textbf{Subject Code} & \textbf{Subject Title} & \textbf{Internal Marks} & \textbf{External Marks} & \textbf{Total Marks} & \textbf{Subject Result} \\
    \hline
"""

    # Add each row from the DataFrame to the LaTeX table
    for index, row in df.iterrows():
        latex_content += f"    {row['Subject Code']} & {row['Subject Title']} & {row['Internal Marks']} & {row['External Marks']} & {row['Total Marks']} & {row['Subject Result']} \\\\ \hline\n"

    # Total calculations
    total_marks = df['Total Marks'].sum()
    cgpa = 9.5  # Example CGPA, replace with actual calculation if needed
    class_result = "Outstanding"  # Example class result, replace as needed

    latex_content += r"""    \end{tabular}
\end{center}

\begin{center}
\begin{tabular}{|m{3cm}|m{6cm}|m{3cm}|m{2cm}|m{4cm}|}
\hline
    \textbf{Total Marks} : """ + str(total_marks) + r""" & \multicolumn{2}{|c|}{\textbf{Marks in Words} : """ + convert_to_words(total_marks) + r"""} & \textbf{CGPA}: """ + str(cgpa) + r""" & \textbf{Class}: """ + class_result + r""" \\
    \hline
\end{tabular}
\end{center} 

\begin{center}
\begin{tabular}{|m{3cm}|m{3cm}|m{3cm}|m{3cm}|m{4cm}|}
\hline
\multicolumn{5}{|c|}{\textbf{Nomenclature / Abbreviations}} \\
\hline
P $>$ PASS & F $>$ FAIL & A $>$ ABSENT & W $>$ WITHHELD & X, NE $>$ NOT ELIGIBLE \\
\hline
\end{tabular}
\end{center}

\vspace{3cm}

% Registrar signature
\begin{flushright}
    Sd/- \\
    \textbf{Principal}
\end{flushright}

\vspace{0.5cm}

% Footer with result sheet ID and date
\noindent
\vfill
\end{document}
"""

    # Write the LaTeX content to the output file
    with open(output_file, 'w') as f:
        f.write(latex_content)

    # Compile the LaTeX file to PDF
    compile_latex_to_pdf(output_file)

def convert_to_words(num):
    # Simple function to convert numbers to words (only works for numbers up to 9999)
    # You can expand this function as needed
    units = ["", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"]
    teens = ["Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"]
    tens = ["", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"]
    thousands = ["", "Thousand"]

    if num < 10:
        return units[num]
    elif num < 20:
        return teens[num - 10]
    elif num < 100:
        return tens[num // 10] + ('' if num % 10 == 0 else ' ' + units[num % 10])
    elif num < 1000:
        return units[num // 100] + ' Hundred' + ('' if num % 100 == 0 else ' and ' + convert_to_words(num % 100))
    elif num < 10000:
        return units[num // 1000] + ' Thousand' + ('' if num % 1000 == 0 else ' and ' + convert_to_words(num % 1000))

def compile_latex_to_pdf(latex_file):
    # Run pdflatex to generate PDF
    try:
        subprocess.run(['pdflatex', latex_file], check=True)
        print(f"Successfully created {latex_file.replace('.tex', '.pdf')}")
    except subprocess.CalledProcessError:
        print("Error occurred while generating PDF.")

# Usage example
csv_file_path = 'students.csv'  # Replace with your CSV file path
output_latex_file = 'output.tex'  # Output LaTeX file
generate_latex_from_csv(csv_file_path, output_latex_file)
