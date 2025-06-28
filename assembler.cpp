/*****************************************************************************
TITLE: Claims																																
AUTHOR: R.murali karthikeya
ROLL NO.: 2301CS32
Declaration of Authorship
*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

map<int, int> originalCodes;
vector<string> errors;
vector<string> warnings;
vector<string> objectFile;
vector<string> listingFile;
unordered_map<string, pair<int, int>> opcodes;
unordered_map<string, int> extras;
set<string> existingLabels;
void initializeopcodes()
{
    opcodes = {
        {"SET", {-1, 1}},
        {"ldc", {0, 1}},
        {"adc", {1, 1}},
        {"ldl", {2, 1}},
        {"stl", {3, 1}},
        {"ldnl", {4, 1}},
        {"stnl", {5, 1}},
        {"add", {6, 0}},
        {"sub", {7, 0}},
        {"shl", {8, 0}},
        {"shr", {9, 0}},
        {"adj", {10, 1}},
        {"a2sp", {11, 0}},
        {"sp2a", {12, 0}},
        {"call", {13, 2}},
        {"return", {14, 0}},
        {"brz", {15, 2}},
        {"brlz", {16, 2}},
        {"br", {17, 2}},
        {"HALT", {18, 0}},
        {"data", {19, 1}}

    };
    extras = {
        {"data", 1},
        {"SET", 1}};
}
int mnemonicsopcode(string &command)
{
    if (opcodes.find(command) != opcodes.end())
    {
        return opcodes[command].first;
    }
    return -1;
}

std::vector<std::string> cleanCodeInput(const std::vector<std::string> &inputCode)
{
    std::vector<std::string> formattedCode;

    // Iterate over each line in the input code
    for (int lineIndex = 0; lineIndex < inputCode.size(); ++lineIndex)
    {
        std::string cleanedLine;
        bool isWhitespace = true;

        // Process each character in the current line
        for (char ch : inputCode[lineIndex])
        {
            if (ch == ';')
            {
                break; // Stop processing at a comment
            }

            if (ch != ' ' && ch != '\t')
            {
                isWhitespace = false; // A non-whitespace character has been encountered
            }

            // Skip leading whitespace
            if (isWhitespace && (ch == ' ' || ch == '\t'))
            {
                continue;
            }

            cleanedLine.push_back(ch); // Add the character to the cleaned line
        }

        // Check if the cleaned line contains alphabetic characters (indicating it's a valid line)
        bool containsAlpha = false;
        for (char ch : cleanedLine)
        {
            if (std::isalpha(ch))
            {
                containsAlpha = true;
                break;
            }
        }

        // If the line contains alphabetic characters, add it to the result
        if (containsAlpha)
        {
            originalCodes[formattedCode.size()] = lineIndex + 1;
            formattedCode.push_back(cleanedLine);
        }
    }

    return formattedCode;
}

bool checkLabelValidity(const std::string &label)
{
    if (label.empty())
    {
        return false; // Return false if the label is empty
    }

    // Extract the first character
    char initialChar = label.front();

    // Check if the first character is a letter or an underscore
    return (std::isalpha(initialChar) || initialChar == '_');
}

std::pair<long, bool> parseBaseValue(const std::string &input)
{
    if (input.empty())
    {
        return {0, true}; // Return 0 for empty input, considering it valid
    }

    long result = 0;
    std::istringstream stream(input);
    char delimiter = '\0'; // For checking base-specific prefix
    stream.str(input);
    if (input[0] == '0')
    {
        stream >> std::oct >> result;
        if (stream && stream.eof())
        {
            return {result, true}; // Successfully parsed as octal
        }
    }
      // Try decimal conversion
       stream.clear();
    stream >> result;
    if (stream && stream.eof())
    {
        return {result, true}; // Successfully parsed as decimal
    }
    
    // Try hexadecimal conversion if octal failed (check for '0x' or '0X' prefix)
    stream.clear();
    stream.str(input);
    if (input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X")
    {
        stream >> std::hex >> result;
        if (stream && stream.eof())
        {
            return {result, true}; // Successfully parsed as hexadecimal
        }
    }

    // If all conversions fail
    return {-1, false};
}
map<string, int> setLabel(vector<string> &_completeCode)
{
    int _count = 0;
    map<string, int> _setLabels;
    int _tempoo = 0;
    //int i=0;
    for (int i = 0; i < (int)_completeCode.size(); ++i)
    {
        int _semicolonCount = 0;
        for (int j = 0; j < (int)_completeCode[i].size(); ++j)
        {
            if (_completeCode[i][j] == ':')
            {
                _semicolonCount++;
            }
        }
        if (!_semicolonCount)
        {
            _count++;
            continue;
        }
        if (_semicolonCount > 1)
        {
            string _temp = "Wrong label format at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        string _label;
        int k = 0;
        while (_completeCode[i][k] != ':')
        {
            _label.push_back(_completeCode[i][k]);
            k++;
        }
        if (mnemonicsopcode(_label) != -1)
        {
            string _temp = "Label used \"" + _label + "\" is a programming keyword: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        if (checkLabelValidity(_label) == 0)
        {
            string _temp = "Label is empty or starts with a character other than an alphabet and underscore at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        if (_setLabels.find(_label) != _setLabels.end())
        {
            string _temp = "Label already used at line: ";
            _temp.append(to_string(originalCodes[i]));
            errors.push_back(_temp);
            continue;
        }
        _setLabels[_label] = _count + _tempoo;
        stringstream _temp(_completeCode[i]);
        string _temp1, _temp2, _temp3;

        _temp >> _temp1;
        _temp >> _temp2;
        _temp >> _temp3;
        if ((int)_temp2.size())
        {
            _count++;
        }
        if (_temp2 == "SET")
        {
            _setLabels[_label] = parseBaseValue(_temp3).first;
            _tempoo++;
        }
    }
    return _setLabels;
}

std::string formatHexadecimal(int value, int targetLength)
{
    std::stringstream hexStream;
    hexStream << std::hex << value;
    std::string hexString = hexStream.str();

    // Reverse the string to make padding easier (as we can add padding to the front)
    std::reverse(hexString.begin(), hexString.end());

    // Add leading zeros if necessary
    while (hexString.length() < targetLength)
    {
        hexString.push_back('0');
    }

    // If the string is longer than required, truncate it
    if (hexString.length() > targetLength)
    {
        hexString.resize(targetLength);
    }

    // Reverse the string back to the correct order
    std::reverse(hexString.begin(), hexString.end());

    return hexString;
}

int main(int argc, char *argv[])
{
    freopen(argv[1], "r", stdin);
    vector<string> completeInput;
    string inputLine;

    // Read input from stdin into completeInput vector
    while (getline(cin, inputLine))
    {
        completeInput.push_back(inputLine);
    }

    // Clean up input code (remove comments, whitespace, etc.)
    vector<string> processedCode = cleanCodeInput(completeInput);

    // Initialize opcode data
    initializeopcodes();

    // Set up labels from processed code
    map<string, int> labelRegistry = setLabel(processedCode);

    int lineCounter = 0;
    int codeIndex = 0;

    // Process each line of the cleaned code
    while (codeIndex < (int)processedCode.size())
    {
        string currentLine = processedCode[codeIndex];
        stringstream lineStream(currentLine);
        string currentMnemonic;
        lineStream >> currentMnemonic;

        // Check if current line represents a label (ends with ':')
        if (*currentMnemonic.rbegin() != ':')
        {
            int opcode = mnemonicsopcode(currentMnemonic);
            if (opcode == -1)
            {
                // Handle invalid mnemonic
                string errorMessage = "Invalid mnemonic at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            // Parse arguments
            stringstream argumentStream(currentLine);
            string argument;
            int argumentCount = 0;
            while (argumentStream >> argument)
            {
                argumentCount++;
            }

            if (argumentCount >= 3)
            {
                // Handle too many operands
                string errorMessage = "Too many operands at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            // Handle opcode with two operands
            if (opcodes[currentMnemonic].second == 2)
            {
                string argument;
                lineStream >> argument;
                existingLabels.insert(argument);
                if (labelRegistry.find(argument) == labelRegistry.end())
                {
                    // Handle undefined label
                    string errorMessage = argument + " label does not exist at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }

                string machineCode = formatHexadecimal(lineCounter, 8);
                machineCode.push_back(' ');
                machineCode.append(formatHexadecimal(labelRegistry[argument] - lineCounter - 1, 6));
                machineCode.append(formatHexadecimal(opcode, 2));

                if (currentMnemonic == "call")
                {
                    machineCode = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[argument], 6) + formatHexadecimal(opcode, 2);
                }

                objectFile.push_back(machineCode);
                listingFile.push_back(machineCode + '\t' + currentLine);
                lineCounter++;
                codeIndex++;
                continue;
            }

            // Handle "data" mnemonic
            if (currentMnemonic == "data")
            {
                string argument;
                lineStream >> argument;
                if (argument.empty())
                {
                    // Handle missing operand
                    string errorMessage = "Missing operand at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                pair<long int, bool> operand = parseBaseValue(argument);
                if (!operand.second)
                {
                    // Handle invalid operand format
                    string errorMessage = "Invalid operand at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }

                if (operand.first < INT_MIN || operand.first > INT_MAX)
                {
                    // Warn about potential overflow
                    string warningMessage = "Possible integer overflow at line: ";
                    warningMessage.append(to_string(originalCodes[codeIndex]));
                    warnings.push_back(warningMessage);
                }

                string machineCode = formatHexadecimal(lineCounter, 8);
                machineCode.push_back(' ');
                machineCode.append(formatHexadecimal(operand.first, 8));
                listingFile.push_back(machineCode + " " + processedCode[codeIndex]);
                objectFile.push_back(machineCode);
                lineCounter++;
                codeIndex++;
                continue;
            }

            string tempArgument;
            lineStream >> tempArgument;
            existingLabels.insert(tempArgument);

            if (opcodes[currentMnemonic].second && tempArgument.empty())
            {
                // Handle missing operand
                string errorMessage = "Missing operand at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }
            else if (opcodes[currentMnemonic].second == 0 && !tempArgument.empty())
            {
                // Handle invalid or extra operand
                string errorMessage = "Invalid or extra operand at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            // Process labels or constants
            if (!tempArgument.empty() && ((tempArgument[0] >= 'a' && tempArgument[0] <= 'z') || tempArgument[0] == '_'))
            {
                if (labelRegistry.find(tempArgument) == labelRegistry.end())
                {
                    // Handle undefined label
                    string errorMessage = tempArgument + " is not defined at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[tempArgument], 6) + formatHexadecimal(opcode, 2);
                listingFile.push_back(code + " " + currentLine);
                objectFile.push_back(code);
                lineCounter++;
                codeIndex++;
                continue;
            }
            else if (!tempArgument.empty() && ((tempArgument[0] >= 'A' && tempArgument[0] <= 'Z') || tempArgument[0] == '_'))
            {
                if (labelRegistry.find(tempArgument) == labelRegistry.end())
                {
                    // Handle undefined label
                    string errorMessage = tempArgument + " is not defined at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[tempArgument], 6) + formatHexadecimal(opcode, 2);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + currentLine);
                lineCounter++;
                codeIndex++;
                continue;
            }

            pair<long, bool> operandMod = parseBaseValue(tempArgument);
            if (!operandMod.second)
            {
                // Handle wrong operand format
                string errorMessage = "Invalid operand format at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            string code = formatHexadecimal(lineCounter++, 8) + " " + formatHexadecimal(operandMod.first, 6) + formatHexadecimal(opcode, 2);
            listingFile.push_back(code + " " + currentLine);
            objectFile.push_back(code);
            codeIndex++;
            continue;
        }
        else
        {
            // Handle lines with labels (ends with ':')
            string nextMnemonic;
            lineStream >> nextMnemonic;
            string prevMnemonic = currentMnemonic;
            currentMnemonic = nextMnemonic;
            stringstream checkStream(processedCode[codeIndex]);
            int argumentCount = 0;
            string temp;
            while (checkStream >> temp)
            {
                ++argumentCount;
            }

            if (argumentCount >= 4)
            {
                // Handle too many operands
                string errorMessage = "Extra operands at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            if (currentMnemonic.empty())
            {
                string code = formatHexadecimal(lineCounter, 8);
                code.push_back(' ');
                code.append(processedCode[codeIndex]);
                listingFile.push_back(code);
                codeIndex++;
                continue;
            }

            // Handle "SET" mnemonic
            if (currentMnemonic == "SET")
            {
                string argument;
                lineStream >> argument;
                pair<long, bool> operandMod = parseBaseValue(argument);
                if (!operandMod.second || argument.empty())
                {
                    // Handle missing or invalid operand for SET
                    string errorMessage = "Missing or invalid operand at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }

                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(operandMod.first, 8);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + currentLine);
                currentMnemonic.pop_back();
                prevMnemonic.pop_back();
                lineCounter++;
                codeIndex++;
                continue;
            }

            // Handle "data" mnemonic
            if (currentMnemonic == "data")
            {
                string operand;
                lineStream >> operand;
                if (operand.empty())
                {
                    // Handle missing operand for data
                    string errorMessage = "Missing operand at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                pair<long, bool> operandMod = parseBaseValue(operand);
                if (!operandMod.second)
                {
                    // Handle invalid operand format for data
                    string errorMessage = "Invalid operand format at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }

                if (operandMod.first < INT_MIN || operandMod.first > INT_MAX)
                {
                    // Handle potential overflow for data
                    string warningMessage = "Possible overflow at line: ";
                    warningMessage.append(to_string(originalCodes[codeIndex]));
                    warnings.push_back(warningMessage);
                }

                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(operandMod.first, 8);
                listingFile.push_back(code + " " + currentLine);
                objectFile.push_back(code);
                lineCounter++;
                codeIndex++;
                continue;
            }

            // Handle regular opcodes
            int opcode = mnemonicsopcode(currentMnemonic);
            if (opcode == -1)
            {
                // Handle invalid opcode
                string errorMessage = "Invalid mnemonic at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            // Handle opcode with two operands
            if (opcodes[currentMnemonic].second == 2)
            {
                string argument;
                lineStream >> argument;
                existingLabels.insert(argument);

                if (labelRegistry.find(argument) == labelRegistry.end())
                {
                    // Handle undefined label for operand
                    string errorMessage = argument + " label does not exist at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }

                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[argument] - lineCounter - 1, 6) + formatHexadecimal(opcode, 2);
                if (currentMnemonic == "call")
                {
                    code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[argument], 6) + formatHexadecimal(opcode, 2);
                }

                objectFile.push_back(code);
                listingFile.push_back(code + " " + currentLine);
                lineCounter++;
                codeIndex++;
                continue;
            }

            string argument;
            lineStream >> argument;
            existingLabels.insert(argument);

            if (opcodes[currentMnemonic].second && argument.empty())
            {
                // Handle missing operand
                string errorMessage = "Missing operand at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }
            else if (opcodes[currentMnemonic].second == 0 && !argument.empty())
            {
                // Handle invalid operand
                string errorMessage = "Invalid operand at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            if (!argument.empty() && ((argument[0] >= 'a' && argument[0] <= 'z') || argument[0] == '_'))
            {
                if (labelRegistry.find(argument) == labelRegistry.end())
                {
                    // Handle undefined label for operand
                    string errorMessage = argument + " is not defined at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[argument], 6) + formatHexadecimal(opcode, 2);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + currentLine);
                lineCounter++;
                codeIndex++;
                continue;
            }
            else if (!argument.empty() && ((argument[0] >= 'A' && argument[0] <= 'Z') || argument[0] == '_'))
            {
                if (labelRegistry.find(argument) == labelRegistry.end())
                {
                    // Handle undefined label for operand
                    string errorMessage = argument + " is not defined at line: ";
                    errorMessage.append(to_string(originalCodes[codeIndex]));
                    errors.push_back(errorMessage);
                    codeIndex++;
                    continue;
                }
                string code = formatHexadecimal(lineCounter, 8) + " " + formatHexadecimal(labelRegistry[argument], 6) + formatHexadecimal(opcode, 2);
                objectFile.push_back(code);
                listingFile.push_back(code + " " + currentLine);
                lineCounter++;
                codeIndex++;
                continue;
            }

            pair<long, bool> operandMod = parseBaseValue(argument);
            if (!operandMod.second)
            {
                // Handle invalid operand format
                string errorMessage = "Invalid operand format at line: ";
                errorMessage.append(to_string(originalCodes[codeIndex]));
                errors.push_back(errorMessage);
                codeIndex++;
                continue;
            }

            string code = formatHexadecimal(lineCounter++, 8) + " " + formatHexadecimal(operandMod.first, 6) + formatHexadecimal(opcode, 2);
            objectFile.push_back(code);
            listingFile.push_back(code + " " + currentLine);
            codeIndex++;
        }
    }

    string _inputFileName;
    int i = 0;
    while (i < (int)strlen(argv[1]))
    {
        if (argv[1][i] != '.')
        {
            _inputFileName.push_back(argv[1][i]);
        }
        else
        {
            break;
        }
        ++i;
    }
    vector<string> _errorsBackup = errors;
    string _logFileName = _inputFileName;
    _logFileName.append(".log");
    ofstream _tempFile;
    _tempFile.open(_logFileName);
    for (auto itr : labelRegistry)
    {
        if (existingLabels.count(itr.first) == 0)
        {
            errors.push_back("Warning: label \"" + itr.first + "\" not used");
        }
    }
    for (auto itr : warnings)
    {
        errors.push_back("Warning: " + itr);
    }
    for (auto itr : errors)
    {
        _tempFile << itr << endl;
    }
    _tempFile.close();
    if ((int)_errorsBackup.size())
    {
        cout << "Compilation errors encountered. Log file with errors and warnings: ";
        cout << _logFileName << endl;
        cout << "Errors:\n";
        for (auto itr : errors)
        {
            cout << itr << endl;
        }
        return 0;
    }
    else
    {
        cout << "Compilation Successful. Log file with warnings: ";
        cout << _logFileName << endl;
    }
    string _listFileName = _inputFileName;
    _listFileName.append(".l");
    _tempFile.open(_listFileName);
    for (auto itr : listingFile)
    {
        _tempFile << itr << endl;
    }
    _tempFile.close();
    cout << "The listing file is: " << _listFileName << endl;
    string _objectFileName = _inputFileName;
    _objectFileName.append(".o");
    _tempFile.open(_objectFileName, ios::binary | ios::out);
    for (auto it : objectFile)
    {
        string s1, s2;
        stringstream _tempo(it);
        _tempo >> s1 >> s2;
        int num = 0;
        reverse(s2.rbegin(), s2.rend());
        int i = 0;
        while (i <= 7)
        {
            int val = 0;
            if (s2[i] >= 'a' && s2[i] <= 'f')
            {
                val = (s2[i] - 'a' + 10);
            }
            else
            {
                val = s2[i] - '0';
            }
            num += val * (int)pow(16, i);
            ++i;
        }
        static_cast<int>(num);
        _tempFile.write((const char *)&num, sizeof(int));
    }
    _tempFile.close();
    cout << "The object file is: " << _objectFileName << endl;
    cout << "\nWarnings: \n";
    for (auto itr : errors)
    {
        _tempFile << itr << endl;
        cout << itr << endl;
    }
    return 0;
}