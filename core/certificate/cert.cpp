#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

struct StudentResult {
    std::string subjectCode;
    std::string subjectTitle;
    int internalMarks;
    int externalMarks;
    int totalMarks;
    std::string subjectResult;
};

std::string convertToWords(int num) {
    // Convert numbers to words (up to 9999)
    const std::string units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const std::string teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const std::string tens[] = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

    if (num < 10) return units[num];
    if (num < 20) return teens[num - 10];
    if (num < 100) return tens[num / 10] + (num % 10 == 0 ? "" : " " + units[num % 10]);
    if (num < 1000) return units[num / 100] + " Hundred" + (num % 100 == 0 ? "" : " and " + convertToWords(num % 100));
    if (num < 10000) return units[num / 1000] + " Thousand" + (num % 1000 == 0 ? "" : " and " + convertToWords(num % 1000));
    return "";
}

void generateLatex(const std::vector<StudentResult> &results, const std::string &outputFile) {
    std::ofstream latexFile(outputFile);
    latexFile << "\\documentclass[a4paper,12pt]{article}\n"
              << "\\usepackage{graphicx}\n"
              << "\\usepackage{geometry}\n"
              << "\\geometry{a4paper, margin=10pt}\n"
              << "\\usepackage{array}\n"
              << "\\begin{document}\n\n"
              << "\\begin{minipage}{0.2\\textwidth}\n"
              << "    \\includegraphics[width=\\textwidth]{vtulogo.png}\n"
              << "\\end{minipage}\n"
              << "\\begin{minipage}{0.5\\textwidth}\n"
              << "    \\centering\n"
              << "    \\textbf{\\Large Visvesvaraya Technological University} \\\\\n"
              << "    \\vspace{0.2cm}\n"
              << "    \\textbf{\\textsc{\\large Vemana Institute of Technology}} \\\\\n"
              << "\\end{minipage}\n"
              << "\\begin{minipage}{0.2\\textwidth}\n"
              << "    \\includegraphics[width=\\textwidth]{vitlogo.jpg}\n"
              << "\\end{minipage}\n\n"
              << "\\begin{center}\n"
              << "    \\textbf{VTU PROVISIONAL RESULTS OF UG / PG June/July-2024 EXAMINATION}\n"
              << "\\end{center}\n\n"
              << "\\vspace{0.5cm}\n"
              << "\\noindent\n"
              << "\\begin{minipage}{0.7\\textwidth}\n"
              << "\\textbf{USN:} 1VI22CS106 \\\\\n"
              << "\\textbf{Student Name:} SANTHOSH KUMAR S\\\\\n"
              << "\\textbf{Semester: } 4\\\\\n"
              << "\\textbf{Month / Year: } August 2004\n"
              << "\\end{minipage}\n"
              << "\\begin{minipage}{0.2\\textwidth}\n"
              << "    \\includegraphics[width=\\textwidth]{vtulogo.png}\n"
              << "\\end{minipage}\n"
              << "\\vspace{0.5cm}\n\n"
              << "\\begin{center}\n"
              << "\\begin{tabular}{|m{2cm}|m{7cm}|m{2cm}|m{2cm}|m{2cm}|m{2cm}|}\n"
              << "    \\hline\n"
              << "    \\textbf{Subject Code} & \\textbf{Subject Title} & \\textbf{Internal Marks} & \\textbf{External Marks} & \\textbf{Total Marks} & \\textbf{Subject Result} \\\\\n"
              << "    \\hline\n";

    int totalMarks = 0;
    for (const auto &result : results) {
        latexFile << "    " << result.subjectCode << " & " << result.subjectTitle << " & "
                  << result.internalMarks << " & " << result.externalMarks << " & "
                  << result.totalMarks << " & " << result.subjectResult << " \\\\ \\hline\n";
        totalMarks += result.totalMarks;
    }

    latexFile << "    \\end{tabular}\n"
              << "\\end{center}\n\n"
              << "\\begin{center}\n"
              << "\\begin{tabular}{|m{3cm}|m{6cm}|m{3cm}|m{2cm}|m{4cm}|}\n"
              << "\\hline\n"
              << "    \\textbf{Total Marks} : " << totalMarks << " & \\multicolumn{2}{|c|}{\\textbf{Marks in Words} : " 
              << convertToWords(totalMarks) << "} & \\textbf{CGPA}: 9.5 & \\textbf{Class}: Outstanding \\\\\n"
              << "\\hline\n"
              << "\\end{tabular}\n"
              << "\\end{center}\n\n"
              << "\\begin{center}\n"
              << "\\begin{tabular}{|m{3cm}|m{3cm}|m{3cm}|m{3cm}|m{4cm}|}\n"
              << "\\hline\n"
              << "\\multicolumn{5}{|c|}{\\textbf{Nomenclature / Abbreviations}} \\\\\n"
              << "\\hline\n"
              << "P $>$ PASS & F $>$ FAIL & A $>$ ABSENT & W $>$ WITHHELD & X, NE $>$ NOT ELIGIBLE \\\\\n"
              << "\\hline\n"
              << "\\end{tabular}\n"
              << "\\end{center}\n\n"
              << "\\vspace{3cm}\n"
              << "\\begin{flushright}\n"
              << "    Sd/- \\\\\n"
              << "    \\textbf{Principal}\n"
              << "\\end{flushright}\n\n"
              << "\\vspace{0.5cm}\n"
              << "\\noindent\n"
              << "\\vfill\n"
              << "\\end{document}\n";

    latexFile.close();
}

void compileLatexToPDF(const std::string &latexFile) {
    std::string command = "pdflatex " + latexFile;
    if (system(command.c_str()) == 0) {
        std::cout << "Successfully created " << latexFile.substr(0, latexFile.find_last_of('.')) + ".pdf" << std::endl;
    } else {
        std::cerr << "Error occurred while generating PDF." << std::endl;
    }
}

int main() {
    std::vector<StudentResult> results;

    // Placeholder for data entry
    int numberOfSubjects;
    std::cout << "Enter the number of subjects: ";
    std::cin >> numberOfSubjects;

    for (int i = 0; i < numberOfSubjects; ++i) {
        StudentResult result;
        std::cout << "Enter details for subject " << (i + 1) << ":\n";

        std::cout << "Subject Code: ";
        std::cin >> result.subjectCode;

        std::cout << "Subject Title: ";
        std::cin.ignore(); // Clear the newline character from the input buffer
        std::getline(std::cin, result.subjectTitle);

        std::cout << "Internal Marks: ";
        std::cin >> result.internalMarks;

        std::cout << "External Marks: ";
        std::cin >> result.externalMarks;

        std::cout << "Total Marks: ";
        std::cin >> result.totalMarks;

        std::cout << "Subject Result: ";
        std::cin >> result.subjectResult;

        results.push_back(result);
    }

    std::string outputLatexFile = "output.tex";  // Output LaTeX file
    generateLatex(results, outputLatexFile);
    compileLatexToPDF(outputLatexFile);

    return 0;
}
