#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Adjusted paths for clarity
const string BASIC_INFORMATION = "BasicInformation.csv";
const string OUTPUT_FILE = "output.csv";

vector<string> extractContents(const string &str) {
  vector<string> tokens;
  string token;
  bool insideQuote = false; // Flag to track whether the current character is inside quotes
  for (char ch : str) {
    if (ch == '"') {
      insideQuote = !insideQuote; // Toggle the insideQuote flag
      token += ch;                // Add the quote to the current token
    } else if (ch == ',' && !insideQuote) {
      // If we hit a comma and we're not inside quotes, it's a field separator
      tokens.push_back(token);
      token.clear();
    } else {
      token += ch; // Add the character to the current token
    }
  }
  if (!token.empty() && token.back() == '\r') {
    token.pop_back(); // Remove the carriage return character if it exists
  }
  tokens.push_back(token); // Add the last token after the loop ends
  return tokens;
}

// Function to read exercise scores and map them by student name
unordered_map<string, string> readExerciseScores(const string &filePath) {
  unordered_map<string, string> scores;
  ifstream file(filePath);
  if (!file.is_open()) {
    cout << "Error: Unable to open file " << filePath << endl;
    throw runtime_error("Unable to open file");
  }
  string line;
  if (getline(file, line)) { // Skip header
    while (getline(file, line)) {
      auto tokens = extractContents(line);
      if (tokens.size() >= 2) { // Assuming the format is [Name, Score]
        scores[tokens[2]] = tokens[3];
      }
    }
  }
  file.close();
  return scores;
}

string processScore(const string &rawScore, int maxScore, double minimumPrecision, bool isExercise = true) {
  double score = stoi(rawScore);
  int canvasMaxScore = isExercise ? 10 : 90;
  score = (score / maxScore) * canvasMaxScore;
  // Adjust the score according to the minimumPrecision
  score = round(score / minimumPrecision) * minimumPrecision;

  // Convert double to string with specified precision
  stringstream stream;
  stream << fixed << setprecision(to_string(minimumPrecision).find('.') + 2) << score;
  return stream.str();
}

bool checkScoreCollisions(int max_score, int num_problems, double min_precision) {
  double score_per_problem = static_cast<double>(max_score) / num_problems;
  map<double, vector<int>> rounded_scores;

  for (int correct_problems = 0; correct_problems <= num_problems; correct_problems++) {
    double raw_score = correct_problems * score_per_problem;
    double rounded_score = round(raw_score / min_precision) * min_precision;

    rounded_scores[rounded_score].push_back(correct_problems);
  }

  bool collisions_found = false;
  for (auto const &pair : rounded_scores) {
    if (pair.second.size() > 1) {
      collisions_found = true;
      cout << pair.second.front();
      for (size_t i = 1; i < pair.second.size() - 1; i++) {
        cout << ", " << pair.second[i];
      }
      cout << " and " << pair.second.back() << " correct problems have the same rounded score: " << fixed << setprecision(2) << pair.first << endl;
    }
  }

  if (!collisions_found) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <exercise_scores.csv>" << endl;
    return 1;
  }

  const string EXERCISE_SCORES = argv[1];

  // Read scores from Exercise3.csv
  auto scores = readExerciseScores(EXERCISE_SCORES);

  ifstream basicInfoFile(BASIC_INFORMATION);
  ofstream outputFile(OUTPUT_FILE);
  string line;

  if (!basicInfoFile.is_open() || !outputFile.is_open()) {
    cout << "Error: Unable to open files." << endl;
    return 1;
  }

  cout << "-----------------------------------" << endl;
  cout << "Reading Basic Information..." << endl;
  cout << "-----------------------------------" << endl;

  // Read the exercise name
  cout << "Enter the name and ID of the assignment: ";
  string exerciseName;
  getline(cin, exerciseName);
  // Read the maximum score for the exercise and the minimum precision and whether it is an exercise or not
  cout << "Enter the maximum score for the exercise: ";
  int maxScore;
  cin >> maxScore;
  cout << "Enter the minimum precision: ";
  double minimumPrecision;
  cin >> minimumPrecision;
  cout << "Is this an exercise? (y/n): ";
  bool isExercise;
  char c;
  cin >> c;
  if (c == 'y') {
    isExercise = true;
  } else if (c == 'n') {
    isExercise = false;
  } else {
    cout << "Invalid input. Please enter 'y' or 'n'." << endl;
    return 1;
  }

  cout << "Checking for score collisions..." << endl;
  cout << "-----------------------------------" << endl;

  if (checkScoreCollisions((isExercise ? 10 : 100), maxScore / 10, minimumPrecision)) {
    cout << "No collisions found." << endl;
  } else {
    cout << "Collisions found." << endl;
    return 1;
  }

  cout << "-----------------------------------" << endl;
  cout << "Processing scores..." << endl;

  // Write the updated header
  if (getline(basicInfoFile, line)) {
    if (line.back() == '\r') {
      line.pop_back(); // Remove the carriage return character if it exists
    }
    line += ",";
    line += exerciseName;
    outputFile << line << endl;
  }

  // Process each student
  while (getline(basicInfoFile, line)) {
    vector<string> row = extractContents(line);
    string score = "0";                        // Default score if not found
    if (scores.find(row[3]) != scores.end()) { // Assuming the first column is the student name
      string rawScore = scores[row[3]];
      score = processScore(rawScore, maxScore, minimumPrecision, isExercise);
    }
    for (size_t i = 0; i < row.size(); ++i) {
      outputFile << row[i];
      if (i < row.size() - 1)
        outputFile << ",";
    }
    outputFile << "," << score << endl; // Append the score
  }

  basicInfoFile.close();
  outputFile.close();

  cout << "-----------------------------------" << endl;
  cout << "Updated Basic Information with " << exerciseName << " scores has been written to " << OUTPUT_FILE << endl;
  return 0;
}
