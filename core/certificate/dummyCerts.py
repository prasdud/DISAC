import pandas as pd
import random
from faker import Faker
import os
import subprocess
import qrcode

# Initialize Faker
fake = Faker()

def convert_to_words(num):
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

def generate_dummy_data(num_records):
    data = []
    for _ in range(num_records):
        usn = f"1VI22CS{random.randint(100, 999)}"
        name = fake.name()
        subject_codes = [f"CS{random.randint(100, 999)}" for _ in range(5)]
        subject_titles = [fake.sentence(nb_words=3) for _ in range(5)]
        internal_marks = [random.randint(0, 50) for _ in range(5)]
        external_marks = [random.randint(0, 50) for _ in range(5)]
        total_marks = [internal + external for internal, external in zip(internal_marks, external_marks)]
        subject_results = ["Pass" if mark >= 40 else "Fail" for mark in total_marks]

        for code, title, internal, external, total, result in zip(subject_codes, subject_titles, internal_marks, external_marks, total_marks, subject_results):
            data.append({
                'USN': usn,
                'Student Name': name,
                'Subject Code': code,
                'Subject Title': title,
                'Internal Marks': internal,
                'External Marks': external,
                'Total Marks': total,
                'Subject Result': result
            })
    
    return pd.DataFrame(data)

def compile_latex_to_pdf(latex_file):
    try:
        subprocess.run(['pdflatex', latex_file], check=True)
        print(f"Successfully created {latex_file.replace('.tex', '.pdf')}")
    except subprocess.CalledProcessError:
        print("Error occurred while generating PDF.")

def generate_qr_code(data, filename='qr_code.png'):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=4,
    )
    qr.add_data(data)
    qr.make(fit=True)

    img = qr.make_image(fill='black', back_color='white')
    img.save(filename)

def generate_latex_from_csv(csv_file, output_file):
    df = pd.read_csv(csv_file)
    unique_usn = df['USN'].unique()

    for usn in unique_usn:
        student_data = df[df['USN'] == usn]
        
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
\textbf{USN:} """ + usn + r""" \\\\
\textbf{Student Name:} """ + student_data['Student Name'].iloc[0] + r""" \\\\
\textbf{Semester: } 4\\\\
\textbf{Month / Year: } August 2004
\end{minipage}
\begin{minipage}{0.2\textwidth}
    \includegraphics[width=\textwidth]{qr_code.png} % QR Code
\end{minipage}
\vspace{0.5cm}

% Centered Table
\begin{center}
\begin{tabular}{|m{2cm}|m{7cm}|m{2cm}|m{2cm}|m{2cm}|m{2cm}|}
    \hline
    \textbf{Subject Code} & \textbf{Subject Title} & \textbf{Internal Marks} & \textbf{External Marks} & \textbf{Total Marks} & \textbf{Subject Result} \\
    \hline
"""

        # Add each row for the student to the LaTeX table
        for index, row in student_data.iterrows():
            latex_content += f"    {row['Subject Code']} & {row['Subject Title']} & {row['Internal Marks']} & {row['External Marks']} & {row['Total Marks']} & {row['Subject Result']} \\\\ \hline\n"

        # Total calculations
        total_marks = student_data['Total Marks'].sum()
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
        output_file = f'certificates/{usn}.tex'
        with open(output_file, 'w') as f:
            f.write(latex_content)

        # Compile the LaTeX file to PDF
        compile_latex_to_pdf(output_file)

# Create a directory for certificates if it doesn't exist
os.makedirs('certificates', exist_ok=True)

# Generate a dummy QR code
generate_qr_code("Dummy QR Code Data")

# Generate dummy data and save to CSV
num_certificates = 500
dummy_data_df = generate_dummy_data(num_certificates)
csv_file_path = 'students.csv'
dummy_data_df.to_csv(csv_file_path, index=False)

# Generate the certificates
generate_latex_from_csv(csv_file_path, 'output.tex')
