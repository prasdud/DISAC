#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <crow.h>

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
    crow::SimpleApp app;

    // Handle preflight (OPTIONS) requests
    CROW_ROUTE(app, "/generate").methods("OPTIONS"_method)
    ([](const crow::request& req) {
        crow::response resp(204);
        resp.set_header("Access-Control-Allow-Origin", "http://127.0.0.1:5500");
        resp.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resp.set_header("Access-Control-Allow-Headers", "Content-Type");
        return resp;
    });

    CROW_ROUTE(app, "/test").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response resp;
        resp.set_header("Access-Control-Allow-Origin", "*");
        resp.body = "Server is running!";
        return resp;
    });

    CROW_ROUTE(app, "/generate").methods("POST"_method)
    ([](const crow::request& req) {
        std::cout << "Received POST request to /generate" << std::endl;
        std::cout << "Request body: " << req.body << std::endl;

        auto resp = crow::response(200);
        resp.set_header("Access-Control-Allow-Origin", "http://127.0.0.1:5500");
        resp.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resp.set_header("Access-Control-Allow-Headers", "Content-Type");
        resp.set_header("Content-Type", "application/json");

        try {
            if (req.body.empty()) {
                std::cout << "Error: Empty request body" << std::endl;
                crow::json::wvalue error_response;
                error_response["status"] = "error";
                error_response["message"] = "Empty request body";
                resp.body = error_response.dump();
                resp.code = 400;
                return resp;
            }

            auto x = crow::json::load(req.body);
            if (!x) {
                std::cout << "Error: Invalid JSON" << std::endl;
                crow::json::wvalue error_response;
                error_response["status"] = "error";
                error_response["message"] = "Invalid JSON data";
                resp.body = error_response.dump();
                resp.code = 400;
                return resp;
            }

            std::vector<StudentResult> results;
            
            // Parse the subject arrays
            for (size_t i = 0; i < x["subjectCode"].size(); i++) {
                StudentResult result;
                result.subjectCode = x["subjectCode"][i].s();
                result.subjectTitle = x["subjectTitle"][i].s();
                result.internalMarks = x["internalMarks"][i].i();
                result.externalMarks = x["externalMarks"][i].i();
                result.totalMarks = x["totalMarks"][i].i();
                result.subjectResult = x["subjectResult"][i].s();
                results.push_back(result);
                
                std::cout << "Processed subject: " << result.subjectCode << std::endl;
            }

            // Generate LaTeX and PDF
            std::string outputLatexFile = "output.tex";
            generateLatex(results, outputLatexFile);
            compileLatexToPDF(outputLatexFile);

            crow::json::wvalue success_response;
            success_response["status"] = "success";
            success_response["pdfPath"] = "output.pdf";
            resp.body = success_response.dump();
            resp.code = 200;
            
            std::cout << "Successfully processed request" << std::endl;
            return resp;
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            crow::json::wvalue error_response;
            error_response["status"] = "error";
            error_response["message"] = e.what();
            resp.body = error_response.dump();
            resp.code = 500;
            return resp;
        }
    });

    // Start the server
    std::cout << "Server starting on http://0.0.0.0:3000" << std::endl;
    app.bindaddr("0.0.0.0").port(3000).multithreaded().run();
    return 0;
}

// int main(int argc, char const *argv[])
// {
//     // serving static files
//     crow::SimpleApp app;
//     CROW_ROUTE(app, "/")([]() {
//         std::ifstream file("../../site/generateCertificate.html");
//         std::stringstream buffer;
//         buffer << file.rdbuf();
//         return crow::response(buffer.str());
//     });

//     // route to handle file submission
//     CROW_ROUTE(app, "/generate").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
//         auto params = crow::query_string(req.body);
//         std::string name = params.get("name");
//         std::string dob = params.get("dob");

//         // Generate the PDF
//         std::string outputLatexFile = "output.tex";
//         generateLatex(results, outputLatexFile);
//         compileLatexToPDF(outputLatexFile);


//         // Return a success message
//         return crow::response(200, "PDF Generated! Check server folder.");
//     });

//     app.port(8000).multithreaded().run();

//     return 0;
// }








// int main() {
//     std::vector<StudentResult> results;

//     // Placeholder for data entry
//     int numberOfSubjects;
//     std::cout << "Enter the number of subjects: ";
//     std::cin >> numberOfSubjects;

//     for (int i =#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <crow.h>

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
    crow::SimpleApp app;

    // Handle preflight (OPTIONS) requests
    CROW_ROUTE(app, "/generate").methods("OPTIONS"_method)
    ([](const crow::request& req) {
        crow::response resp(204);
        resp.set_header("Access-Control-Allow-Origin", "http://127.0.0.1:5500");
        resp.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resp.set_header("Access-Control-Allow-Headers", "Content-Type");
        return resp;
    });

    CROW_ROUTE(app, "/test").methods("GET"_method)
    ([](const crow::request& req) {
        crow::response resp;
        resp.set_header("Access-Control-Allow-Origin", "*");
        resp.body = "Server is running!";
        return resp;
    });

    CROW_ROUTE(app, "/generate").methods("POST"_method)
    ([](const crow::request& req) {
        std::cout << "Received POST request to /generate" << std::endl;
        std::cout << "Request body: " << req.body << std::endl;

        auto resp = crow::response(200);
        resp.set_header("Access-Control-Allow-Origin", "http://127.0.0.1:5500");
        resp.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resp.set_header("Access-Control-Allow-Headers", "Content-Type");
        resp.set_header("Content-Type", "application/json");

        try {
            if (req.body.empty()) {
                std::cout << "Error: Empty request body" << std::endl;
                crow::json::wvalue error_response;
                error_response["status"] = "error";
                error_response["message"] = "Empty request body";
                resp.body = error_response.dump();
                resp.code = 400;
                return resp;
            }

            auto x = crow::json::load(req.body);
            if (!x) {
                std::cout << "Error: Invalid JSON" << std::endl;
                crow::json::wvalue error_response;
                error_response["status"] = "error";
                error_response["message"] = "Invalid JSON data";
                resp.body = error_response.dump();
                resp.code = 400;
                return resp;
            }

            std::vector<StudentResult> results;
            
            // Parse the subject arrays
            for (size_t i = 0; i < x["subjectCode"].size(); i++) {
                StudentResult result;
                result.subjectCode = x["subjectCode"][i].s();
                result.subjectTitle = x["subjectTitle"][i].s();
                result.internalMarks = x["internalMarks"][i].i();
                result.externalMarks = x["externalMarks"][i].i();
                result.totalMarks = x["totalMarks"][i].i();
                result.subjectResult = x["subjectResult"][i].s();
                results.push_back(result);
                
                std::cout << "Processed subject: " << result.subjectCode << std::endl;
            }

            // Generate LaTeX and PDF
            std::string outputLatexFile = "output.tex";
            generateLatex(results, outputLatexFile);
            compileLatexToPDF(outputLatexFile);

            crow::json::wvalue success_response;
            success_response["status"] = "success";
            success_response["pdfPath"] = "output.pdf";
            resp.body = success_response.dump();
            resp.code = 200;
            
            std::cout << "Successfully processed request" << std::endl;
            return resp;
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            crow::json::wvalue error_response;
            error_response["status"] = "error";
            error_response["message"] = e.what();
            resp.body = error_response.dump();
            resp.code = 500;
            return resp;
        }
    });

    // Start the server
    std::cout << "Server starting on http://0.0.0.0:3000" << std::endl;
    app.bindaddr("0.0.0.0").port(3000).multithreaded().run();
    return 0;
}
