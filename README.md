# GradeConverter from JOJ's csv to Canvas's csv

## Usage

- Convert the csv file from JOJ to Canvas's format, which can be imported to Canvas's gradebook.

## Prerequisites

1. Clone the repository to your local workspace.
2. Under the same directory, create the file `BasicInformation.csv` containing each students' information.

   - The format of the file should be like this:

     ```csv
     [Student], [ID], [SIS User ID], [SIS Login ID], [Section]
     ```

   - For more information. please refer to [Canvas's official document](https://community.canvaslms.com/t5/指南中文版-讲师指南-instructor/我如何在评分册-Gradebook-中导入评分/ta-p/440592)

3. Under the same directory, create the file `output.csv`.
4. Download the csv file from JOJ and put it under the same directory.

## How to use

1. Compile the source code with the provided `Makefile` by running `make`.
2. There're totally five arguments for the program:
   - `exercise_scores.csv`: the csv file from JOJ, this is a command line argument.
   - `exerciseName`: the name of the exercise from canvas, read by `stdin`.
     - e.g. `Exercise 2`, `Exercise 3`, `Project 1`, `Project 2`, etc.
   - `totalScore`: the total score of the exercise on JOJ, read by `stdin`. This should be the number of test cases times 10.
   - `minimumPrecision`: the minimum precision of the score, read by `stdin`. This should be the precision that the score will be rounded to.
     - e.g. `0.5`, `1`, `0.25`, etc.
     - e.g. `0.5` will round the score to the nearest 0.5. 9.33 will be rounded to 9.5, 9.25 will be rounded to 9.5 like the `round` function in `<cmath>` library.
   - `isExercise`: whether the exercise is an exercise or a project, read by `stdin`. `y` for exercise, `n` for project. By default, this program assumes the points for an exercise is 10 and the points for a project is 100.
   - There're corresponding prompts for each argument typed by `stdin`, just follow the prompts to input the arguments.
3. The program will check if there's any collision in the scores. If there's no collision, the program will convert the scores and write the result to `output.csv`.
4. Then you may check the `output.csv` file to see the result and upload it to Canvas's gradebook.

## Sample Usage

```plaintext
./GradeConverter Exercise3.csv
-----------------------------------
Reading Basic Information...
-----------------------------------
Enter the name and ID of the assignment: Exercise 3
Enter the maximum score for the exercise: 310
Enter the minimum precision: 0.25
Is this an exercise? (y/n): y
Checking for score collisions...
-----------------------------------
No collisions found.
-----------------------------------
Processing scores...
-----------------------------------
Updated Basic Information with Exercise 3 scores has been written to output.csv
```

## Note

- This program is not robust and does not handle errors and exceptions carefully. You need to check the `output.csv` file to see if the conversion is successful and be careful with the input arguments.
- I have not tested this program vastly, so there might be some bugs. Feel free to report any bugs or issues to me.
- Make sure that the csv file is downloaded from JOJ's scoreboard.
- You should generate your own `BasicInformation.csv` file based on your course information.
- I didn't spend much time on improving the code quality, so the code might be a little bit messy. I will try to improve it in the future if I have time.
